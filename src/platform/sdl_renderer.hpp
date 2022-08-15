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
		int init() override;
		void shutdown() override;

		uint32 height() const override;
		uint32 width() const override;

		// Start frame (bind buffers for usage)
		void begin() override;
		// release buffers from usage
		void end() override;
		void submit(const shader_program_handle & program, uint16 view, uint32 depth, uint8 flags) override;
		// submit changes to the GPU
		void present() override;

		uniform_handle create_uniform(uniform_type type, const char *name) override;

		texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint64 flags, const char *name) override;

		texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint16 num_layers, uint64 flags, const char *name) override;

		framebuffer_handle create_framebuffer(const texture_handle *textures, uint8 texture_count, bool destroy_textures) override;

		vertex_buffer_handle create_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags = VB_NONE) override;

		index_buffer_handle create_index_buffer(const void *mem, uint32 size, index_buffer_flags flags) override;

		shader_handle create_shader(const char *name, shader_stage stage) override;

		shader_program_handle create_shader_program(shader_handle vertex, shader_handle fragment) override;
		shader_program_handle create_shader_program(shader_handle compute) override;

		void update_texture_2d(texture_handle handle, uint16 layers, uint16 mip_layer, uint16 offset_x, uint16 offset_y, uint16 w, uint16 h, const void *mem, uint32 size, uint16 pitch = UINT16_MAX) override;

		void set_vertex_buffer(vertex_buffer_handle handle, uint8 stream) override;
		void set_index_buffer(index_buffer_handle handle) override;
		void set_texture(uniform_handle uniform, texture_handle texture, uint16 stage) override;

		void destroy(framebuffer_handle handle) override;
		void destroy(texture_handle handle) override;
		void destroy(uniform_handle handle) override;
		void destroy(vertex_buffer_handle handle) override;
		void destroy(index_buffer_handle handle) override;
		void destroy(shader_handle handle) override;
		void destroy(shader_program_handle handle) override;

		SDL_Window *window = nullptr;
		entt::registry registry{};
	};

	extern int get_platform_data(SDL_Window *window, bgfx::PlatformData &pd);
}    // namespace tt

#endif    // PLATFORM_SDL_RENDERER_HPP__
