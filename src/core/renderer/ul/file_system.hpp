#if !defined(CORE_RENDERER_UL_FILESYSTEM_HPP__)
#	define CORE_RENDERER_UL_FILESYSTEM_HPP__

#	include <Ultralight/platform/FileSystem.h>
#	include <Ultralight/String.h>
#	include <system_types.hpp>
#	include <entt/entity/registry.hpp>

namespace tt {
	class file_system : public ultralight::FileSystem {
	private:
		/**
		 * @brief Check if file path exists, return true if exists.
		 * 
		 * @param path 
		 * @return true if file exists
		 */
		bool FileExists(const ultralight::String16 &path) final;

		/**
		 * @brief Get file size of previously opened file, store result in 'result'. Return true on success.
		 * 
		 * @param handle 
		 * @param result 
		 * @return true if handle is valid
		 */
		bool GetFileSize(ultralight::FileHandle handle, int64_t &result) final;

		/**
		 * @brief Get file mime type (eg "text/html"), store result in 'result'.
		 * 
		 * @param path 
		 * @param result 
		 * @return True on success
		 */
		bool GetFileMimeType(const ultralight::String16 &path, ultralight::String16 &result) final;

		/**
		 * @brief Open file path for reading or writing. Return file handle on success, or invalidFileHandle on
		 * failure.
		 * 
		 * @param path 
		 * @param open_for_writing 
		 * @return ultralight::FileHandle 
		 */
		ultralight::FileHandle OpenFile(const ultralight::String16 &path, bool open_for_writing) final;

		/**
		 * @brief Close previously-opened file.
		 * 
		 * @param handle 
		 */
		void CloseFile(ultralight::FileHandle &handle) final;

		/**
		 * @brief Read from currently-opened file, return number of bytes read or -1 on failure.
		 * 
		 * @param handle 
		 * @param data 
		 * @param length 
		 * @return -1 on failure
		 */
		int64_t ReadFromFile(ultralight::FileHandle handle, char *data, int64_t length) final;

	private:
		entt::registry file_reg;
	};
}    // namespace tt

#endif    // CORE_RENDERER_UL_FILESYSTEM_HPP__
