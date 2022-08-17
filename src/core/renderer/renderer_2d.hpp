#if !defined(CORE_RENDERER_RENDERER_2D_HPP__)
#	define CORE_RENDERER_RENDERER_2D_HPP__

#	include "components.hpp"

#	include <core/string.hpp>
#	include <system_types.hpp>

namespace tt {
	struct renderer_2d {
		virtual void line(float x1, float y1, float x2, float y2, uint color = default_color, float thickness = 0.02) = 0;
		virtual void line(vec2 p1, vec2 p2, uint color = default_color, float thickness = 0.02) = 0;

		virtual void rect(float x, float y, float h, float w, uint color = default_color) = 0;
		virtual void rect(vec2 c, vec2 extents, uint color = default_color) = 0;
		virtual void rect(tt::rect, uint color = default_color) = 0;

		virtual void fill_rect(float cx, float cy, float h, float w, uint color = default_color) = 0;
		virtual void fill_rect(vec2 c, vec2 extents, uint color = default_color) = 0;
		virtual void fill_rect(tt::rect, uint color = default_color) = 0;

		virtual void circle(float x, float y, float r, uint color = default_color) = 0;
		virtual void circle(vec2 c, float r, uint color = default_color) = 0;
		virtual void circle(tt::circle, uint color = default_color) = 0;

		virtual void fill_circle(float x, float y, float r, uint color = default_color) = 0;
		virtual void fill_circle(vec2 c, float r, uint color = default_color) = 0;
		virtual void fill_circle(tt::circle, uint color = default_color) = 0;

		virtual void str(const tt::string &, vec2 c, const font & = default_font, uint color = default_color) = 0;

		virtual void cam(const camera *) = 0;
	};
}    // namespace tt

#endif    // CORE_RENDERER_RENDERER_2D_HPP__
