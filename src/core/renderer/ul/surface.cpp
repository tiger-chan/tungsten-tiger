#include "surface.hpp"

// TEMP texture vert
namespace tt {
	bgfx::VertexLayout& texture_surface_vert::layout() {
		static bgfx::VertexLayout layout{};
		layout.begin()
			.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0,  4, bgfx::AttribType::Uint8, true, true)
			.end();

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

		texture_uniform = bgfx::createUniform("Ultralight_Surface", bgfx::UniformType::Sampler);

		// Create our Texture object.
		auto flags = BGFX_SAMPLER_MIN_POINT
				   | BGFX_SAMPLER_U_CLAMP
				   | BGFX_SAMPLER_V_CLAMP
				   | BGFX_TEXTURE_RT;
		texture = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::BGRA8, flags);
		bgfx::setName(texture, "Ultralight Surface");

		frame_buffer = bgfx::createFrameBuffer(1, &texture, true);

		vert_buffer = bgfx::createVertexBuffer(bgfx::makeRef(verts, sizeof(verts)), texture_surface_vert::layout());
		idx_buffer = bgfx::createIndexBuffer(bgfx::makeRef(indicies, sizeof(indicies)));
	}

	void texture_surface::frame() {
		auto mem_ref = bgfx::makeRef(mem, sz);
		bgfx::updateTexture2D(texture, 0, 0, 0, 0, w, h, mem_ref);

		bgfx::setVertexBuffer(0, vert_buffer);
		bgfx::setIndexBuffer(idx_buffer);
		bgfx::setTexture(0, texture_uniform, texture);
	}

	void texture_surface::destroy() {
		if (mem) {
			bgfx::destroy(frame_buffer);
			bgfx::destroy(texture_uniform);
			bgfx::destroy(idx_buffer);
			bgfx::destroy(vert_buffer);
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