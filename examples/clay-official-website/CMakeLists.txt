cmake_minimum_required(VERSION 3.28)
project(clay_official_website C)

set(CMAKE_C_STANDARD 99)

add_executable(clay_official_website main.c)

if(!MSVC)
target_compile_options(clay_official_website PUBLIC -Wall -Werror -Wno-unknown-pragmas)
endif()
target_include_directories(clay_official_website PUBLIC .)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")