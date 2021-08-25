# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(Kit)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Diligent Engine exposes the following macros:
#
# PLATFORM_WIN32
# PLATFORM_UNIVERSAL_WINDOWS
# PLATFORM_ANDROID
# PLATFORM_LINUX
# PLATFORM_MACOS
# PLATFORM_IOS
# PLATFORM_TVOS

include_directories(WINAPI_KERNEL)

set(DILIGENT_NO_VULKAN TRUE CACHE BOOL "don't need vulkan")
set(DILIGENT_NO_FORMAT_VALIDATION TRUE CACHE BOOL "don't need format validation")

if(WIN32)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        add_definitions(-DPLATFORM_UNIVERSAL_WINDOWS)
    else()
        add_definitions(-DPLATFORM_WIN32)
    endif()
else()
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        add_definitions(-DPLATFORM_ANDROID)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        add_definitions(-DPLATFORM_LINUX)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        if(IOS)
            add_definitions(-DPLATFORM_IOS)
        else()
            add_definitions(-DPLATFORM_MACOS)
        endif()
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
        add_definitions(-DPLATFORM_IOS)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "tvOS")
        add_definitions(-DPLATFORM_TVOS)
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
endif(WIN32)

if(PLATFORM_MACOS OR PLATFORM_IOS OR PLATFORM_TVOS)
    add_definitions(-DPLATFORM_APPLE)
endif()

add_subdirectory(MultiTouch)

add_subdirectory(DiligentEngine/DiligentCore)
#add_subdirectory(DiligentEngine/DiligentFX)
add_subdirectory(DiligentEngine/DiligentTools)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        DiligentEngine
)

add_subdirectory(TimeEngine)

add_subdirectory(DiligentAnyOpt)

add_subdirectory(WINAPI_KERNEL)

add_subdirectory(PixelToNDC)

add_subdirectory(DiligentHelpers)

if (PLATFORM_WIN32 OR PLATFORM_LINUX)
    add_subdirectory(SDL2-2.0.14)
    set(SDL2_INCLUDE SDL2-2.0.14/include)
endif()

add_subdirectory(PlatformBase)

add_subdirectory(VertexEngine)

add_subdirectory(Views)

add_subdirectory(Application)

add_subdirectory(Applications)

add_library(
        AppInstance SHARED
        AppInstance/AppInstance.cpp
)

target_link_libraries(
        AppInstance
        Applications
        PlatformBase
)

if (PLATFORM_ANDROID)
    add_library(native-lib SHARED AppInstance/native-lib.cpp)

    target_link_libraries(native-lib AppInstance)
endif()

target_include_directories(
        AppInstance PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        DiligentEngine
        ${SDL2_INCLUDE}
)

set_target_properties(
        AppInstance PROPERTIES
        FRAMEWORK TRUE
        XCODE_EMBED_FRAMEWORKS "Diligent-GraphicsEngineOpenGL-shared;Views;DiligentAnyOpt;DiligentHelpers;TimeEngine;VertexEngine;MultiTouch;PixelToNDC;PlatformBase;WINAPI::KERNEL;Application;Applications"
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)
