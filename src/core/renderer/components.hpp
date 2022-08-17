#if !defined(CORE_RENDERER_COMPONENTS_HPP__)
#	define CORE_RENDERER_COMPONENTS_HPP__

#	include "system_types.hpp"

#	include <glm/glm.hpp>

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

	struct cuboid {
		double height;
		double width;
		double depth;
	};

	struct font {
	};

	enum class entity {};

	static constexpr const uint default_color = 0xff'ff'ff'ff;
	static constexpr const font default_font = {};
}    // namespace tt

#endif    // CORE_RENDERER_COMPONENTS_HPP__
