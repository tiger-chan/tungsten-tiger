cmake_minimum_required(VERSION 3.15)
project(TungstenTiger CXX)

add_executable(TungstenTiger "src/tungsten_tiger.cpp" src/main.cpp)

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED)

target_link_libraries(TungstenTiger SDL2::SDL2)
target_link_libraries(TungstenTiger bgfx::bgfx)

target_include_directories(TungstenTiger PUBLIC src)
include(src/_src.cmake)

install(TARGETS TungstenTiger)

