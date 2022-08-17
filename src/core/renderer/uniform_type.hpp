#if !defined(CORE_RENDERER_UNIFORM_FORMAT_HPP__)
#	define CORE_RENDERER_UNIFORM_FORMAT_HPP__

#	include <system_types.hpp>

namespace tt {
	enum uniform_type {
		// Sampler.
		UT_SAMPLER,
		// Reserved, do not use.
		UT_END,

		// 4 floats vector.
		UT_VEC4,
		// 3x3 matrix.
		UT_MAT3,
		// 4x4 matrix.
		UT_MAT4,
	};
}

#endif    // CORE_RENDERER_UNIFORM_FORMAT_HPP__