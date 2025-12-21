cmake_minimum_required(VERSION 4.2)
project(GeometryDashSDL C)

set(CMAKE_C_STANDARD 11)

add_executable(GeometryDashSDL main.c)

find_package(SDL2 REQUIRED)
target_link_libraries(GeometryDashSDL ${SDL2_LIBRARIES} m)