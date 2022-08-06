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

	///
	/// Our HTML string to load into the View.
	///
	const char *htmlString() {
		return R"(
    <html>
      <head>
        <style type="text/css">
          body {
            margin: 0;
            padding: 0;
            overflow: hidden;
            color: black;
            font-family: Arial;
            background: transparent;
            display: flex;
            justify-content: center;
            align-items: center;
          }
          div {
            width: 350px;
            height: 350px;
            text-align: center;
            border-radius: 25px;
            background: linear-gradient(-45deg, #e5eaf9, #f9eaf6);
            box-shadow: 0 7px 18px -6px #8f8ae1;
          }
          h1 {
            padding: 1em;
          }
          p {
            background: white;
            padding: 2em;
            margin: 40px;
            border-radius: 25px;
          }
        </style>
      </head>
      <body>
        <div>
          <h1>Hello World!</h1>
          <p>Welcome to Ultralight!</p>
        </div>
      </body>
    </html>
    )";
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

	byte *shader_data[2];
	bgfx::ShaderHandle shaders[2];
	bgfx::ProgramHandle ul_shader_prog = BGFX_INVALID_HANDLE;
	// Load shaders
	{
		std::filesystem::path root(std::filesystem::absolute("."));
		root /= "assets";
		root /= "shaders";

		switch (bgfx::getRendererType() )
		{
		case bgfx::RendererType::Noop:
		case bgfx::RendererType::Direct3D9:  root /= "dx9";   break;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: root /= "dx11";  break;
		case bgfx::RendererType::Agc:
		case bgfx::RendererType::Gnm:        root /= "pssl";  break;
		case bgfx::RendererType::Metal:      root /= "metal"; break;
		case bgfx::RendererType::Nvn:        root /= "nvn";   break;
		case bgfx::RendererType::OpenGL:     root /= "glsl";  break;
		case bgfx::RendererType::OpenGLES:   root /= "essl";  break;
		case bgfx::RendererType::Vulkan:     root /= "spirv"; break;
		case bgfx::RendererType::WebGPU:     root /= "spirv"; break;

		case bgfx::RendererType::Count:
			return EXIT_FAILURE;
		}

		auto fs_shader = root / "fs_texture.bin";
		auto vs_shader = root / "vs_texture.bin";
		auto as_str = vs_shader.string();
		auto [vs_bytes, vs_size] = read_binary(as_str.c_str());
		as_str = fs_shader.string();
		auto [fs_bytes, fs_size] = read_binary(as_str.c_str());

		shaders[0] = bgfx::createShader(bgfx::makeRef(vs_bytes, vs_size));
		shader_data[0] = vs_bytes;
		bgfx::setName(shaders[0], "vs_texture.bin");
		shaders[1] = bgfx::createShader(bgfx::makeRef(fs_bytes, fs_size));
		shader_data[1] = fs_bytes;
		bgfx::setName(shaders[1], "fs_texture.bin");
		
		ul_shader_prog = bgfx::createProgram(shaders[0], shaders[1]);
	}


	{
		auto ul_renderer = ultralight::Renderer::Create();

		auto ul_view = ul_renderer->CreateView(renderer.width(), renderer.height(), true, nullptr);

		//ul_view->LoadHTML(htmlString());
		//ul_view->LoadURL("https://google.com");
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

				bgfx::submit(20, ul_shader_prog);

				bgfx::frame();
				accumulator -= 1 / 60.0;
			}
		}
	}

	bgfx::destroy(ul_shader_prog);
	bgfx::destroy(shaders[0]);
	bgfx::destroy(shaders[1]);

	delete[] shader_data[0];
	delete[] shader_data[1];

	renderer.shutdown();

	return EXIT_SUCCESS;
}
