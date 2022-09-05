#include <SDL.h>
#undef main
#include <system_types.hpp>
#include <core/renderer/draw_2d.hpp>
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
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

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
	tt::draw_2d::manager().init();

	static constexpr const auto MS_PER_UPDATE = 1.0 / 60.0;

	{
		tt::overlay().load_url("file:///index.html");

		double cur_time = to_milliseconds(std::chrono::high_resolution_clock::now());
		double accum = 0.0;
		double t = 0.0;

		// Poll for events and wait till user closes window
		bool quit = false;
		SDL_Event currentEvent;
		while (!quit) {
			auto time = to_milliseconds(std::chrono::high_resolution_clock::now());
			double frame_time = time - cur_time;
			if (frame_time > 0.25) {
				frame_time = 0.25; // max out when debugging...
			}
			cur_time = time;
			accum += frame_time;
			
			// Process input
			while (SDL_PollEvent(&currentEvent) != 0) {
				if (currentEvent.type == SDL_QUIT) {
					quit = true;
				}
			}

			// Integrate Physics and game changes
			while (accum >= MS_PER_UPDATE) {
				tt::overlay().update();

				t += MS_PER_UPDATE;
				accum -= MS_PER_UPDATE;
			}

			double alpha = accum / MS_PER_UPDATE;

			// Render
			{
				tt::overlay().frame();

				{
					// tt::draw_2d::begin_path();
					auto x = tt::get_renderer().width();
					auto y = tt::get_renderer().height();
					tt::draw_2d::color(1.0f * ((sin(time) + 1) * 0.5f), 1.0f * ((cos(time) + 1) * 0.5f), 0.0f);

					tt::draw_2d::thickness(1.0f + 5.0f * ((sin(time) + 1) * 0.5f));
					//tt::draw_2d::line(0, 0, x, y);

					auto rx = 100.0f + 5.0f * ((sin(time) + 1) * 0.5f);
					auto ry = 100.0f + 10.0f * ((cos(time) + 1) * 0.5f);
					//tt::draw_2d::oval(x * 0.5f, y * 0.5f, rx, ry);

					tt::draw_2d::rect(x * 0.1f, y * 0.1f, x * 0.9f, y * 0.9f);

					//tt::draw_2d::arc(x * 0.1f, y * 0.1f, rx, ry, M_PI * 0.25, M_PI *0.4);

					//tt::draw_2d::circle(x * 0.5f, y * 0.5f, 50.0f + 5.0f * ((sin(time) + 1) * 0.5f));
				}

				tt::draw_2d::manager().frame();

				tt::get_renderer().present();
			}
		}
	}

	tt::draw_2d::manager().shutdown();
	tt::overlay().shutdown();

	renderer.shutdown();

	return EXIT_SUCCESS;
}
