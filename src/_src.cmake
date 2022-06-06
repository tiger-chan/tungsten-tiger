target_sources(TungstenTiger
	PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/sem_ver.cpp"
)

target_include_directories(TungstenTiger PUBLIC .)

include(${CMAKE_CURRENT_LIST_DIR}/core/_core.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/platform/_platform.cmake)
