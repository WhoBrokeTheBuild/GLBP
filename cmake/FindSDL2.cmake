# FindSDL2.cmake
#
# Finds the Simple DirectMedia Library
#
# This will define the following variables
#
#   SDL2_FOUND
#   SDL2_INCLUDE_DIR
#   SDL2_LIBRARIES
#   SDL2_RUNTIME_DIR
#
# and the following imported targets
#
#   SDL2::SDL2
#
# The following variables can be set as arguments
#
#   SDL2_ROOT_DIR
#

FIND_PACKAGE(PkgConfig QUIET)
PKG_CHECK_MODULES(_SDL2_PC QUIET sdl2)

SET(SDL2_INCLUDE_DIR ${_SDL2_PC_INCLUDE_DIRS})
FIND_PATH(
    SDL2_INCLUDE_DIR
    NAMES SDL.h
    PATHS ${SDL2_ROOT_DIR}
    PATH_SUFFIXES include/SDL2
)

FIND_PATH(
    SDL2_RUNTIME_DIR
    NAMES SDL2.dll
    PATHS ${SDL2_ROOT_DIR} ${_SDL_PC_LIBDIR} ${_SDL2_PC_LIBRARY_DIRS} 
    PATH_SUFFIXES bin
)

SET(SDL2_LIBRARY ${_SDL2_PC_LIBRARIES})
FIND_LIBRARY(
    SDL2_LIBRARY
    NAMES SDL2
    PATHS ${SDL2_ROOT_DIR}
    PATH_SUFFIXES lib
)

FIND_LIBRARY(
    SDL2_MAIN_LIBRARY
    NAMES SDL2main
    PATHS ${SDL2_ROOT_DIR}
    PATH_SUFFIXES lib
)

IF(SDL2_MAIN_LIBRARY AND SDL2_LIBRARY)
    LIST(APPEND SDL2_LIBRARY "${SDL2_MAIN_LIBRARY}")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    SDL2
    REQUIRED_VARS 
        SDL2_INCLUDE_DIR
        SDL2_LIBRARY 
)

ADD_LIBRARY(SDL2::SDL2 INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    SDL2::SDL2 PROPERTIES
    INTERFACE_LINK_LIBRARIES "${SDL2_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
)
