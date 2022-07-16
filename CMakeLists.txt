cmake_minimum_required(VERSION 3.15)
project(TungstenTiger CXX)

add_executable(TungstenTiger "src/tungsten_tiger.cpp" src/main.cpp)



find_package(bgfx REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

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
add_custom_command(TARGET TungstenTiger POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_BIN_DIRS}" $<TARGET_FILE_DIR:TungstenTiger>)

# Set the assets path to "/assets" or "/../Resources/assets" on macOS
if (APPLE)
	set(ASSETS_PATH "$<TARGET_FILE_DIR:TungstenTiger>/../Resources/assets") 
else ()
	set(ASSETS_PATH "$<TARGET_FILE_DIR:TungstenTiger>/assets") 
endif ()

# Copy resources to assets directory
add_custom_command(TARGET TungstenTiger POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_RESOURCES_DIR}" "${ASSETS_PATH}/resources")

target_link_libraries(TungstenTiger
    ${ULTRA_LIB_CORE}
	${ULTRA_LIB_APP_CORE}
	${ULTRA_LIB}
	${ULTRA_LIB_WEB_CORE}
)

# End Ultralight include

target_link_libraries(TungstenTiger glm::glm)
target_link_libraries(TungstenTiger bgfx::bgfx)
target_link_libraries(TungstenTiger SDL2::SDL2)

target_include_directories(TungstenTiger PUBLIC src)
include(src/_src.cmake)

install(TARGETS TungstenTiger)

