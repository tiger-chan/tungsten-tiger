#include "system_types.hpp"
#include <glm/glm.hpp>

namespace tt {
	struct position {
	};

	struct bounds {
	};

	struct visible {
	};

	struct color {
	};

	struct camera {
	};

	struct texture {
	};

	using uvs = glm::vec2;

	struct mesh;

	using vec2 = glm::vec2;

	struct rect {
		double height;
		double width;
	};

	struct circle {
		double radius;
	};

	struct box {
		double height;
		double width;
		double depth;
	};

	struct font {
	};

	struct entity {};

	static constexpr const uint default_color = 0xff'ff'ff'ff;
	static constexpr const font default_font = {};
}