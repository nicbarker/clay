cmake_minimum_required(VERSION 3.27)
project(clay-ncurses-example C)


# Find ncursesw explicitly
find_library(NCURSESW_LIB NAMES ncursesw REQUIRED)
find_path(NCURSESW_INCLUDE_DIR NAMES ncurses.h PATH_SUFFIXES ncursesw)

add_compile_definitions(_XOPEN_SOURCE_EXTENDED _XOPEN_SOURCE=700)

add_executable(clay-ncurses-example main.c)

target_link_libraries(clay-ncurses-example PRIVATE ${NCURSESW_LIB})

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_link_libraries(clay-ncurses-example PRIVATE m)
endif()

target_include_directories(clay-ncurses-example PRIVATE ${NCURSESW_INCLUDE_DIR} ../../)
