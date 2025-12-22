cmake_minimum_required(VERSION 4.2)
project(GeometryDashSDL C)
set(CMAKE_C_STANDARD 11)
add_executable(GeometryDashSDL main.c)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
target_link_libraries(GeometryDashSDL ${SDL2_LIBRARIES} m)

#target_link_libraries(GeometryDashSDL
#        SDL2::SDL2
#SDL2_mixer::SDL2_mixer
#)

