# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(Kit)

set(CMAKE_CXX_STANDARD 17)
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

# TODO: seperate the logging library from Diligent-Common into a submodule DiligentLog
# TODO: which DiligentCore will then link against
#
# add_subdirectory(DiligentEngine/DiligentLog)

add_subdirectory(MultiTouch)
include_directories(MultiTouch/DiligentLog/Primitives/interface)
add_subdirectory(DiligentEngine/DiligentCore)
#add_subdirectory(DiligentEngine/DiligentFX)
add_subdirectory(DiligentEngine/DiligentTools)

include_directories(
        DiligentEngine
        MultiTouch
        MultiTouch/Utils
)

add_subdirectory(TimeEngine)
include_directories(TimeEngine)

add_subdirectory(DiligentHelpers)
include_directories(DiligentHelpers)

if (PLATFORM_WIN32 OR PLATFORM_LINUX)
    add_subdirectory(SDL2-2.0.14)
    include_directories(SDL2-2.0.14/include)
endif()

add_subdirectory(PlatformBase)
include_directories(PlatformBase)

add_subdirectory(WINAPI_KERNEL)

add_subdirectory(VertexEngine)
include_directories(VertexEngine)

add_subdirectory(Objects)
include_directories(Objects)
include_directories(Objects/Views)

add_library(
        AppInstance SHARED
        AppInstance.cpp
)

target_link_libraries(
        AppInstance
        Objects
        PlatformBase
        WINAPI::KERNEL
)

if (PLATFORM_ANDROID)
    add_library(native-lib SHARED native-lib.cpp)

    target_link_libraries(native-lib AppInstance)
endif()
