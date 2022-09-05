#if !defined(PLATFORM_SDL_RENDERER_HPP__)
#	define PLATFORM_SDL_RENDERER_HPP__

#	include "renderer.hpp"
#	include <entt/entity/registry.hpp>

struct SDL_Window;
namespace bgfx {
	struct PlatformData;
}

namespace tt {
	class sdl_renderer_manager : public renderer_manager {
		int init() final;
		void shutdown() final;

		uint32 height() const final;
		uint32 width() const final;

		// Start frame (bind buffers for usage)
		void begin() final;
		// release buffers from usage
		void end() final;
		void submit(const shader_program_handle & program, uint8 view, uint32 depth, uint8 flags) final;
		// submit changes to the GPU
		void present() final;

		uniform_handle create_uniform(uniform_type type, const char *name) final;

		texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint64 flags, const char *name) final;

		texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint16 num_layers, uint64 flags, const char *name) final;

		framebuffer_handle create_framebuffer(const texture_handle *textures, uint8 texture_count, bool destroy_textures) final;

		vertex_buffer_handle create_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags) final;

		dyn_vertex_buffer_handle create_dyn_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags) final;

		index_buffer_handle create_index_buffer(const void *mem, uint32 size, index_buffer_flags flags) final;

		dyn_index_buffer_handle create_dyn_index_buffer(const void *mem, uint32 size, index_buffer_flags flags) final;

		shader_handle create_shader(const char *name, shader_stage stage) final;

		shader_program_handle create_shader_program(shader_handle vertex, shader_handle fragment) final;
		shader_program_handle create_shader_program(shader_handle compute) final;

		void update_texture_2d(texture_handle handle, uint16 layers, uint16 mip_layer, uint16 offset_x, uint16 offset_y, uint16 w, uint16 h, const void *mem, uint32 size, uint16 pitch = UINT16_MAX) final;

		void set_vertex_buffer(dyn_vertex_buffer_handle handle, uint8 stream, int32 start, int32 count) final;
		void set_vertex_buffer(vertex_buffer_handle handle, uint8 stream) final;
		void set_index_buffer(dyn_index_buffer_handle handle, int32 start, int32 count) final;
		void set_index_buffer(index_buffer_handle handle) final;
		void set_texture(uniform_handle uniform, texture_handle texture, uint16 stage) final;
		void set_state(uint64 state, uint32 rgba = 0) final;
		void set_view_rect(uint8 view, uint16 x, uint16 y, uint16 width, uint16 height) final;
		void set_view_transform(uint8 view_id, const float *view, const float *proj) final;

		void touch(uint8 view) final;

		void destroy(framebuffer_handle handle) final;
		void destroy(texture_handle handle) final;
		void destroy(uniform_handle handle) final;
		void destroy(vertex_buffer_handle handle) final;
		void destroy(dyn_vertex_buffer_handle handle) final;
		void destroy(index_buffer_handle handle) final;
		void destroy(dyn_index_buffer_handle handle) final;
		void destroy(shader_handle handle) final;
		void destroy(shader_program_handle handle) final;

		void update(dyn_index_buffer_handle handle, byte *mem, int16 mem_size, int32 start) final;
		void update(dyn_vertex_buffer_handle handle, byte *mem, int16 mem_size, int32 start) final;

		SDL_Window *window = nullptr;
		entt::registry registry{};
	};

	extern int get_platform_data(SDL_Window *window, bgfx::PlatformData &pd);
}    // namespace tt

#endif    // PLATFORM_SDL_RENDERER_HPP__
