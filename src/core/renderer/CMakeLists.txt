target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/components.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/material.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/draw_2d.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/renderer_3d.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/renderer_state.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/shader.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/texture_flags.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/texture_format.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/texture.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/uniform_type.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/vertex_buffer_flags.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/vertex_layout.cpp"
	)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/ul)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/memory)
