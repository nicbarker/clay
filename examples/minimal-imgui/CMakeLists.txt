cmake_minimum_required(VERSION 3.27)
project(minimal_imgui)
set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(3rd_party)

add_executable(minimal_imgui main.cpp)

target_compile_options(minimal_imgui PUBLIC)
target_include_directories(minimal_imgui PUBLIC .)

target_link_libraries(minimal_imgui PUBLIC sokol imgui)
#set(CMAKE_CXX_FLAGS_DEBUG "-DCLAY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
if(WIN32)
  target_link_libraries(minimal_imgui PUBLIC kernel32 user32 shell32 gdi32)
endif()
