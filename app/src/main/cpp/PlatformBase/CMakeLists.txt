# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(PlatformBase)

add_library(
        PlatformBase SHARED dummy.cpp
)

# Diligent Engine exposes the following macros:
#
# PLATFORM_WIN32
# PLATFORM_UNIVERSAL_WINDOWS
# PLATFORM_ANDROID
# PLATFORM_LINUX
# PLATFORM_MACOS
# PLATFORM_IOS
# PLATFORM_TVOS

if (PLATFORM_WIN32)
    add_subdirectory(Win32Base)
    target_link_libraries(PlatformBase Win32 MultiTouch)
elseif (PLATFORM_UNIVERSAL_WINDOWS)
    add_subdirectory(UniversalWindowsBase)
    target_link_libraries(PlatformBase UniversalWindowsBase MultiTouch)
elseif (PLATFORM_ANDROID)
    add_subdirectory(AndroidBase)
    target_link_libraries(PlatformBase AndroidBase MultiTouch)
elseif (PLATFORM_LINUX)
    add_subdirectory(LinuxBase)
    target_link_libraries(PlatformBase LinuxBase MultiTouch)
elseif (PLATFORM_MACOS)
    add_subdirectory(MacOSBase)
    target_link_libraries(PlatformBase MacOSBase MultiTouch)
    set_target_properties(
            PlatformBase PROPERTIES
            FRAMEWORK TRUE
            XCODE_EMBED_FRAMEWORKS "MacOSBase"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
    )
elseif (PLATFORM_IOS)
    add_subdirectory(iOSBase)
    target_link_libraries(PlatformBase iOSBase)
    set_target_properties(
            PlatformBase PROPERTIES
            FRAMEWORK TRUE
            XCODE_EMBED_FRAMEWORKS "iOSBase"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
    )
elseif (PLATFORM_TVOS)
    add_subdirectory(TvOSBase)
    target_link_libraries(PlatformBase TvOSBase MultiTouch)
    set_target_properties(
            PlatformBase PROPERTIES
            FRAMEWORK TRUE
            XCODE_EMBED_FRAMEWORKS "TvOSBase"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
