#if !defined(CORE_RENDERER_UL_GPU_DRIVER_HPP__)
#	define CORE_RENDERER_UL_GPU_DRIVER_HPP__

#	include <system_types.hpp>
#	include <Ultralight/platform/GPUDriver.h>
#	include <vector>
#	include <unordered_map>

namespace tt {
	namespace details {
		struct texture_entry {

		};
	}

	using texture_map = std::unordered_map<uint32, details::texture_entry>;

	class bgfx_gpu_driver : public ultralight::GPUDriver {
		~bgfx_gpu_driver();
		void BeginSynchronize() override;
		void EndSynchronize() override;
		void CreateTexture(uint32 texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) override;
		void UpdateTexture(uint32 texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) override;
		void DestroyTexture(uint32 texture_id) override;
		uint32 NextRenderBufferId() override;
		void CreateRenderBuffer(uint32 render_buffer_id, const ultralight::RenderBuffer &buffer) override;
		void DestroyRenderBuffer(uint32 render_buffer_id) override;
		uint32 NextGeometryId() override;
		void CreateGeometry(uint32 geometry_id, const ultralight::VertexBuffer &vertices, const ultralight::IndexBuffer &indices) override;
		void UpdateGeometry(uint32 geometry_id, const ultralight::VertexBuffer &vertices, const ultralight::IndexBuffer &indices) override;
		void DestroyGeometry(uint32 geometry_id) override;
		void UpdateCommandList(const ultralight::CommandList &list) override;

		private:
		uint32 next_texture_id = 1;
		uint32 next_render_buffer_id = 1;
		uint32 next_geometry_id = 1;
		std::vector<ultralight::Command> cmd_list;
		int batch_count;

		texture_map textures;
	};
}    // namespace tt

#endif    // CORE_RENDERER_UL_GPU_DRIVER_HPP__
