#include <SDL.h>
#undef main
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "platform/platform.hpp"
#include "platform/renderer.hpp"

#include "tungsten_tiger.hpp"

#include <iostream>

int main() {
	tungsten_tiger();

	tt::platform& platform = tt::get_platform();
	tt::renderer_manager &renderer = tt::get_renderer();

	renderer.init();

	// Poll for events and wait till user closes window
	bool quit = false;
	SDL_Event currentEvent;
	while (!quit) {
		while (SDL_PollEvent(&currentEvent) != 0) {
			if (currentEvent.type == SDL_QUIT) {
				quit = true;
			}
		}
		bgfx::frame();
	}


	renderer.shutdown();

	return EXIT_SUCCESS;
}
