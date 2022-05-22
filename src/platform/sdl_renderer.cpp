#include "sdl_renderer.hpp"

#include <SDL.h>
#include <iostream>
#undef main
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

namespace tt {
	int sdl_renderer_manager::init() {
		const int WIDTH = 640;
		const int HEIGHT = 480;

		// Initialize SDL systems
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
			return EXIT_FAILURE;
		}
		else {
			//Create a window
			window = SDL_CreateWindow("Window Title",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				WIDTH,
				HEIGHT,
				SDL_WINDOW_SHOWN);
			if (window == nullptr) {
				std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
				return EXIT_FAILURE;
			}
		}

		{
			bgfx::Init bgfx_init{};
			if (auto platform_result = get_platform_data(window, bgfx_init.platformData)) {
				return platform_result;
			}

			// TODO: This should be choosen through configuration.
			bgfx_init.type = bgfx::RendererType::Vulkan;

			// TODO: This should be choosen through configuration.
			bgfx_init.resolution.height = HEIGHT;
			bgfx_init.resolution.width = WIDTH;
			bgfx_init.resolution.reset = BGFX_RESET_VSYNC;

			// Render an empty frame
			bgfx::renderFrame();

			bgfx::init(bgfx_init);

			// Enable debug text.
			bgfx::setDebug(BGFX_DEBUG_TEXT /*| BGFX_DEBUG_STATS*/);

			// Set view rectangle for 0th view
			bgfx::setViewRect(0, 0, 0, uint16_t(WIDTH), uint16_t(HEIGHT));

			// Clear the view rect
			bgfx::setViewClear(0,
				BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
				0x443355FF,
				1.0f,
				0);

			// Set empty primitive on screen
			bgfx::touch(0);
		}

		return 0;
	}

	void sdl_renderer_manager::shutdown() {
		// Free up window
		SDL_DestroyWindow(window);

		bgfx::shutdown();
		// Shutdown SDL
		SDL_Quit();
	}

	renderer_manager &get_renderer() {
		static sdl_renderer_manager renderer{};
		return renderer;
	}
}    // namespace tt
