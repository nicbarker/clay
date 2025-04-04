cmake_minimum_required(VERSION 3.27)
project(sokol_corner_radius C)

add_executable(sokol_corner_radius main.c)
target_link_libraries(sokol_corner_radius PUBLIC sokol)
