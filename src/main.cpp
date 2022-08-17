#include <SDL.h>
#undef main
#include <system_types.hpp>
#include <platform/platform.hpp>
#include <platform/renderer.hpp>
#include <platform/screen_overlay.hpp>
#include "tungsten_tiger.hpp"

#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <iostream>


// These should be extracted to an global service.
#include <fstream>
#include <filesystem>
#include <chrono>

namespace {
	double to_milliseconds(const std::chrono::steady_clock::time_point &time) {
		static constexpr double converter(0.001);
		return std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count() * converter;
	}

	std::tuple<byte *, uint32> read_binary(const char *filename) {
		std::fstream f{filename, f.binary | f.in | f.ate};
		if (!f.is_open()) {
			return std::make_tuple(nullptr, 0);
		}

		uint32 size = f.tellg();
		byte *bytes = new  byte[size];
		
		f.seekg(0);
		f.read(static_cast<char *>((void*)bytes), size);

		f.close();

		return std::make_tuple(bytes, size);
	}
}

int main() {
	tungsten_tiger();

	tt::platform &platform = tt::get_platform();
	tt::renderer_manager &renderer = tt::get_renderer();

	renderer.init();
	tt::overlay().init();

	{
		tt::overlay().load_url("file:///index.html");

		double accumulator = 0;
		auto time = to_milliseconds(std::chrono::high_resolution_clock::now());
		auto last_frame = time - 1 / 60.0;

		// Poll for events and wait till user closes window
		bool quit = false;
		SDL_Event currentEvent;
		while (!quit) {
			time = to_milliseconds(std::chrono::high_resolution_clock::now());
			auto delta_time = time - last_frame;
			
			while (SDL_PollEvent(&currentEvent) != 0) {
				if (currentEvent.type == SDL_QUIT) {
					quit = true;
				}
			}
			tt::overlay().update();

			accumulator += delta_time;

			if (accumulator >= 1 / 60.0) {
				tt::overlay().frame();

				tt::get_renderer().present();
				accumulator -= 1 / 60.0;
			}
		}
	}

	tt::overlay().shutdown();

	renderer.shutdown();

	return EXIT_SUCCESS;
}
