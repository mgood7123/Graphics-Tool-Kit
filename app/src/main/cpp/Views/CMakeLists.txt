# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(Views)

add_library(
        View SHARED
        View.cpp
        ViewGroup.cpp
)

target_link_libraries(
        View
        DiligentHelpers
        TimeEngine
        VertexEngine
        MultiTouch
)

add_subdirectory(ImGuiViews)
add_subdirectory(PainterViews)

add_library(
        Views SHARED
        CubeView.cpp
        GridView.cpp
        RootView.cpp
        TriangleView.cpp
        DebugView.cpp
        LinearLayout.cpp
)

target_link_libraries(
        Views
        View
        ImGuiViews
        PainterViews
)

set_target_properties(
        Views PROPERTIES
        FRAMEWORK TRUE
        XCODE_EMBED_FRAMEWORKS "View;ImGuiViews;PainterViews"
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)
