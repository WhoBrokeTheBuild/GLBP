
ADD_EXECUTABLE(
    triangle
    src/Main.cpp
)

TARGET_LINK_LIBRARIES(
    triangle
    ${_ENGINE}
)