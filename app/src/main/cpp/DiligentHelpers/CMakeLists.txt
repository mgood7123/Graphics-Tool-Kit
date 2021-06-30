# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("DiligentHelper")

add_subdirectory(../DiligentEngine/DiligentCore ${CMAKE_CURRENT_BINARY_DIR}/DiligentCore)
add_subdirectory(../DiligentEngine/DiligentFX ${CMAKE_CURRENT_BINARY_DIR}/DiligentFX)
add_subdirectory(../DiligentEngine/DiligentTools ${CMAKE_CURRENT_BINARY_DIR}/DiligentTools)

add_library(
        DiligentHelper SHARED

        DiligentAppBase.cpp
)

target_compile_options(DiligentHelper PRIVATE -DUNICODE -DENGINE_DLL)

target_link_libraries(DiligentHelper
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