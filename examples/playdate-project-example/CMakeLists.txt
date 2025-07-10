cmake_minimum_required(VERSION 3.27)
set(CMAKE_C_STANDARD 99)

set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
        # Convert path from Windows
        file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
        execute_process(
                        COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
                        COMMAND head -n 1
                        COMMAND cut -c9-
                        OUTPUT_VARIABLE SDK
                        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
endif()

if (NOT EXISTS ${SDK})
        message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
        return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME clay_playdate_example)
set(PLAYDATE_GAME_DEVICE clay_playdate_example_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)

if (TOOLCHAIN STREQUAL "armgcc")
        add_executable(${PLAYDATE_GAME_DEVICE} main.c)
else()
        add_library(${PLAYDATE_GAME_NAME} SHARED main.c)
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)

