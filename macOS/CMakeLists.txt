# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(macOS)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../app/src/main/cpp ${CMAKE_CURRENT_BINARY_DIR}/GRAPHICS)

add_executable(app dummy.cpp)

target_link_libraries(app native-lib)
copyExe(app)