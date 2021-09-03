# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(DiligentLibrary)

add_library(DiligentLibrary SHARED empty.cpp)

target_compile_options(DiligentLibrary PRIVATE -DUNICODE -DENGINE_DLL)

if (DILIGENT_NO_VULKAN)
    set(VK_LIB "")
else()
    set(VK_LIB Diligent-GraphicsEngineVk-shared)
endif()

target_link_libraries(
        DiligentLibrary
        Diligent-BuildSettings
        Diligent-Common
        Diligent-GraphicsAccessories
        Diligent-GraphicsTools
        Diligent-ShaderTools
        Diligent-TargetPlatform
        Diligent-GraphicsEngineOpenGL-shared
        ${VK_LIB}
        Diligent-AssetLoader
        Diligent-Imgui
        Diligent-TextureLoader
        Diligent-Log
)
