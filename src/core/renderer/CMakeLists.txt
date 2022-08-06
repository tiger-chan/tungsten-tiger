target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/components.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/material.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/renderer_2d.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/renderer_3d.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/shader.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/texture.hpp"
	)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ul)
