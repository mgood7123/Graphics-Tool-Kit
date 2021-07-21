# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("DiligentHelpers")

add_library(DiligentAnyOpt AnyOpt.cpp)
target_compile_options(DiligentAnyOpt PRIVATE -DUNICODE -DENGINE_DLL)
target_link_libraries(DiligentAnyOpt Diligent-Common)

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