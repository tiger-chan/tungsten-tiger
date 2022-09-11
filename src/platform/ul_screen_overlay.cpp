
#include "ul_screen_overlay.hpp"

#include "core/renderer/ul/file_system.hpp"
#include "core/renderer/ul/surface.hpp"
#include "platform/platform.hpp"
#include "platform/renderer.hpp"
#include "renderer.hpp"

#include <AppCore/Platform.h>
#include <bgfx/bgfx.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Platform.h>

namespace {
	tt::file_system *file_sys() {
		static tt::file_system fs{};
		return &fs;
	}
	
	tt::texture_surface_factory *surface_factory() {
		static tt::texture_surface_factory tsf{};
		return &tsf;
	}
}

namespace tt {
	void ul_screen_overlay::init() {
		ultralight::Config config;
		config.scroll_timer_delay = 1.0 / 90.0;

		config.device_scale = 1.0;
		config.font_family_standard = "Arial";
		config.resource_path = "./assets/resources/";
		config.use_gpu_renderer = false;

		ultralight::Platform::instance().set_config(config);

		ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
		ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));

		ultralight::Platform::instance().set_surface_factory(::surface_factory());
		ultralight::Platform::instance().set_file_system(::file_sys());

		vertex_shader = tt::get_renderer().create_shader("ul_texture.vert.bin", tt::shader_stage::vertex);
		fragment_shader = tt::get_renderer().create_shader("ul_texture.frag.bin", tt::shader_stage::fragment);

		shader_program = tt::get_renderer().create_shader_program(vertex_shader, fragment_shader);

		ul_renderer = ultralight::Renderer::Create();
		ul_view = ul_renderer->CreateView(get_renderer().width(), get_renderer().height(), true, nullptr);
	}

	void ul_screen_overlay::shutdown() {
		ul_view.Swap(ultralight::RefPtr<ultralight::View>{});
		ul_renderer.Swap(ultralight::RefPtr<ultralight::Renderer>{});

		tt::get_renderer().destroy(shader_program);
		tt::get_renderer().destroy(fragment_shader);
		tt::get_renderer().destroy(vertex_shader);
	}

	void ul_screen_overlay::update() {
		ul_renderer->Update();
	}

	void ul_screen_overlay::frame() {
		ul_renderer->Render();
				
		tt::get_renderer().set_view_rect(20, 0, 0, get_renderer().width(), get_renderer().height());

		tt::get_renderer().touch(0);

		auto surface = static_cast<tt::texture_surface*>(ul_view->surface());
		surface->frame();

		tt::get_renderer().set_state(RS_STATE_WRITE_MASK
							   | RS_STATE_DEPTH_TEST_LESS
							   | RS_STATE_BLEND_FUNC(RS_STATE_BLEND_INV_FACTOR, RS_STATE_BLEND_INV_SRC_ALPHA)
							   | RS_STATE_MSAA);

		tt::get_renderer().submit(shader_program, 20);
	}

	void ul_screen_overlay::load_url(const char* url) {
		ul_view->LoadURL(url);
	}

	void ul_screen_overlay::load_html(const char* html) {
		ul_view->LoadHTML(html);
	}

	screen_overlay &overlay() {
		static ul_screen_overlay ol{};
		return ol;
	}
}    // namespace tt
