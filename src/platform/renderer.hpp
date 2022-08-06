#if !defined(PLATFORM_RENDERER_HPP__)
#	define PLATFORM_RENDERER_HPP__

#	include <system_types.hpp>
#include <core/renderer/components.hpp>

namespace tt {
	struct renderer_manager {
		virtual int init() = 0;
		virtual void shutdown() = 0;

		virtual uint32 height() const = 0;
		virtual uint32 width() const = 0;

		virtual struct material *get_materials() const {
			return nullptr;
		}

		// Start frame (bind buffers for usage)
		virtual void begin() = 0;
		// release buffers from usage
		virtual void end() = 0;
		virtual void submit(const entity &) = 0;
		// submit changes to the GPU
		virtual void present() = 0;
	};

	extern renderer_manager &get_renderer();
}    // namespace tt

#endif    // PLATFORM_RENDERER_HPP__
