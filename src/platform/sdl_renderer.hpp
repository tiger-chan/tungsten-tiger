#if !defined(PLATFORM_SDL_RENDERER_HPP__)
#	define PLATFORM_SDL_RENDERER_HPP__

#	include "renderer.hpp"

struct SDL_Window;
namespace bgfx {
	struct PlatformData;
}

namespace tt {
	class sdl_renderer_manager : public renderer_manager {
		int init() override;
		void shutdown() override;

		uint32 height() const override;
		uint32 width() const override;

		// Start frame (bind buffers for usage)
		void begin() override;
		// release buffers from usage
		void end() override;
		void submit(const entity &) override;
		// submit changes to the GPU
		void present() override;

		SDL_Window *window = nullptr;
	};

	extern int get_platform_data(SDL_Window *window, bgfx::PlatformData &pd);
}    // namespace tt

#endif    // PLATFORM_SDL_RENDERER_HPP__
