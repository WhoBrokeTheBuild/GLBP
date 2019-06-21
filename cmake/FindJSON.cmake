# FindJSON.cmake
#
# Finds the nlohmann/json library
#
# This will define the following variables
#
#   JSON_FOUND
#   JSON_INCLUDE_DIR
#
# and the following imported targets
#
#   JSON::JSON
#
# The following variables can be set as arguments
#
#   JSON_ROOT_DIR
#

FIND_PACKAGE(nlohmann_json QUIET)

IF(NOT JSON_INCLUDE_DIR)
    FIND_PATH(
        JSON_INCLUDE_DIR
        NAMES nlohmann/json.hpp
        PATHS ${JSON_ROOT_DIR}
        PATH_SUFFIXES include
    )
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    JSON
    REQUIRED_VARS JSON_INCLUDE_DIR
)

ADD_LIBRARY(JSON::JSON INTERFACE IMPORTED)
SET_TARGET_PROPERTIES(
    JSON::JSON PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${JSON_INCLUDE_DIR}"
)