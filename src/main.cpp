#include <SDL.h>
#undef main
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>

#include "tungsten_tiger.hpp"

#include <iostream>

int main() {
	tungsten_tiger();

	SDL_Window *window = NULL;
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

	// Poll for events and wait till user closes window
	bool quit = false;
	SDL_Event currentEvent;
	while (!quit) {
		while (SDL_PollEvent(&currentEvent) != 0) {
			if (currentEvent.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	// Free up window
	SDL_DestroyWindow(window);
	// Shutdown SDL
	SDL_Quit();

	return EXIT_SUCCESS;
}
