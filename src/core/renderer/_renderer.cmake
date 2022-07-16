target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/material.cpp"
			"${CMAKE_CURRENT_LIST_DIR}/renderer_2d.hpp"
	)

include(${CMAKE_CURRENT_LIST_DIR}/ul/_ul.cmake)
