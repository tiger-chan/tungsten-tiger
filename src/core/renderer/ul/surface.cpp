#include "surface.hpp"
#include <core/renderer/vertex_layout.hpp>

// TEMP texture vert
namespace tt {
	vertex_layout& texture_surface_vert::layout() {
		static vertex_layout layout{};
		tt::layout_begin(layout);
		tt::layout_add(layout, VA_POSITION, VAT_FLOAT, 3);
		tt::layout_add(layout, VA_TEX_COORD_0, VAT_UINT8, 4, true, true);
		tt::layout_end(layout);

		return layout;
	}
}    // namespace tt

// texture_surface
namespace tt {
	texture_surface::texture_surface(uint32 width, uint32 height)
		: mem{ nullptr }
		, mapped{ false }
		, verts{
			texture_surface_vert{-1.0f, -1.0f, 0.0f, 0, UINT8_MAX},
			texture_surface_vert{1.0f, -1.0f, 0.0f, UINT8_MAX, UINT8_MAX},
			texture_surface_vert{1.0f, 1.0f, 0.0f, UINT8_MAX, 0},
			texture_surface_vert{-1.0f, 1.0f, 0.0f, 0, 0},
		}
		, indicies{
			1, 2, 0,
			0, 2, 3} {
		Resize(width, height);
	}

	texture_surface::~texture_surface() {
		// If it's been allocated delete the pbo and texture
		destroy();
	}

	uint32 texture_surface::width() const {
		return w;
	}

	uint32 texture_surface::height() const {
		return h;
	}

	uint32 texture_surface::row_bytes() const {
		return row;
	}

	size_t texture_surface::size() const {
		return sz;
	}

	void *texture_surface::LockPixels() {
		// Map our PBO to system memory so Ultralight can draw to it.
		if (mapped) {
			return nullptr;
		}
		mapped = true;
		return mem;
	}

	void texture_surface::UnlockPixels() {
		// Unmap our PBO.

		if (!mapped) {
			return;
		}

		mapped = false;
	}

	void texture_surface::Resize(uint32 width, uint32 height) {
		// Destroy any existing PBO and texture.
		destroy();

		w = width;
		h = height;
		row = w * 4;
		sz = row * h;

		// Create our PBO (pixel buffer object), with a size of 'sz'
		mem = new uint8[sz];
		std::memset(mem, '\0', sz);

		
		texture_uniform = get_renderer().create_uniform(UT_SAMPLER, "ultralight_surface");

		// Create our Texture object.
		auto flags = TF_SAMPLER_MIN_POINT
				   | TF_SAMPLER_U_CLAMP
				   | TF_SAMPLER_V_CLAMP
				   | TF_TEXTURE_RT;
		
		texture = get_renderer().create_texture_2d(w, h, TF_BGRA8, flags, "ultralight surface texture");

		frame_buffer = get_renderer().create_framebuffer(&texture, 1, true);
		vert_buffer = get_renderer().create_vertex_buffer(verts, sizeof(verts), texture_surface_vert::layout());
		idx_buffer = get_renderer().create_index_buffer(indicies, sizeof(indicies));
	}

	void texture_surface::frame() {
		get_renderer().update_texture_2d(texture, 0, 0, 0, 0, w, h, mem, sz);

		get_renderer().set_vertex_buffer(vert_buffer, 0);
		get_renderer().set_index_buffer(idx_buffer);
		get_renderer().set_texture(texture_uniform, texture, 0);
	}

	void texture_surface::destroy() {
		if (mem) {
			get_renderer().destroy(frame_buffer);
			get_renderer().destroy(texture_uniform);
			get_renderer().destroy(idx_buffer);
			get_renderer().destroy(vert_buffer);
			delete[] mem;
			mem = nullptr;
		}
	}
}    // namespace tt

// texture_surface_factory
namespace tt {
	texture_surface_factory::texture_surface_factory() {
	}

	texture_surface_factory::~texture_surface_factory() {
	}

	ultralight::Surface *texture_surface_factory::CreateSurface(uint32_t width, uint32_t height) {
		return new texture_surface(width, height);
	}

	void texture_surface_factory::DestroySurface(ultralight::Surface *surface) {
		// I don't think this cast should need to be here...
		delete static_cast<texture_surface *>(surface);
	}
}    // namespace tt