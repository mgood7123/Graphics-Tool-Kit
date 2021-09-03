# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("DiligentHelpers")

add_library(PipelineManager SHARED PipelineManager.cpp)
target_link_libraries(PipelineManager DiligentAnyOpt WINAPI::Kernel)

add_library(DrawTools SHARED DrawTools.cpp)
target_link_libraries(DrawTools PipelineManager PixelToNDC)

set_target_properties(
        DrawTools PROPERTIES
        FRAMEWORK TRUE
        XCODE_EMBED_FRAMEWORKS PipelineManager
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)

add_library(RenderTarget SHARED RenderTarget.cpp)
target_link_libraries(RenderTarget DrawTools VertexEngine)

set_target_properties(
        RenderTarget PROPERTIES
        FRAMEWORK TRUE
        XCODE_EMBED_FRAMEWORKS DrawTools
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)

add_library(
        DiligentHelpers SHARED
        DiligentAppBase.cpp
)

target_link_libraries(
        DiligentHelpers
        DiligentAnyOpt
        RenderTarget
        DiligentLibrary
)

set_target_properties(
        DiligentHelpers PROPERTIES
        FRAMEWORK TRUE
        XCODE_EMBED_FRAMEWORKS RenderTarget
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)
