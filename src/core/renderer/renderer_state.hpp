#if !defined(CORE_RENDERER_RENDERER_STATE_HPP__)
#	define CORE_RENDERER_RENDERER_STATE_HPP__

#	include <system_types.hpp>

namespace tt {
	// Enable R write.
	extern const uint64 RS_STATE_WRITE_R;
	// Enable G write.
	extern const uint64 RS_STATE_WRITE_G;
	// Enable B write.
	extern const uint64 RS_STATE_WRITE_B;
	// Enable alpha write.
	extern const uint64 RS_STATE_WRITE_A;
	// Enable depth write.
	extern const uint64 RS_STATE_WRITE_Z;
	/// Enable RGB write.
	extern const uint64 RS_STATE_WRITE_RGB;

	extern const uint64 RS_STATE_WRITE_MASK;

	// Enable depth test, less.
	extern const uint64 RS_STATE_DEPTH_TEST_LESS;
	// Enable depth test, less or equal.
	extern const uint64 RS_STATE_DEPTH_TEST_LEQUAL;
	// Enable depth test, equal.
	extern const uint64 RS_STATE_DEPTH_TEST_EQUAL;
	// Enable depth test, greater or equal.
	extern const uint64 RS_STATE_DEPTH_TEST_GEQUAL;
	// Enable depth test, greater.
	extern const uint64 RS_STATE_DEPTH_TEST_GREATER;
	// Enable depth test, not equal.
	extern const uint64 RS_STATE_DEPTH_TEST_NOTEQUAL;
	// Enable depth test, never.
	extern const uint64 RS_STATE_DEPTH_TEST_NEVER;
	// Enable depth test, always.
	extern const uint64 RS_STATE_DEPTH_TEST_ALWAYS;

	// 0, 0, 0, 0
	extern const uint64 RS_STATE_BLEND_ZERO;
	// 1, 1, 1, 1
	extern const uint64 RS_STATE_BLEND_ONE;
	// Rs, Gs, Bs, As
	extern const uint64 RS_STATE_BLEND_SRC_COLOR;
	// 1-Rs, 1-Gs, 1-Bs, 1-As
	extern const uint64 RS_STATE_BLEND_INV_SRC_COLOR;
	// As, As, As, As
	extern const uint64 RS_STATE_BLEND_SRC_ALPHA;
	// 1-As, 1-As, 1-As, 1-As
	extern const uint64 RS_STATE_BLEND_INV_SRC_ALPHA;
	// Ad, Ad, Ad, Ad
	extern const uint64 RS_STATE_BLEND_DST_ALPHA;
	// 1-Ad, 1-Ad, 1-Ad ,1-Ad
	extern const uint64 RS_STATE_BLEND_INV_DST_ALPHA;
	// Rd, Gd, Bd, Ad
	extern const uint64 RS_STATE_BLEND_DST_COLOR;
	// 1-Rd, 1-Gd, 1-Bd, 1-Ad
	extern const uint64 RS_STATE_BLEND_INV_DST_COLOR;
	// f, f, f, 1; f)
	extern const uint64 RS_STATE_BLEND_SRC_ALPHA_SAT;
	// Blend factor
	extern const uint64 RS_STATE_BLEND_FACTOR;
	// 1-Blend factor
	extern const uint64 RS_STATE_BLEND_INV_FACTOR;

	// Blend add: src + dst.
	extern const uint64 RS_STATE_BLEND_EQUATION_ADD;
	// Blend subtract: src - dst.
	extern const uint64 RS_STATE_BLEND_EQUATION_SUB;
	// Blend reverse subtract: dst - src.
	extern const uint64 RS_STATE_BLEND_EQUATION_REVSUB;
	// Blend min: min(src, dst).
	extern const uint64 RS_STATE_BLEND_EQUATION_MIN;
	// Blend max: max(src, dst).
	extern const uint64 RS_STATE_BLEND_EQUATION_MAX;

	// Cull clockwise triangles.
	extern const uint64 RS_STATE_CULL_CW;
	// Cull counter-clockwise triangles.
	extern const uint64 RS_STATE_CULL_CCW;

	// Tristrip.
	extern const uint64 RS_STATE_PT_TRISTRIP;
	// Lines.
	extern const uint64 RS_STATE_PT_LINES;
	// Line strip.
	extern const uint64 RS_STATE_PT_LINESTRIP;
	// Points.
	extern const uint64 RS_STATE_PT_POINTS;

	// Enable MSAA rasterization.
	extern const uint64 RS_STATE_MSAA;
	// Enable line AA rasterization.
	extern const uint64 RS_STATE_LINEAA;
	// Enable conservative rasterization.
	extern const uint64 RS_STATE_CONSERVATIVE_RASTER;
	// No state.
	extern const uint64 RS_STATE_NONE;
	// Front counter-clockwise (default is clockwise).
	extern const uint64 RS_STATE_FRONT_CCW;
	// Enable blend independent.
	extern const uint64 RS_STATE_BLEND_INDEPENDENT;
	// Enable alpha to coverage.
	extern const uint64 RS_STATE_BLEND_ALPHA_TO_COVERAGE;
	/// Default state is write to RGB, alpha, and depth with depth test less enabled, with clockwise
	/// culling and MSAA (when writing into MSAA frame buffer, otherwise this flag is ignored).
	extern const uint64 RS_STATE_DEFAULT;

	#define RS_STATE_BLEND_FUNC(src, dst) state_blend_func(src, dst)
	
	uint64 state_blend_func(uint64 src, uint64 dst);
}    // namespace tt

#endif    // CORE_RENDERER_RENDERER_STATE_HPP__
