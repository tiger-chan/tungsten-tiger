#if !defined(CORE_PLATFORM_UL_SCREEN_OVERLAY_HPP__)
#	define CORE_PLATFORM_UL_SCREEN_OVERLAY_HPP__

#include <system_types.hpp>
#include "screen_overlay.hpp"
#include <entt/entity/fwd.hpp>

#include <Ultralight/Renderer.h>

namespace tt {
	enum class shader_handle : uint32;
	enum class shader_program_handle : uint32;

	struct ul_screen_overlay : public screen_overlay {
		void init() override;
		void shutdown() override;
		void update() override;
		void frame() override;

		void load_url(const char* url) override;
		void load_html(const char* html) override;

		shader_handle vertex_shader;
		shader_handle fragment_shader;
		shader_program_handle shader_program;

		ultralight::RefPtr<ultralight::Renderer> ul_renderer;
		ultralight::RefPtr<ultralight::View> ul_view;
	};

	extern screen_overlay &overlay();
}

#endif    // CORE_PLATFORM_UL_SCREEN_OVERLAY_HPP__
