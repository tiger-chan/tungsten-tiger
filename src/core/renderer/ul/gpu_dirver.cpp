#include "gpu_dirver.hpp"

#include <bgfx/bgfx.h>

namespace tt {
	bgfx_gpu_driver::~bgfx_gpu_driver() {
		// TODO: Cleanup
	}

	void bgfx_gpu_driver::BeginSynchronize() {
	}
	
	void bgfx_gpu_driver::EndSynchronize() {
	}

	void bgfx_gpu_driver::CreateTexture(uint32 texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) {
		// Create new texture based on bitmap
	}

	void bgfx_gpu_driver::UpdateTexture(uint32 texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) {
		// Update texture based on current bitmap
	}

	void bgfx_gpu_driver::DestroyTexture(uint32 texture_id) {
		// Destroy internal texture represented by id
	}

	uint32 bgfx_gpu_driver::NextRenderBufferId() {
		return next_render_buffer_id++;
	}

	void bgfx_gpu_driver::CreateRenderBuffer(uint32 render_buffer_id, const ultralight::RenderBuffer &buffer) {
		// Create render buffer based on ul::buffer
	}

	void bgfx_gpu_driver::DestroyRenderBuffer(uint32 render_buffer_id) {
		// Destroy internal render_buffer represented by id
	}

	uint32 bgfx_gpu_driver::NextGeometryId() {
		return next_geometry_id++;
	}

	void bgfx_gpu_driver::CreateGeometry(uint32 geometry_id, const ultralight::VertexBuffer &vertices, const ultralight::IndexBuffer &indices) {
		// Create vert/index buffer based on ul::buffers
	}

	void bgfx_gpu_driver::UpdateGeometry(uint32 geometry_id, const ultralight::VertexBuffer &vertices, const ultralight::IndexBuffer &indices) {
		// update vert/index buffer based on ul::buffers
	}

	void bgfx_gpu_driver::DestroyGeometry(uint32 geometry_id) {
		// Destroy internal geo represented by id
	}

	void bgfx_gpu_driver::UpdateCommandList(const ultralight::CommandList &list) {
		// Fill out command list based on commands stored in driver presently
	}
}