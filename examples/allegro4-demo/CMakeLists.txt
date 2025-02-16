cmake_minimum_required(VERSION 3.27)
project(allegro4_demo C)
set(CMAKE_C_STANDARD 99)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Fetch pre-built MinGW binaries
FetchContent_Declare(
  allegro4
  URL https://bitbucket.org/bugsquasher/unofficial-allegro-5-binaries/downloads/A443Deluxe.7z
)

FetchContent_MakeAvailable(allegro4)

link_directories(${allegro4_SOURCE_DIR}/lib/)

add_executable(allegro4_demo main.c ../../renderers/alleg4/alleg4.c)

target_compile_options(allegro4_demo PUBLIC)
target_include_directories(allegro4_demo PUBLIC . ${CMAKE_SOURCE_DIR} ${allegro4_SOURCE_DIR}/include/)

# All global variables Allegro makes available are lost when linking statically,
# at least not when extern-ing all over the place.
target_link_libraries(allegro4_demo PUBLIC alleg44.dll.a)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

add_custom_command(TARGET allegro4_demo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${allegro4_SOURCE_DIR}/bin/dlls/alleg44.dll"
    $<TARGET_FILE_DIR:allegro4_demo>
)

add_custom_command(
	TARGET allegro4_demo POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/res
		${CMAKE_CURRENT_BINARY_DIR}/res)
