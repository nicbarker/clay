
# to build this, install mingw

gcc image_demo.c -ggdb -oimgdemo -lgdi32 -lmingw32 -lole32 -lwindowscodecs # -mwindows # comment -mwindows out for console output