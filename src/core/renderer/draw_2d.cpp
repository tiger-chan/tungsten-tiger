#include "draw_2d.hpp"

#include <core/renderer/memory/allocator.hpp>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

namespace {
	static constexpr const float two_pi = 2 * M_PI;

	static thread_local tt::color stroke_color{ tt::default_color };
	static thread_local tt::color fill_color{ tt::default_color };
	static thread_local float thickness{ 1.0 };
	static thread_local float duration{ 1.0 };

	static thread_local glm::vec2 path[500]{};
	static thread_local glm::vec2 cursor{};
	static thread_local int16 head{ 0 };
}    // namespace

namespace tt {
	const tt::vertex_layout &line_vertex::layout() {
		static tt::vertex_layout result{
			[]() {
				tt::vertex_layout result;

				tt::layout_begin(result);
				tt::layout_add(result, tt::VA_POSITION, tt::VAT_FLOAT, 3);
				tt::layout_add(result, tt::VA_TEX_COORD_0, tt::VAT_FLOAT, 2);
				tt::layout_add(result, tt::VA_COLOR_0, tt::VAT_UINT8, 4, true);
				tt::layout_end(result);

				return result;
			}()
		};

		return result;
	}

	void drawing_2d::init() {
		vertex_shader = get_renderer().create_shader("line.vert.bin", shader_stage::vertex);
		fragment_shader = get_renderer().create_shader("line.frag.bin", shader_stage::fragment);

		shader_program = get_renderer().create_shader_program(vertex_shader, fragment_shader);

		allocations.reserve(50);
	}

	void drawing_2d::shutdown() {
		for (auto &&command: allocations) {
			dealloc(&command.vert);
			dealloc(&command.idx);
		}
		allocations.clear();

		tt::get_renderer().destroy(shader_program);
		tt::get_renderer().destroy(fragment_shader);
		tt::get_renderer().destroy(vertex_shader);
	}

	void drawing_2d::emplace(line_vertex *vertex, uint16 count, uint16 *indicies, uint16 index_count, tt::color color, float duration) {
		// add all the data to the memory pool associated to this drawing.

		auto &&command = allocations.emplace_back();
		command.vert = valloc(count, &line_vertex::layout());
		memcpy(map(command.vert), vertex, sizeof(*vertex) * count);
		unmap(command.vert);
		
		command.idx = ialloc(index_count);
		memcpy(map(command.idx), indicies, sizeof(*indicies) * index_count);
		unmap(command.idx);

		command.vcount = count;
		command.voffset = command.vert.offset;
		
		command.icount = index_count;
		command.ioffset = command.idx.offset;
	}

	void drawing_2d::frame() {
		auto &&renderer = get_renderer();
		uint8 view_id = 10;
		float w = renderer.width();
		float h = renderer.height();
		glm::mat4 proj = glm::ortho(
			0.0f, // left
			w, // right
			h, // bottom
			0.0f, // top
			0.1f, // near
			100.0f); // far

		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f)); 

		renderer.set_view_transform(view_id, glm::value_ptr<float>(view), glm::value_ptr<float>(proj));
		
		for (auto&& command : allocations) {
			auto&& vert = command.vert;
			auto&& idx = command.idx;
			renderer.set_vertex_buffer(vert.pool->data.vert_id, 0, command.voffset, command.vcount);
			renderer.set_index_buffer(idx.pool->data.idx_id, command.ioffset, command.icount);

			renderer.set_view_rect(view_id, 0, 0, renderer.width(), renderer.height());

			renderer.set_state(RS_STATE_WRITE_MASK
									| RS_STATE_DEPTH_TEST_LESS
									| RS_STATE_BLEND_FUNC(RS_STATE_BLEND_INV_FACTOR, RS_STATE_BLEND_INV_SRC_ALPHA)
									| RS_STATE_MSAA);

			renderer.submit(shader_program, view_id);

			dealloc(&vert);
			dealloc(&idx);
		}

		allocations.clear();
	}

	namespace draw_2d {
		drawing_2d &manager() {
			static drawing_2d r{};
			return r;
		};

		void color(float r, float g, float b, float a) {
			stroke_color.r = 255 * r;
			stroke_color.g = 255 * g;
			stroke_color.b = 255 * b;
			stroke_color.a = 255 * a;
		}

		void reset_color() {
			::stroke_color = tt::default_color;
		}

		void thickness(float size) {
			::thickness = tt::max(size, 1.0f);
		}

		void duration(int32 ticks) {
			// Currently assume fixed 60 ticks in a second
			::duration = ticks * 60.0f;
		}

		void duration(float seconds) {
			::duration = seconds;
		}

		void reset_duration() {
			::duration = 1.0f;
		}

		void begin_path() {
			cursor = {};
			head = 0;
		}

		void move_to(float x, float y) {
			if (head) {
				end_path();
			}
			cursor = { x, y };
		}

		void line_to(float x, float y) {
			path[head++] = cursor;
			cursor = { x, y };
		}

		void end_path() {
			if (!head) {
				// Not enough points to submit
				return;
			}
			path[head++] = cursor;

			int16 line_count = head - 1;

			// The width of the line when drawn
			float w = ::thickness;
			// Indicates if the start and end points of the path are the same
			bool is_closed = false;
			{
				auto &first = path[0];
				auto &last = path[line_count - 1];
				is_closed = tt::abs(last.x - first.x) < 0.1
						 && tt::abs(last.y - first.y) < 0.1;
			}

			line_vertex *verts = new line_vertex[line_count * 4];
			uint16 *indices = new ::uint16[line_count * 6];
			int16 index = 0;
			int16 id = 0;

			for (int16 i = 1; i < head; ++i, id += 6, index += 4) {
				// Generate list of verticies and indicies
				const auto &p1 = ::path[i - 1];
				const auto &p2 = ::path[i];

				// calculate the 4 verticies of the line
				auto dir = p2 - p1;
				float mag = glm::length(dir);
				dir /= mag;

				dir *= w;
				const auto perp = glm::vec2(-dir.y, dir.x);

				// Set verticies for line
				{
					static const auto set_vert = [](line_vertex *vert, glm::vec2 p) {
						vert->x = p.x;
						vert->y = p.y;
						vert->z = 10;
					};
					
					static const auto set_color = [](line_vertex *vert, tt::color color) {
						vert->rgba = color;
					};

					{
						auto p = p1 - dir - perp;
						set_vert(verts + index + 0, p);
						set_color(verts + index + 0, ::stroke_color);
					}

					{
						auto p = p2 + dir - perp;
						set_vert(verts + index + 1, p);
						set_color(verts + index + 1, ::stroke_color);
					}

					{
						auto p = p2 + dir + perp;
						set_vert(verts + index + 2, p);
						set_color(verts + index + 2, ::stroke_color);
					}

					{
						auto p = p1 - dir + perp;
						set_vert(verts + index + 3, p);
						set_color(verts + index + 3, ::stroke_color);
					}
				}

				// Set indicies for line
				{
					static const constexpr int16 tries[] = {
						1, 2, 0,
						0, 2, 3
					};

					for (int16 j = 0; j < sizeof(tries) / sizeof(tries[0]); ++j) {
						indices[id + j] = index + tries[j];
					}
				}
			}

			// Sumbit to renderer here
			manager().emplace(verts, index, indices, id, ::stroke_color, ::duration);
			delete[] verts;
			delete[] indices;

			// Clear after submission
			head = 0;
		}

		void line(float x0, float y0, float x1, float y1) {
			begin_path();
			move_to(x0, y0);
			line_to(x1, y1); 
			end_path();
		}

		void rect(float x0, float y0, float x1, float y1) {
			float x = tt::min(x0, x1);
			float y = tt::min(y0, y1);
			float dx = tt::abs(x0 - x1);
			float dy = tt::abs(y0 - y1);

			begin_path();
			move_to(x, y);    // Bottom left
			line_to(x, y + dy);    // Top left
			line_to(x + dx, y + dy);    // Top right
			line_to(x + dx, y);    // Bottom right
			line_to(x, y);    // Connect Bottom left
			end_path();
		}

		void circle(float cx, float cy, float r, int8 steps) {
			oval(cx, cy, r, r, steps);
		}

		void oval(float cx, float cy, float rx, float ry, int8 steps) {
			arc(cx, cy, rx, ry, 0, two_pi, steps);
		}

		void arc(float cx, float cy, float rx, float ry, float sd, float ed, int8 steps) {
			float d_radius = ed - sd;
			float step = d_radius / steps;
			float angle = sd;

			float x = cx + rx * cos(angle);
			float y = cy + ry * sin(angle);

			begin_path();

			move_to(x, y);
			for (int8 i = 0; i < steps; ++i) {
				angle += step;
				x = cx + rx * cos(angle);
				y = cy + ry * sin(angle);
				line_to(x, y);
			}

			end_path();
		}
	}    // namespace draw_2d
}    // namespace tt
