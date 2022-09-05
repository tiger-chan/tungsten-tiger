#if !defined(PLATFORM_RENDERER_HPP__)
#	define PLATFORM_RENDERER_HPP__

#	include <system_types.hpp>
#	include <core/renderer/components.hpp>
#	include <core/renderer/renderer_state.hpp>
#	include <core/renderer/texture_flags.hpp>
#	include <core/renderer/texture_format.hpp>
#	include <core/renderer/uniform_type.hpp>
#	include <core/renderer/vertex_buffer_flags.hpp>
#	include <core/renderer/shader_stage.hpp>

namespace tt {
	enum class uniform_handle : uint32 {};
	enum class texture_handle : uint32 {};
	enum class framebuffer_handle : uint32 {};
	enum class vertex_buffer_handle : uint32 {};
	enum class dyn_vertex_buffer_handle : uint32 {};
	enum class index_buffer_handle : uint32 {};
	enum class dyn_index_buffer_handle : uint32 {};
	enum class shader_handle : uint32 {};
	enum class shader_program_handle : uint32 {};

	struct vertex_layout;

	enum index_buffer_flags {
		IB_NONE,
		// Index buffer contains 32-bit indices.
		IB_INDEX_32 = 0x1000,
	};

	struct renderer_manager {
		virtual int init() = 0;
		virtual void shutdown() = 0;

		virtual uint32 height() const = 0;
		virtual uint32 width() const = 0;

		virtual struct material *get_materials() const {
			return nullptr;
		}

		// Start frame (bind buffers for usage)
		virtual void begin() = 0;
		// release buffers from usage
		virtual void end() = 0;
		virtual void submit(const shader_program_handle & program, uint8 view, uint32 depth = 0, uint8 flags = UINT8_MAX) = 0;
		// submit changes to the GPU
		virtual void present() = 0;

		virtual uniform_handle create_uniform(uniform_type type, const char *name) = 0;

		virtual texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint64 flags = TF_TEXTURE_NONE | TF_SAMPLER_NONE, const char *name = nullptr) = 0;

		virtual texture_handle create_texture_2d(uint16 w, uint16 h, texture_format format, uint16 num_layers, uint64 flags = TF_TEXTURE_NONE | TF_SAMPLER_NONE, const char *name = nullptr) = 0;

		virtual framebuffer_handle create_framebuffer(const texture_handle *textures, uint8 texture_count, bool destroy_textures) = 0;

		virtual vertex_buffer_handle create_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags = VB_NONE) = 0;
		
		virtual dyn_vertex_buffer_handle create_dyn_vertex_buffer(const void *mem, uint32 size, const vertex_layout &layout, vertex_buffer_flags flags = VB_NONE) = 0;

		virtual index_buffer_handle create_index_buffer(const void *mem, uint32 size, index_buffer_flags flags = IB_NONE) = 0;

		virtual dyn_index_buffer_handle create_dyn_index_buffer(const void *mem, uint32 size, index_buffer_flags flags = IB_NONE) = 0;

		virtual shader_handle create_shader(const char *name, shader_stage stage) = 0;
		
		virtual shader_program_handle create_shader_program(shader_handle vertex, shader_handle fragment) = 0;
		virtual shader_program_handle create_shader_program(shader_handle compute) = 0;

		virtual void update_texture_2d(texture_handle handle, uint16 layers, uint16 mip_layer, uint16 offset_x, uint16 offset_y, uint16 w, uint16 h, const void *mem, uint32 size, uint16 pitch = UINT16_MAX) = 0;

		virtual void set_vertex_buffer(dyn_vertex_buffer_handle handle, uint8 stream, int32 start, int32 count) = 0;
		virtual void set_vertex_buffer(vertex_buffer_handle handle, uint8 stream) = 0;
		virtual void set_index_buffer(dyn_index_buffer_handle handle, int32 start, int32 count) = 0;
		virtual void set_index_buffer(index_buffer_handle handle) = 0;
		virtual void set_texture(uniform_handle uniform, texture_handle texture, uint16 stage) = 0;
		virtual void set_state(uint64 state, uint32 rgba = 0) = 0;
		virtual void set_view_rect(uint8 view, uint16 x, uint16 y, uint16 width, uint16 height) = 0;
		virtual void set_view_transform(uint8 view_id, const float *view, const float *proj) = 0;

		virtual void touch(uint8 view) = 0;

		virtual void destroy(framebuffer_handle handle) = 0;
		virtual void destroy(texture_handle handle) = 0;
		virtual void destroy(uniform_handle handle) = 0;
		virtual void destroy(vertex_buffer_handle handle) = 0;
		virtual void destroy(dyn_vertex_buffer_handle handle) = 0;
		virtual void destroy(index_buffer_handle handle) = 0;
		virtual void destroy(dyn_index_buffer_handle handle) = 0;
		virtual void destroy(shader_handle handle) = 0;
		virtual void destroy(shader_program_handle handle) = 0;

		virtual void update(dyn_index_buffer_handle handle, byte *mem, int16 mem_size, int32 start) = 0;
		virtual void update(dyn_vertex_buffer_handle handle, byte *mem, int16 mem_size, int32 start) = 0;
	};

	extern renderer_manager &get_renderer();
}    // namespace tt

#endif    // PLATFORM_RENDERER_HPP__
