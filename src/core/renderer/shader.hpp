#if !defined(CORE_RENDERER_SHADER_HPP__)
#	define CORE_RENDERER_SHADER_HPP__

#	include <system_types.hpp>
#	include <core/string.hpp>
#	include "components.hpp"

namespace tt {
	enum class shader_stage {
		vertex,
		geo,
		fragment,
		compute,
	};

	struct shader {
		shader_stage stage;
		byte *data{ nullptr };
		uint32 data_len{ 0 };
	};

	struct shader_mapping {
		// need to have some form of interface between the structure and the buffer mappings
	};

	struct shader_program {
		entity stages[4]{};
		tt::string name;
	};
}    // namespace tt

#endif    // CORE_RENDERER_SHADER_HPP__
