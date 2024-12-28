# Defines:
#  CAIRO_FOUND        - System has Cairo
#  CAIRO_INCLUDE_DIRS - Cairo include directories
#  CAIRO_LIBRARY      - Cairo library
#  Cairo::Cairo       - Imported target

find_path(CAIRO_INCLUDE_DIRS
        NAMES cairo/cairo.h
        PATHS ${CAIRO_ROOT_DIR}
        PATH_SUFFIXES include
)

find_library(CAIRO_LIBRARY
        NAMES cairo
        PATHS ${CAIRO_ROOT_DIR}
        PATH_SUFFIXES lib lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cairo
        REQUIRED_VARS CAIRO_LIBRARY CAIRO_INCLUDE_DIRS
)

if(Cairo_FOUND AND NOT TARGET Cairo::Cairo)
    add_library(Cairo::Cairo UNKNOWN IMPORTED)
    set_target_properties(Cairo::Cairo PROPERTIES
            IMPORTED_LOCATION "${CAIRO_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${CAIRO_INCLUDE_DIRS}"
    )
endif()

mark_as_advanced(CAIRO_INCLUDE_DIRS CAIRO_LIBRARY)