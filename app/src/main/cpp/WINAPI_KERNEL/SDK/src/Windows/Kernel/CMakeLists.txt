cmake_minimum_required(VERSION 3.4.1)

add_library(WINAPIKERNEL SHARED
        WindowsAPIKernel.cpp
        WindowsAPITable.cpp
        WindowsAPIObject.cpp
        WindowsAPIHandle.cpp
)

target_link_libraries(WINAPIKERNEL DiligentAnyOpt)

# Windows API Kernel target alias for superprojects
add_library(WINAPI::KERNEL ALIAS WINAPIKERNEL)
