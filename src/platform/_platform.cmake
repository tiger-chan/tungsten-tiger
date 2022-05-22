if (WIN32)
	target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/windows.cpp"
	)
elseif(UNIX)
	target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/linux.cpp"
	)
endif()

target_sources(TungstenTiger
		PUBLIC
			"${CMAKE_CURRENT_LIST_DIR}/sdl_renderer.cpp"
	)