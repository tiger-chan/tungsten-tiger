#include "surface.hpp"

// texture_surface
namespace tt {
	texture_surface::texture_surface(uint32 width, uint32 height)
		: mem{ nullptr }
		, mapped{ false } {
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

		bgfx::ViewId surface_id = 10;
		bgfx::setViewName(surface_id, "Ultralight Surface");
		bgfx::setViewClear(surface_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
		bgfx::setViewRect(surface_id, w, h, bgfx::BackbufferRatio::Equal);
		bgfx::setViewFrameBuffer(surface_id, frame_buffer);

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

		// Create our Texture object.
		auto flags = BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			| BGFX_TEXTURE_RT;
		texture = bgfx::createTexture2D(w, h, false, 1, bgfx::TextureFormat::RGBA8, flags);
		bgfx::setName(texture, "Ultralight Surface");

		frame_buffer = bgfx::createFrameBuffer(1, &texture, true);
	}

	void texture_surface::destroy() {
		if (mem) {
			bgfx::destroy(frame_buffer);
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