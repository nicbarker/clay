cmake_minimum_required(VERSION 3.27)
project(clay_examples_cpp_project_example CXX)

set(CMAKE_CXX_STANDARD 20)
if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
endif()

add_executable(clay_examples_cpp_project_example main.cpp)

target_include_directories(clay_examples_cpp_project_example PUBLIC .)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()
