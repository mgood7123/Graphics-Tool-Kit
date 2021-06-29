# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("kit")

add_subdirectory(DiligentEngine/DiligentCore)
add_subdirectory(DiligentEngine/DiligentFX)
add_subdirectory(DiligentEngine/DiligentTools)

include_directories(android)
include_directories(helpers)
include_directories(DiligentEngine/DiligentCore/Graphics/GraphicsEngine/include)

add_library(
        native-lib SHARED

        # android
        android/native-lib.cpp
        android/AppInstanceAndroidBase.cpp
        android/MotionEventSerializer.cpp
        android/JVM_MANAGER.cpp

        # helpers
        helpers/DiligentAppBase.cpp

        # user application
        AppInstance.cpp
)

target_compile_options(native-lib PRIVATE -DUNICODE -DENGINE_DLL)

target_link_libraries(native-lib
        android
        log
        GLESv3
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