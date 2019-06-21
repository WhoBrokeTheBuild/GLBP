CMAKE_MINIMUM_REQUIRED(VERSION 3.8 FATAL_ERROR)

PROJECT(GLBP VERSION 0.0.1)

###
### Configuration
###

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake")

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

###
### Dependencies
###

FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/depend")

SET(GLM_ROOT_DIR    "${CMAKE_BINARY_DIR}/depend/glm")
SET(JSON_ROOT_DIR   "${CMAKE_BINARY_DIR}/depend/json")
SET(SDL2_ROOT_DIR   "${CMAKE_BINARY_DIR}/depend/SDL2")

EXECUTE_PROCESS(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" "${CMAKE_SOURCE_DIR}/depend"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/depend"
    RESULT_VARIABLE result
)
IF(result)
    MESSAGE("${result}")
    MESSAGE(FATAL_ERROR "Failed to configure depend projects")
ENDIF()

SET(BUILD_COMMAND ${CMAKE_COMMAND} --build . )
IF("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
    SET(BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
ENDIF()

EXECUTE_PROCESS(
    COMMAND ${BUILD_COMMAND}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/depend"
    RESULT_VARIABLE result
)

IF(result)
    MESSAGE("${result}")
    MESSAGE(FATAL_ERROR "Failed to build depend projects")
ENDIF()

FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(JSON REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

###
### Library
###

SET(_ENGINE "GLBP")

ADD_LIBRARY(${_ENGINE} SHARED "")

FILE(GLOB_RECURSE 
    _ENGINE_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

FILE(GLOB_RECURSE 
    _ENGINE_SOURCES  
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

TARGET_SOURCES(
    ${_ENGINE}
    PUBLIC
        ${_ENGINE_INCLUDES}
    PRIVATE
        ${_ENGINE_SOURCES}
)

TARGET_INCLUDE_DIRECTORIES(
    ${_ENGINE}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

TARGET_COMPILE_FEATURES(
    ${_ENGINE} 
    PUBLIC
        cxx_std_17
)

SET_TARGET_PROPERTIES(
    ${_ENGINE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

TARGET_COMPILE_OPTIONS(
    ${_ENGINE} 
    PRIVATE
        # Disable unknown pragmas warning, C++ exceptions
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wno-unknown-pragmas -fno-exceptions>
        $<$<CXX_COMPILER_ID:MSVC>:/MP /wd4068 /EHsc->
)

TARGET_COMPILE_DEFINITIONS(
    ${_ENGINE} 
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
)

TARGET_LINK_LIBRARIES(
    ${_ENGINE}
    GLM::GLM
    JSON::JSON
    SDL2::SDL2
    Threads::Threads
)

###
### Example Projects
###

ADD_SUBDIRECTORY(examples)