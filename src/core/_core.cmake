target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/string_algo.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/string_fixed.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/string_util.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/string_view.hpp"
			"${CMAKE_CURRENT_LIST_DIR}/string.cpp"
	)

include(${CMAKE_CURRENT_LIST_DIR}/renderer/_renderer.cmake)