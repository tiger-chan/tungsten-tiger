#if !defined(CORE_RENDERER_UL_SURFACE_HPP__)
#	define CORE_RENDERER_UL_SURFACE_HPP__

#	include <system_types.hpp>
#	include <bgfx/bgfx.h>
#	include <Ultralight/platform/Surface.h>

namespace tt {
	class texture_surface : public ultralight::Surface {
	public:
		texture_surface(uint32 width, uint32 height);
		~texture_surface();

		texture_surface(texture_surface &&) = delete;
		texture_surface(const texture_surface &) = delete;
		void operator=(texture_surface &&) = delete;
		void operator=(const texture_surface &) = delete;

		uint32 width() const override;
		uint32 height() const override;
		uint32 row_bytes() const override;
		size_t size() const override;
		void *LockPixels() override;
		void UnlockPixels() override;
		void Resize(uint32 width, uint32 height) override;

	private:
		void destroy();
		uint32 w;
		uint32 h;
		uint32 row;
		uint32 sz;
		uint8 *mem;
		bgfx::TextureHandle texture;
		bgfx::FrameBufferHandle frame_buffer;
		bool mapped;
	};

	class texture_surface_factory : public ultralight::SurfaceFactory {
	public:
		texture_surface_factory();

		~texture_surface_factory();

		// Called by Ultralight when it wants to create a Surface.
		ultralight::Surface *CreateSurface(uint32_t width, uint32_t height) override;

		// Called by Ultralight when it wants to destroy a Surface.
		void DestroySurface(ultralight::Surface *surface) override;
	};
}    // namespace tt

#endif    // CORE_RENDERER_UL_SURFACE_HPP__
