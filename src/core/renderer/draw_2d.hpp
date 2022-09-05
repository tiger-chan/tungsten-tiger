#if !defined(CORE_RENDERER_RENDERER_2D_HPP__)
#	define CORE_RENDERER_RENDERER_2D_HPP__

#	include "components.hpp"

#	include <constr/string.hpp>
#	include <system_types.hpp>
#	include <core/renderer/vertex_layout.hpp>
#	include <core/renderer/memory/allocation.hpp>
#	include <platform/renderer.hpp>

#	include <vector>

namespace tt {
	struct line_vertex {
		float x, y, z;
		float u, v;
		tt::color rgba{};

		static const tt::vertex_layout &layout();
	};

	struct drawing_2d {
		struct render_command {
			gl::memory::allocation idx{};
			gl::memory::allocation vert{};
			int16 voffset{};
			int16 vcount{};
			int16 ioffset{};
			int16 icount{};
		};

		struct render_batch {

		};

		void init();
		void shutdown();
		void emplace(line_vertex *vertex, uint16 count, uint16 *indicies, uint16 index_count, tt::color color, float duration);
		void frame();

		tt::shader_handle vertex_shader;
		tt::shader_handle fragment_shader;
		tt::shader_program_handle shader_program;

		std::vector<render_command> allocations;
	};

	namespace draw_2d {

		drawing_2d &manager();

		/**
		 * @brief Set stroke color for lines generated after this is called
		 * 
		 * @param r value between 0.0 and 1.0 representing `red`
		 * @param g value between 0.0 and 1.0 representing `green`
		 * @param b value between 0.0 and 1.0 representing `blue`
		 * @param a value between 0.0 and 1.0 representing `alpha`
		 */
		void color(float r, float g, float b, float a = 1.0);
		void reset_color();

		void thickness(float size);

		/**
		 * @brief Sets the duration (in ticks) for how long to display the path
		 * 
		 * @param ticks 
		 */
		void duration(int32 ticks);

		/**
		 * @brief Sets the duration for how long to display the path
		 * 
		 * @param ticks 
		 */
		void duration(float seconds);

		void reset_duration();

		/**
		 * @brief Start a new path
		 * 
		 */
		void begin_path();

		/**
		 * @brief move the cursor to the point (x,y), without adding a segment to the the path;
		 * that is, without drawing anything
		 * 
		 * @param x horizontal position
		 * @param y vertial position
		 */
		void move_to(float x, float y);

		/**
		 * @brief Add a line segment to the path that starts at the current cursor location and
		 * ends at the point (x,y), and move the cursor to (x,y)
		 * 
		 * @param x horizontal position
		 * @param y vertial position
		 */
		void line_to(float x, float y);

		/**
		 * @brief submit the path formed
		 * 
		 */
		void end_path();

		/**
		 * @brief Draw line between the points provided
		 * 
		 * @param x0 
		 * @param y0 
		 * @param x1 
		 * @param y1 
		 */
		void line(float x0, float y0, float x1, float y1);

		/**
		 * @brief create rectangle from the points provided
		 * 
		 * @param x0 
		 * @param y0 
		 * @param x1 
		 * @param y1 
		 */
		void rect(float x0, float y0, float x1, float y1);

		/**
		 * @brief Create circe with the provided center, radius, and segments
		 * 
		 * @param cx center x coordinate
		 * @param cy center y coordinate
		 * @param r radius
		 * @param steps number of segments
		 */
		void circle(float cx, float cy, float r, int8 steps = 32);

		/**
		 * @brief Create oval with the provided center, radii, and segments
		 * 
		 * @param cx center x coordinate
		 * @param cy center y coordinate
		 * @param rx radius along the x axis
		 * @param ry radius along the y axis
		 * @param steps number of segments
		 */
		void oval(float cx, float cy, float rx, float ry, int8 steps = 32);

		/**
		 * @brief Create Arc of N degrees with the provided center, radii, and segments
		 * 
		 * @param cx center x coordinate
		 * @param cy center y coordinate
		 * @param rx radius along the x axis
		 * @param ry radius along the y axis
		 * @param sd starting section of the arc to draw (in radians)
		 * @param ed ending section of the arc to draw (in radians)
		 * @param steps 
		 */
		void arc(float cx, float cy, float rx, float ry, float sd, float ed, int8 steps = 32);
	}    // namespace draw_2d
}    // namespace tt

#endif    // CORE_RENDERER_RENDERER_2D_HPP__
