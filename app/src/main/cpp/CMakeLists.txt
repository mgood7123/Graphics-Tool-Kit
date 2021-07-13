# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("kit")


include_directories(
        DiligentEngine
        DiligentEngine/DiligentCore/Primitives/interface
)

include_directories(WINAPI_KERNEL)

add_subdirectory(DiligentEngine/DiligentCore ${CMAKE_CURRENT_BINARY_DIR}/DiligentCore)
add_subdirectory(DiligentEngine/DiligentFX ${CMAKE_CURRENT_BINARY_DIR}/DiligentFX)
add_subdirectory(DiligentEngine/DiligentTools ${CMAKE_CURRENT_BINARY_DIR}/DiligentTools)

add_subdirectory(TimeEngine)
include_directories(TimeEngine)

add_subdirectory(DiligentHelpers)
include_directories(DiligentHelpers)

add_subdirectory(AndroidBase)
include_directories(AndroidBase)

add_subdirectory(WINAPI_KERNEL)

add_subdirectory(Objects)
include_directories(Objects)

add_library(
        native-lib SHARED
        native-lib.cpp
        AppInstance.cpp
)

target_link_libraries(
        native-lib
        AndroidBase
        DiligentHelpers
        TimeEngine
        Objects
        WINAPI::KERNEL
)