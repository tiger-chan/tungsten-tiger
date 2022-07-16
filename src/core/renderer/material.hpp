#include <system_types.hpp>
#include "components.hpp"

namespace tt {
	struct shader;
	struct texture;
	struct font;
	enum render_flags;
	struct material {
		byte *uniforms;
		shader *shader;
		texture *textures;

		uint uniforms_size;
		uint textures_size;
		render_flags flags;
	};

	struct material_instance {
		material *mat;

		byte *uniforms;
		shader *shader;
		texture *textures;

		uint uniforms_size;
		uint textures_size;
		render_flags flags;
	};
}    // namespace tt
