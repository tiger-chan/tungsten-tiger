cmake_minimum_required(VERSION 3.15)
project(TungstenTiger CXX)

add_executable(TungstenTiger "src/tungsten_tiger.cpp" src/main.cpp)
target_include_directories(TungstenTiger PUBLIC include)

find_package(SDL2 REQUIRED)

target_link_libraries(TungstenTiger SDL2::SDL2)

install(TARGETS TungstenTiger)

