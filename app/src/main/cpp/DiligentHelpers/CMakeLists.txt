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

target_link_libraries(
        DiligentHelpers
        TimeEngine
        DiligentAnyOpt
        Diligent-AssetLoader
        Diligent-BuildSettings
        Diligent-Common
        Diligent-GraphicsAccessories
        Diligent-GraphicsEngineOpenGL-shared
        Diligent-GraphicsEngineVk-shared
        Diligent-GraphicsTools
        Diligent-Imgui
        Diligent-ShaderTools
        Diligent-TargetPlatform
        Diligent-TextureLoader
)