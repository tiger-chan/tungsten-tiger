#if !defined(CORE_RENDERER_RENDERER_3D_HPP__)
#	define CORE_RENDERER_RENDERER_3D_HPP__

#	include "components.hpp"

#	include <system_types.hpp>
#	include <core/string.hpp>

namespace tt {
	struct renderer_3d {
		// draw 2d shapes in 3d space: line, rect, circle
		// draw filled 2d shapes.
		virtual void line(float x, float y, float z, float h, float w, uint color = default_color, float thickness = 0.02) = 0;
		virtual void line(vec2, vec2, uint color = default_color, float thickness = 0.02) = 0;

		virtual void rect(float x, float y, float h, float w, uint color = default_color) = 0;
		virtual void rect(vec2, vec2, uint color = default_color) = 0;
		virtual void rect(tt::rect, uint color = default_color) = 0;

		virtual void fill_rect(float x, float y, float h, float w, uint color = default_color) = 0;
		virtual void fill_rect(vec2, vec2, uint color = default_color) = 0;
		virtual void fill_rect(tt::rect, uint color = default_color) = 0;

		virtual void circle(float x, float y, float r, uint color = default_color) = 0;
		virtual void circle(vec2, float, uint color = default_color) = 0;
		virtual void circle(tt::circle, uint color = default_color) = 0;

		virtual void fill_circle(float x, float y, float r, uint color = default_color) = 0;
		virtual void fill_circle(vec2, float, uint color = default_color) = 0;
		virtual void fill_circle(tt::circle, uint color = default_color) = 0;

		virtual void str(const tt::string &, vec2, const font & = default_font, uint color = default_color) = 0;

		// draw generic 3d shapes e.g., box sphere
		// draw meshes / scenes

		virtual void cam(const camera *) = 0;
	};

}    // namespace tt

#endif    // CORE_RENDERER_RENDERER_3D_HPP__
