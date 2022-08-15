#if !defined(CORE_RENDERER_TEXTURE_FLAGS_HPP__)
#	define CORE_RENDERER_TEXTURE_FLAGS_HPP__

#	include <system_types.hpp>

namespace tt {
	inline namespace texture_flags {
		/*
		 * Texture flags
		 */

		extern const uint64 TF_TEXTURE_NONE;
		// Texture will be used for MSAA sampling.
		extern const uint64 TF_TEXTURE_MSAA_SAMPLE;
		// Render target no MSAA.
		extern const uint64 TF_TEXTURE_RT;
		// Texture will be used for compute write.
		extern const uint64 TF_TEXTURE_COMPUTE_WRITE;
		// Sample texture as sRGB.
		extern const uint64 TF_TEXTURE_SRGB;
		// Texture will be used as blit destination.
		extern const uint64 TF_TEXTURE_BLIT_DST;
		// Texture will be used for read back from GPU.
		extern const uint64 TF_TEXTURE_READ_BACK;

		// Render target MSAAx2 mode.
		extern const uint64 TF_TEXTURE_RT_MSAA_X2;
		// Render target MSAAx4 mode.
		extern const uint64 TF_TEXTURE_RT_MSAA_X4;
		// Render target MSAAx8 mode.
		extern const uint64 TF_TEXTURE_RT_MSAA_X8;
		// Render target MSAAx16 mode.
		extern const uint64 TF_TEXTURE_RT_MSAA_X16;
		// Render target will be used for writing
		extern const uint64 TF_TEXTURE_RT_WRITE_ONLY;

		/*
		 * Sampler flags
		 */
		// Wrap U mode: Mirror
		extern const uint64 TF_SAMPLER_U_MIRROR;
		// Wrap U mode: Clamp
		extern const uint64 TF_SAMPLER_U_CLAMP;
		// Wrap U mode: Border
		extern const uint64 TF_SAMPLER_U_BORDER;

		extern const uint64 TF_SAMPLER_U_MASK;
		// Wrap V mode: Mirror
		extern const uint64 TF_SAMPLER_V_MIRROR;
		// Wrap V mode: Clamp
		extern const uint64 TF_SAMPLER_V_CLAMP;
		// Wrap V mode: Border
		extern const uint64 TF_SAMPLER_V_BORDER;

		extern const uint64 TF_SAMPLER_V_MASK;
		// Wrap W mode: Mirror
		extern const uint64 TF_SAMPLER_W_MIRROR;
		// Wrap W mode: Clamp
		extern const uint64 TF_SAMPLER_W_CLAMP;
		// Wrap W mode: Border
		extern const uint64 TF_SAMPLER_W_BORDER;

		// Min sampling mode: Point
		extern const uint64 TF_SAMPLER_MIN_POINT;
		// Min sampling mode: Anisotropic
		extern const uint64 TF_SAMPLER_MIN_ANISOTROPIC;

		// Mag sampling mode: Point
		extern const uint64 TF_SAMPLER_MAG_POINT;
		// Mag sampling mode: Anisotropic
		extern const uint64 TF_SAMPLER_MAG_ANISOTROPIC;

		extern const uint64 TF_SAMPLER_MAG_MASK;
		// Mip sampling mode: Point
		extern const uint64 TF_SAMPLER_MIP_POINT;

		// Compare when sampling depth texture: less.
		extern const uint64 TF_SAMPLER_COMPARE_LESS;
		// Compare when sampling depth texture: less or equal.
		extern const uint64 TF_SAMPLER_COMPARE_LEQUAL;
		// Compare when sampling depth texture: equal.
		extern const uint64 TF_SAMPLER_COMPARE_EQUAL;
		// Compare when sampling depth texture: greater or equal.
		extern const uint64 TF_SAMPLER_COMPARE_GEQUAL;
		// Compare when sampling depth texture: greater.
		extern const uint64 TF_SAMPLER_COMPARE_GREATER;
		// Compare when sampling depth texture: not equal.
		extern const uint64 TF_SAMPLER_COMPARE_NOTEQUAL;
		// Compare when sampling depth texture: never.
		extern const uint64 TF_SAMPLER_COMPARE_NEVER;
		// Compare when sampling depth texture: always.
		extern const uint64 TF_SAMPLER_COMPARE_ALWAYS;

		extern const uint64 TF_SAMPLER_COMPARE_MASK;
		extern const uint64 TF_SAMPLER_SAMPLE_STENCIL;

		extern const uint64 TF_SAMPLER_NONE;

		// Sample stencil instead of depth.
		extern const uint64 TF_SAMPLER_POINT;

		extern const uint64 TF_SAMPLER_UVW_MIRROR;

		extern const uint64 TF_SAMPLER_UVW_CLAMP;

		extern const uint64 TF_SAMPLER_UVW_BORDER;
	}    // namespace texture_flags
}    // namespace tt

#endif    // CORE_RENDERER_TEXTURE_FLAGS_HPP__
