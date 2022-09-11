cmake_minimum_required(VERSION 3.15)
project(TungstenTiger CXX)


add_executable(${PROJECT_NAME} "src/tungsten_tiger.cpp" src/main.cpp)
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
)

# Suppress the Debug/Release subdirectory for the build
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<0:>")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<0:>")

# Needed to ensure BGFX can correctly identify which version of the cpp standard is being used
target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>: /Zc:__cplusplus>)


find_package(bgfx REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(EnTT REQUIRED)
find_package(constr REQUIRED)
find_package(mime REQUIRED)

# Set the assets path to "/assets" or "/../Resources/assets" on macOS
if (APPLE)
	set(ASSETS_PATH "$<TARGET_FILE_DIR:TungstenTiger>/../Resources/assets")
else ()
	set(ASSETS_PATH "$<TARGET_FILE_DIR:TungstenTiger>/assets")
endif ()

# Begin Ultralight include
# TODO Replace this method of inclusion with Conan import when available
set(ULTRALIGHT_SDK "${CMAKE_CURRENT_LIST_DIR}/libs/ultralight")
set(ULTRALIGHT_INCLUDE_DIRS "${ULTRALIGHT_SDK}/include/")
set(ULTRALIGHT_LINK_DIRS "${ULTRALIGHT_SDK}/lib/")
set(ULTRALIGHT_BIN_DIRS "${ULTRALIGHT_SDK}/bin/")
set(ULTRALIGHT_RESOURCES_DIR "${ULTRALIGHT_SDK}/bin/resources")

include_directories("${ULTRALIGHT_INCLUDE_DIRS}")

find_library(
    ULTRA_LIB_CORE
    NAMES UltralightCore
    HINTS ${ULTRALIGHT_LINK_DIRS})

find_library(
    ULTRA_LIB_APP_CORE
    NAMES AppCore
    HINTS ${ULTRALIGHT_LINK_DIRS})

find_library(
    ULTRA_LIB
    NAMES Ultralight
    HINTS ${ULTRALIGHT_LINK_DIRS})

find_library(
    ULTRA_LIB_WEB_CORE
    NAMES WebCore
    HINTS ${ULTRALIGHT_LINK_DIRS})

# Copy all binaries to target directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_BIN_DIRS}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Copy resources to assets directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_RESOURCES_DIR}" "${ASSETS_PATH}/resources")

target_link_libraries(${PROJECT_NAME}
    ${ULTRA_LIB_CORE}
	${ULTRA_LIB_APP_CORE}
	${ULTRA_LIB}
	${ULTRA_LIB_WEB_CORE}
)

# End Ultralight include

add_subdirectory(assets)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/assets/resources" "${ASSETS_PATH}/resources")
  
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} bgfx::bgfx)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} constr::constr)
target_link_libraries(${PROJECT_NAME} mime::mime)

target_include_directories(${PROJECT_NAME} PUBLIC src)
add_subdirectory(src)

install(TARGETS ${PROJECT_NAME})

