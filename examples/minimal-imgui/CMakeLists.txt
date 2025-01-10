cmake_minimum_required(VERSION 3.27)
project(minimal_imgui)
set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#add_subdirectory(3rd_party)

include(FetchContent)

FetchContent_Declare(
  sokol
  GIT_REPOSITORY "https://github.com/floooh/sokol.git"
  GIT_COMMIT  "789d97071d17cbab4e3835a0b0b8b379e98c114f"
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG "v1.91.6"
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(sokol imgui)

add_executable(minimal_imgui
  main.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_compile_options(minimal_imgui PUBLIC)
target_include_directories(minimal_imgui PUBLIC .)

#set(CMAKE_CXX_FLAGS_DEBUG "-DCLAY_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
target_include_directories(minimal_imgui PUBLIC ${sokol_SOURCE_DIR} ${imgui_SOURCE_DIR})
if(WIN32)
  target_link_libraries(minimal_imgui PUBLIC kernel32 user32 shell32 gdi32)
elseif(APPLE)
  target_link_libraries(minimal_imgui PUBLIC "-framework Cocoa" "-framework QuartzCore" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
  target_link_libraries(minimal_imgui PUBLIC X11 Xi Xcursor GL dl pthread m)
endif()
