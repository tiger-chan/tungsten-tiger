target_sources(TungstenTiger
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/sem_ver.cpp"
)

target_include_directories(TungstenTiger PUBLIC .)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/core)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/platform)
