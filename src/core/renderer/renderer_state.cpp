#include "renderer_state.hpp"
#include <bgfx/bgfx.h>

namespace tt {
	// Enable R write.
	const uint64 RS_STATE_WRITE_R = BGFX_STATE_WRITE_R;
	// Enable G write.
	const uint64 RS_STATE_WRITE_G = BGFX_STATE_WRITE_G;
	// Enable B write.
	const uint64 RS_STATE_WRITE_B = BGFX_STATE_WRITE_B;
	// Enable alpha write.
	const uint64 RS_STATE_WRITE_A = BGFX_STATE_WRITE_A;
	// Enable depth write.
	const uint64 RS_STATE_WRITE_Z = BGFX_STATE_WRITE_Z;
	/// Enable RGB write.
	const uint64 RS_STATE_WRITE_RGB = BGFX_STATE_WRITE_RGB;

	const uint64 RS_STATE_WRITE_MASK = BGFX_STATE_WRITE_MASK;

	// Enable depth test, less.
	const uint64 RS_STATE_DEPTH_TEST_LESS = BGFX_STATE_DEPTH_TEST_LESS;
	// Enable depth test, less or equal.
	const uint64 RS_STATE_DEPTH_TEST_LEQUAL = BGFX_STATE_DEPTH_TEST_LEQUAL;
	// Enable depth test, equal.
	const uint64 RS_STATE_DEPTH_TEST_EQUAL = BGFX_STATE_DEPTH_TEST_EQUAL;
	// Enable depth test, greater or equal.
	const uint64 RS_STATE_DEPTH_TEST_GEQUAL = BGFX_STATE_DEPTH_TEST_GEQUAL;
	// Enable depth test, greater.
	const uint64 RS_STATE_DEPTH_TEST_GREATER = BGFX_STATE_DEPTH_TEST_GREATER;
	// Enable depth test, not equal.
	const uint64 RS_STATE_DEPTH_TEST_NOTEQUAL = BGFX_STATE_DEPTH_TEST_NOTEQUAL;
	// Enable depth test, never.
	const uint64 RS_STATE_DEPTH_TEST_NEVER = BGFX_STATE_DEPTH_TEST_NEVER;
	// Enable depth test, always.
	const uint64 RS_STATE_DEPTH_TEST_ALWAYS = BGFX_STATE_DEPTH_TEST_ALWAYS;

	// 0, 0, 0, 0
	const uint64 RS_STATE_BLEND_ZERO = BGFX_STATE_BLEND_ZERO;
	// 1, 1, 1, 1
	const uint64 RS_STATE_BLEND_ONE = BGFX_STATE_BLEND_ONE;
	// Rs, Gs, Bs, As
	const uint64 RS_STATE_BLEND_SRC_COLOR = BGFX_STATE_BLEND_SRC_COLOR;
	// 1-Rs, 1-Gs, 1-Bs, 1-As
	const uint64 RS_STATE_BLEND_INV_SRC_COLOR = BGFX_STATE_BLEND_INV_SRC_COLOR;
	// As, As, As, As
	const uint64 RS_STATE_BLEND_SRC_ALPHA = BGFX_STATE_BLEND_SRC_ALPHA;
	// 1-As, 1-As, 1-As, 1-As
	const uint64 RS_STATE_BLEND_INV_SRC_ALPHA = BGFX_STATE_BLEND_INV_SRC_ALPHA;
	// Ad, Ad, Ad, Ad
	const uint64 RS_STATE_BLEND_DST_ALPHA = BGFX_STATE_BLEND_DST_ALPHA;
	// 1-Ad, 1-Ad, 1-Ad ,1-Ad
	const uint64 RS_STATE_BLEND_INV_DST_ALPHA = BGFX_STATE_BLEND_INV_DST_ALPHA;
	// Rd, Gd, Bd, Ad
	const uint64 RS_STATE_BLEND_DST_COLOR = BGFX_STATE_BLEND_DST_COLOR;
	// 1-Rd, 1-Gd, 1-Bd, 1-Ad
	const uint64 RS_STATE_BLEND_INV_DST_COLOR = BGFX_STATE_BLEND_INV_DST_COLOR;
	// f, f, f, 1; f)
	const uint64 RS_STATE_BLEND_SRC_ALPHA_SAT = BGFX_STATE_BLEND_SRC_ALPHA_SAT;
	// Blend factor
	const uint64 RS_STATE_BLEND_FACTOR = BGFX_STATE_BLEND_FACTOR;
	// 1-Blend factor
	const uint64 RS_STATE_BLEND_INV_FACTOR = BGFX_STATE_BLEND_INV_FACTOR;

	// Blend add: src + dst.
	const uint64 RS_STATE_BLEND_EQUATION_ADD = BGFX_STATE_BLEND_EQUATION_ADD;
	// Blend subtract: src - dst.
	const uint64 RS_STATE_BLEND_EQUATION_SUB = BGFX_STATE_BLEND_EQUATION_SUB;
	// Blend reverse subtract: dst - src.
	const uint64 RS_STATE_BLEND_EQUATION_REVSUB = BGFX_STATE_BLEND_EQUATION_REVSUB;
	// Blend min: min(src, dst).
	const uint64 RS_STATE_BLEND_EQUATION_MIN = BGFX_STATE_BLEND_EQUATION_MIN;
	// Blend max: max(src, dst).
	const uint64 RS_STATE_BLEND_EQUATION_MAX = BGFX_STATE_BLEND_EQUATION_MAX;

	// Cull clockwise triangles.
	const uint64 RS_STATE_CULL_CW = BGFX_STATE_CULL_CW;
	// Cull counter-clockwise triangles.
	const uint64 RS_STATE_CULL_CCW = BGFX_STATE_CULL_CCW;

	// Tristrip.
	const uint64 RS_STATE_PT_TRISTRIP = BGFX_STATE_PT_TRISTRIP;
	// Lines.
	const uint64 RS_STATE_PT_LINES = BGFX_STATE_PT_LINES;
	// Line strip.
	const uint64 RS_STATE_PT_LINESTRIP = BGFX_STATE_PT_LINESTRIP;
	// Points.
	const uint64 RS_STATE_PT_POINTS = BGFX_STATE_PT_POINTS;

	// Enable MSAA rasterization.
	const uint64 RS_STATE_MSAA = BGFX_STATE_MSAA;
	// Enable line AA rasterization.
	const uint64 RS_STATE_LINEAA = BGFX_STATE_LINEAA;
	// Enable conservative rasterization.
	const uint64 RS_STATE_CONSERVATIVE_RASTER = BGFX_STATE_CONSERVATIVE_RASTER;
	// No state.
	const uint64 RS_STATE_NONE = BGFX_STATE_NONE;
	// Front counter-clockwise (default is clockwise).
	const uint64 RS_STATE_FRONT_CCW = BGFX_STATE_FRONT_CCW;
	// Enable blend independent.
	const uint64 RS_STATE_BLEND_INDEPENDENT = BGFX_STATE_BLEND_INDEPENDENT;
	// Enable alpha to coverage.
	const uint64 RS_STATE_BLEND_ALPHA_TO_COVERAGE = BGFX_STATE_BLEND_ALPHA_TO_COVERAGE;
	/// Default state is write to RGB, alpha, and depth with depth test less enabled, with clockwise
	/// culling and MSAA (when writing into MSAA frame buffer, otherwise this flag is ignored).
	const uint64 RS_STATE_DEFAULT = BGFX_STATE_DEFAULT;

	uint64 state_blend_func(uint64 src, uint64 dst) {
		return BGFX_STATE_BLEND_FUNC(src, dst);
	}
}    // namespace tt