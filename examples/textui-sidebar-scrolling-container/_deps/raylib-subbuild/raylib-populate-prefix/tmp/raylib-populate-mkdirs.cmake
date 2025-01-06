# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-src")
  file(MAKE_DIRECTORY "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-src")
endif()
file(MAKE_DIRECTORY
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-build"
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix"
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/phillip/Projects/clay/examples/textui-sidebar-scrolling-container/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
