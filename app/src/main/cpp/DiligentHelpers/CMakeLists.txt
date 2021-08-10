# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("DiligentHelpers")

add_library(PipelineManager PipelineManager.cpp)
target_link_libraries(PipelineManager DiligentAnyOpt WINAPI::Kernel)

add_library(DrawTools DrawTools.cpp)
target_link_libraries(DrawTools PipelineManager PixelToNDC)

add_library(RenderTarget RenderTarget.cpp)
target_link_libraries(RenderTarget DrawTools VertexEngine)

add_library(
        DiligentHelpers SHARED
        DiligentAppBase.cpp
)

target_compile_options(DiligentHelpers PRIVATE -DUNICODE -DENGINE_DLL)

if (DILIGENT_NO_VULKAN)
    set(VK_LIB "")
else()
    set(VK_LIB Diligent-GraphicsEngineVk-shared)
endif()

target_link_libraries(
        DiligentHelpers
        DiligentAnyOpt
        RenderTarget

#   DiligentCore
        Diligent-BuildSettings
        Diligent-Common
        Diligent-GraphicsAccessories
        Diligent-GraphicsEngineOpenGL-shared
        Diligent-GraphicsTools
        Diligent-ShaderTools
        Diligent-TargetPlatform
        ${VK_LIB}

#   DiligentTools
        Diligent-AssetLoader
        Diligent-Imgui
        Diligent-TextureLoader
)
