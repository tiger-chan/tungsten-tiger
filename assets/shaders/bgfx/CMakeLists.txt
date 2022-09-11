set(COMPILE_GLSL		ON CACHE BOOL "SHADERC GLSL Compilation of shaders")
set(COMPILE_HLSL		ON CACHE BOOL "SHADERC D3D11 Compilation of shaders (Windows Only)")
set(COMPILE_METAL		OFF CACHE BOOL "SHADERC METAL Compilation of shaders")
set(COMPILE_PSSL		OFF CACHE BOOL "SHADERC PSSL Compilation of shaders")
set(COMPILE_SPIRV		ON CACHE BOOL "SHADERC SPIRV Compilation of shaders")

set(BIN_SHADERS "")
set(SHADERS "")

set(SHADER_INTERMEDIATE_DIR	"${CMAKE_BINARY_DIR}/assets/shaders")
set(SHADER_OUTPUT_DIR	"${CMAKE_BINARY_DIR}/bin/assets/shaders")

set(GLSL_DIR	"glsl")
set(HLSL_DIR	"dx11")
set(METAL_DIR	"metal")
set(PSSL_DIR	"pssl")
set(SPIRV_DIR	"spirv")

macro(add_shader SHADER_PATH FILENAME)
	set(SHADER "${SHADER_PATH}/${FILENAME}")
	list(APPEND SHADERS "${SHADER}")
	set(SHADERS ${SHADERS} PARENT_SCOPE)
endmacro()


add_subdirectory(line)
add_subdirectory(ul_texture)

macro(add_custom_shader COMPILER SHADER SHADER_FILE SHADER_FLAG SHADER_TYPE INT_FILE)
	get_filename_component(INT_OUT_DIR ${INT_FILE} DIRECTORY)
	get_filename_component(BIN_OUT_DIR ${SHADER_FILE} DIRECTORY)
	add_custom_command(
		OUTPUT ${SHADER_FILE}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${INT_OUT_DIR}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_OUT_DIR}
		COMMAND "${SHADERC}"
			"${SHADER_FLAGS}"
			--depends
			--disasm
			-i "${SHADERC_SRC_DIR}"
			--type ${SHADER_TYPE}
			-o "${INT_FILE}"
			-f "${SHADER}"
		COMMAND ${CMAKE_COMMAND} -E copy "${INT_FILE}" "${SHADER_FILE}"
		DEPENDS ${SHADER}
		COMMENT "[SHADERC ${COMPILER}]: Compiling ${SHADER}"
		COMMAND_EXPAND_LISTS
	)
endmacro()

if (COMPILE_GLSL)
	foreach(SHADER ${SHADERS})
		get_filename_component(TYPE ${SHADER} LAST_EXT)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)
		
		set(SHADER_TYPE "")
		if("${TYPE}" STREQUAL ".vert")
			set(SHADER_TYPE "vertex")
		elseif("${TYPE}" STREQUAL ".frag")
			set(SHADER_TYPE "fragment")
		elseif("${TYPE}" STREQUAL ".comp")
			set(SHADER_TYPE "compute")
		endif()

		set(INT_OUT "${SHADER_INTERMEDIATE_DIR}/${GLSL_DIR}")
		set(BIN_OUT "${SHADER_OUTPUT_DIR}/${GLSL_DIR}")

		set(SHADER_INT_FILE "${INT_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FILE "${BIN_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FLAGS --platform linux -p 440)
		add_custom_shader("GLSL" "${SHADER}" "${SHADER_FILE}" "${SHADER_FLAGS}" "${SHADER_TYPE}" "${SHADER_INT_FILE}")

		list(APPEND BIN_SHADERS "${SHADER_FILE}")
	endforeach()
endif()

if (COMPILE_HLSL)
	foreach(SHADER ${SHADERS})
		get_filename_component(TYPE ${SHADER} LAST_EXT)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)
		
		set(SHADER_TYPE "")
		set(SHADER_FLAGS --platform windows)
		if("${TYPE}" STREQUAL ".vert")
			set(SHADER_TYPE "vertex")
			list(APPEND SHADER_FLAGS vs_5_0 -O 3)
		elseif("${TYPE}" STREQUAL ".frag")
			set(SHADER_TYPE "fragment")
			list(APPEND SHADER_FLAGS ps_5_0 -O 3)
		elseif("${TYPE}" STREQUAL ".comp")
			set(SHADER_TYPE "compute")
			list(APPEND SHADER_FLAGS cs_5_0 -O 1)
		endif()


		set(INT_OUT "${SHADER_INTERMEDIATE_DIR}/${HLSL_DIR}")
		set(BIN_OUT "${SHADER_OUTPUT_DIR}/${HLSL_DIR}")

		set(SHADER_INT_FILE "${INT_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FILE "${BIN_OUT}/${SHADER_NAME}.bin")

		add_custom_shader("HLSL" "${SHADER}" "${SHADER_FILE}" "${SHADER_FLAGS}" "${SHADER_TYPE}" "${SHADER_INT_FILE}")

		list(APPEND BIN_SHADERS "${SHADER_FILE}")
	endforeach()
endif()

if (COMPILE_METAL)
	foreach(SHADER ${SHADERS})
		get_filename_component(TYPE ${SHADER} LAST_EXT)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)
		
		set(SHADER_TYPE "")
		if("${TYPE}" STREQUAL ".vert")
			set(SHADER_TYPE "vertex")
		elseif("${TYPE}" STREQUAL ".frag")
			set(SHADER_TYPE "fragment")
		elseif("${TYPE}" STREQUAL ".comp")
			set(SHADER_TYPE "compute")
		endif()


		set(INT_OUT "${SHADER_INTERMEDIATE_DIR}/${METAL_DIR}")
		set(BIN_OUT "${SHADER_OUTPUT_DIR}/${METAL_DIR}")

		set(SHADER_INT_FILE "${INT_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FILE "${BIN_OUT}/${SHADER_NAME}.bin")

		set(SHADER_FLAGS --platform osx -p metal)
		add_custom_shader("METAL" "${SHADER}" "${SHADER_FILE}" "${SHADER_FLAGS}" "${SHADER_TYPE}" "${SHADER_INT_FILE}")
		list(APPEND BIN_SHADERS "${SHADER_FILE}")
	endforeach()
endif()

if (COMPILE_PSSL)
	foreach(SHADER ${SHADERS})
		get_filename_component(TYPE ${SHADER} LAST_EXT)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)
		
		set(SHADER_TYPE "")
		if("${TYPE}" STREQUAL ".vert")
			set(SHADER_TYPE "vertex")
		elseif("${TYPE}" STREQUAL ".frag")
			set(SHADER_TYPE "fragment")
		elseif("${TYPE}" STREQUAL ".comp")
			set(SHADER_TYPE "compute")
		endif()


		set(INT_OUT "${SHADER_INTERMEDIATE_DIR}/${PSSL_DIR}")
		set(BIN_OUT "${SHADER_OUTPUT_DIR}/${PSSL_DIR}")

		set(SHADER_INT_FILE "${INT_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FILE "${BIN_OUT}/${SHADER_NAME}.bin")

		set(SHADER_FLAGS --platform orbis -p pssl)
		add_custom_shader("PSSL" "${SHADER}" "${SHADER_FILE}" "${SHADER_FLAGS}" "${SHADER_TYPE}" "${SHADER_INT_FILE}")
		list(APPEND BIN_SHADERS "${SHADER_FILE}")
	endforeach()
endif()

if (COMPILE_SPIRV)
	foreach(SHADER ${SHADERS})
		get_filename_component(TYPE ${SHADER} LAST_EXT)
		get_filename_component(SHADER_NAME ${SHADER} NAME)
		get_filename_component(SHADER_DIR ${SHADER} DIRECTORY)
		
		set(SHADER_TYPE "")
		if("${TYPE}" STREQUAL ".vert")
			set(SHADER_TYPE "vertex")
		elseif("${TYPE}" STREQUAL ".frag")
			set(SHADER_TYPE "fragment")
		elseif("${TYPE}" STREQUAL ".comp")
			set(SHADER_TYPE "compute")
		endif()

		set(INT_OUT "${SHADER_INTERMEDIATE_DIR}/${SPIRV_DIR}")
		set(BIN_OUT "${SHADER_OUTPUT_DIR}/${SPIRV_DIR}")

		set(SHADER_INT_FILE "${INT_OUT}/${SHADER_NAME}.bin")
		set(SHADER_FILE "${BIN_OUT}/${SHADER_NAME}.bin")

		set(SHADER_FLAGS --platform linux -p spirv)
		add_custom_shader("SPIRV" "${SHADER}" "${SHADER_FILE}" "${SHADER_FLAGS}" "${SHADER_TYPE}" "${SHADER_INT_FILE}")
		list(APPEND BIN_SHADERS "${SHADER_FILE}")
	endforeach()
endif()

add_custom_target(CompileShaders ALL
	DEPENDS ${BIN_SHADERS}
	)

add_dependencies(${PROJECT_NAME} CompileShaders)
