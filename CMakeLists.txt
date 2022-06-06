cmake_minimum_required(VERSION 3.15)
project(TungstenTiger CXX)

add_executable(TungstenTiger "src/tungsten_tiger.cpp" src/main.cpp)

find_package(bgfx REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)

target_link_libraries(TungstenTiger glm::glm)
target_link_libraries(TungstenTiger bgfx::bgfx)
target_link_libraries(TungstenTiger SDL2::SDL2)

target_include_directories(TungstenTiger PUBLIC src)
include(src/_src.cmake)

install(TARGETS TungstenTiger)

