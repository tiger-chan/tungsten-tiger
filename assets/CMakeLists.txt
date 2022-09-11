set(UPPER_BUILD_TYPE ${CMAKE_BUILD_TYPE})
string(TOUPPER ${UPPER_BUILD_TYPE} UPPER_BUILD_TYPE)

set(BGFX_DIR ${bgfx_BUILD_DIRS_${UPPER_BUILD_TYPE}})
set(SHADERC_DIR "${BGFX_DIR}bin/")
set(SHADERC "${SHADERC_DIR}shaderc")
set(SHADERC_LIB_DIR "${BGFX_DIR}includes/")
set(SHADERC_SRC_DIR "${BGFX_DIR}src/")

add_subdirectory(shaders)

# add_custom_command(
# 	TARGET ${PROJECT_NAME}
# 	PRE_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E echo "echoed ${SHADERC_DIR}"
# 	COMMENT "Custome Command")