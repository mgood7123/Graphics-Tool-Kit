# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(Objects)

add_library(
        ObjectBase SHARED
        ObjectBase.cpp
)

target_link_libraries(
        ObjectBase
        DiligentHelpers
        TimeEngine
        VertexEngine
        MultiTouch
)

add_library(
        ObjectGroup SHARED
        ObjectGroup.cpp
)

target_link_libraries(
        ObjectGroup
        ObjectBase
)

add_subdirectory(Painters)

add_library(
        Objects SHARED
        Triangle.cpp
        Cube.cpp
        CubeOnTriangle.cpp
)

target_link_libraries(
        Objects
        ObjectBase
        ObjectGroup
        Painters
)
