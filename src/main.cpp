#include <SDL.h>
#undef main
#include "core/renderer/ul/surface.hpp"
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

int main() {
	tungsten_tiger();

	tt::platform &platform = tt::get_platform();
	tt::renderer_manager &renderer = tt::get_renderer();

	renderer.init();

	ultralight::Config config;
	config.scroll_timer_delay = 1.0 / 90.0;

	config.device_scale = 2.0;
	config.font_family_standard = "Arial";
	config.resource_path = "./resources/";
	config.use_gpu_renderer = false;

	ultralight::Platform::instance().set_config(config);

	ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
	ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
	ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

	auto surface_factory = std::make_unique<tt::texture_surface_factory>();
	ultralight::Platform::instance().set_surface_factory(surface_factory.get());

	{
		auto ul_renderer = ultralight::Renderer::Create();

		auto ul_view = ul_renderer->CreateView(renderer.width(), renderer.height(), false, nullptr);

		ul_view->LoadURL("https://en.wikipedia.org");

		// Poll for events and wait till user closes window
		bool quit = false;
		SDL_Event currentEvent;
		while (!quit) {
			while (SDL_PollEvent(&currentEvent) != 0) {
				if (currentEvent.type == SDL_QUIT) {
					quit = true;
				}
			}
			ul_renderer->Update();

			ul_renderer->Render();
			//bgfx::submit(10, ul_shader_program);
			bgfx::frame();
		}
	}

	renderer.shutdown();

	return EXIT_SUCCESS;
}
