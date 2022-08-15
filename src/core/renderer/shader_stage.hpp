#if !defined(CORE_RENDERER_SHADER_STAGE_HPP__)
#	define CORE_RENDERER_SHADER_STAGE_HPP__

#	include <system_types.hpp>

namespace tt {
	enum class shader_stage {
		vertex,
		fragment,
		compute
	};
}

#endif    // CORE_RENDERER_SHADER_STAGE_HPP__