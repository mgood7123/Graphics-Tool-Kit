# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(ImGuiViews)

add_library(
        ImGuiViews SHARED
        ImGuiView.cpp
        TextView.cpp
)

target_link_libraries(
        ImGuiViews
        View
)
