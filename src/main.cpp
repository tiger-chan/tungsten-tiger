#include <SDL.h>
#undef main
#include "core/renderer/ul/surface.hpp"
#include "core/renderer/ul/file_system.hpp"
#include "platform/platform.hpp"
#include "platform/renderer.hpp"
#include "tungsten_tiger.hpp"

#include <AppCore/Platform.h>
#include <SDL2/SDL_syswm.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Platform.h>
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

	ultralight::Config config;
	config.scroll_timer_delay = 1.0 / 90.0;

	config.device_scale = 1.0;
	config.font_family_standard = "Arial";
	config.resource_path = "./assets/resources/";
	config.use_gpu_renderer = false;

	ultralight::Platform::instance().set_config(config);

	ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
	ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
	ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

	auto file_system = std::make_unique<tt::file_system>();
	auto surface_factory = std::make_unique<tt::texture_surface_factory>();
	ultralight::Platform::instance().set_surface_factory(surface_factory.get());
	ultralight::Platform::instance().set_file_system(file_system.get());

	tt::shader_handle shaders[2];
	tt::shader_program_handle ul_shader_prog;
	shaders[0] = tt::get_renderer().create_shader("vs_texture.bin", tt::shader_stage::vertex);
	shaders[1] = tt::get_renderer().create_shader("fs_texture.bin", tt::shader_stage::fragment);

	ul_shader_prog = tt::get_renderer().create_shader_program(shaders[0], shaders[1]);


	{
		auto ul_renderer = ultralight::Renderer::Create();

		auto ul_view = ul_renderer->CreateView(renderer.width(), renderer.height(), true, nullptr);

		ul_view->LoadURL("file:///index.html");

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
			ul_renderer->Update();

			accumulator += delta_time;

			if (accumulator >= 1 / 60.0) {
				ul_renderer->Render();
				

				bgfx::setViewRect(20, 0, 0, renderer.width(), renderer.height());

				bgfx::touch(0);

				auto surface = static_cast<tt::texture_surface*>(ul_view->surface());
				surface->frame();

				bgfx::setState(0 | BGFX_STATE_WRITE_MASK
							   | BGFX_STATE_DEPTH_TEST_LESS
							   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_INV_FACTOR, BGFX_STATE_BLEND_INV_SRC_ALPHA)
							   | BGFX_STATE_MSAA);

				tt::get_renderer().submit(ul_shader_prog, 20);

				bgfx::frame();
				accumulator -= 1 / 60.0;
			}
		}
	}

	tt::get_renderer().destroy(ul_shader_prog);
	tt::get_renderer().destroy(shaders[0]);
	tt::get_renderer().destroy(shaders[1]);

	renderer.shutdown();

	return EXIT_SUCCESS;
}
