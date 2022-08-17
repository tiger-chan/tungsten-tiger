#if !defined(CORE_RENDERER_MATERIAL_HPP__)
#	define CORE_RENDERER_MATERIAL_HPP__

#	include <system_types.hpp>
#	include "components.hpp"

namespace tt {
	enum render_flags;
	struct material {
		entity shader;
		entity textures;

		uint textures_size;
		render_flags flags;
	};

	struct material_instance {
		entity mat;

		entity shader_program;
		entity textures;

		uint textures_size;
		render_flags flags;
	};
}    // namespace tt

#endif    // CORE_RENDERER_MATERIAL_HPP__
