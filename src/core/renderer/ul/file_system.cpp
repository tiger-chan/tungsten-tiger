#include "file_system.hpp"

#include <constr/string_util.hpp>
#include <mime/mime.hpp>
#include <filesystem>
#include <fstream>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>

namespace {
	struct file_handle {
		char *bytes{nullptr};
		int64 size{ 0 };
	};

	ultralight::FileHandle to_file_handle(entt::entity entity) {
		return static_cast<ultralight::FileHandle>(entity);
	}
	
	entt::entity to_entity_file(ultralight::FileHandle handle) {
		return static_cast<entt::entity>(handle);
	}
}    // namespace

namespace tt {
	bool file_system::FileExists(const ultralight::String16 &path) {
		std::filesystem::path p(path.data());
		return std::filesystem::exists(p);
	}

	bool file_system::GetFileSize(ultralight::FileHandle handle, int64_t &result) {
		auto entity = to_entity_file(handle);
		if (file_reg.valid(entity)) {
			return result = file_reg.get<file_handle>(entity).size, true;
		}
		return false;
	}

	bool file_system::GetFileMimeType(const ultralight::String16 &path, ultralight::String16 &result) {
		ultralight::String8 tmp_path{ ultralight::String(path).utf8() };
		const char* type;
		auto type_result = mime::typef(tmp_path.data(), &type);
		if (type_result == mime::MIME_SUCCESS) {
			result = ultralight::String(type).utf16();
			return true;
		}

		return false;
	}

	ultralight::FileHandle file_system::OpenFile(const ultralight::String16 &path, bool open_for_writing) {
		const auto& config = ultralight::Platform::instance().config();
		auto full_path = std::filesystem::path(config.resource_path.data());
		full_path /= path.data();
		full_path = std::filesystem::absolute(full_path);
		std::fstream file;
		auto flags = file.ate | file.in | file.binary;
		if (open_for_writing) {
			flags |= file.out;
		}
		file.open(full_path.c_str(), flags);

		if (!file.is_open()) {
			return ultralight::invalidFileHandle;
		}

		auto entity = file_reg.create();
		file_handle handle;

		handle.size = file.tellg();
		file.seekg(0);
		handle.bytes = new char[handle.size];
		file.read(handle.bytes, handle.size);

		file_reg.emplace<file_handle>(entity, std::move(handle));
		return to_file_handle(entity);
	}

	void file_system::CloseFile(ultralight::FileHandle &handle) {
		auto entity = to_entity_file(handle);
		delete[] file_reg.get<file_handle>(entity).bytes;
		file_reg.destroy(entity);
	}

	int64_t file_system::ReadFromFile(ultralight::FileHandle handle, char *data, int64_t length) {
		auto entity = to_entity_file(handle);
		if (file_reg.valid(entity)) {
			auto& file = file_reg.get<file_handle>(entity);
			auto read_bytes = tt::min(length, file.size);
			constr::strncpy(data, file.bytes, read_bytes);
			return read_bytes;
		}

		return -1;
	}
}    // namespace tt