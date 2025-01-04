cmake_minimum_required(VERSION 3.10)

project(GLFWExample)

set(SRC main.c)

# Set libraries and extensions based on OS
if(WIN32)
    set(LIBS -lglfw3 -lgdi32 -lopengl32)
elseif(APPLE)
    set(LIBS -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo)
elseif(UNIX)
    set(LIBS -lglfw -lGL -lm -ldl -lpthread)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Add executable
add_executable(main${EXT} ${SRC})

# Link libraries
target_include_directories(main PRIVATE ../../../renderers/RSGL)
target_include_directories(main PRIVATE ../../../)
target_link_libraries(main${EXT} ${LIBS})