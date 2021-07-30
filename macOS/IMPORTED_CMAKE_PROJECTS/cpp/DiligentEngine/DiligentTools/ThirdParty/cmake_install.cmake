# Install script for directory: /Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

set(CMAKE_BINARY_DIR "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp")

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "libjpeg-readme.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/README")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "libtiff-copyright.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/COPYRIGHT")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "lpng-license.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/LICENSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "tinygltf-license.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/tinygltf/LICENSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "zlib-readme.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/README")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "imgui-license.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/LICENSE.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentTools" TYPE FILE RENAME "imGuIZMO.quat-license.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imGuIZMO.quat/license.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/Debug/libLibJpeg.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibJpeg.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibJpeg.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibJpeg.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/Release/libLibJpeg.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibJpeg.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibJpeg.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibJpeg.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/MinSizeRel/libLibJpeg.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibJpeg.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibJpeg.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibJpeg.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/RelWithDebInfo/libLibJpeg.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibJpeg.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibJpeg.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibJpeg.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/Debug/libLibPng.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibPng.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibPng.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibPng.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/Release/libLibPng.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibPng.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibPng.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibPng.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/MinSizeRel/libLibPng.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibPng.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibPng.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibPng.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/RelWithDebInfo/libLibPng.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibPng.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibPng.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibPng.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/Debug/libLibTiff.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibTiff.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibTiff.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libLibTiff.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/Release/libLibTiff.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibTiff.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibTiff.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libLibTiff.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/MinSizeRel/libLibTiff.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibTiff.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibTiff.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libLibTiff.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/RelWithDebInfo/libLibTiff.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibTiff.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibTiff.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libLibTiff.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/Debug/libZLib.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libZLib.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libZLib.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Debug/libZLib.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/Release/libZLib.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libZLib.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libZLib.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/Release/libZLib.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/MinSizeRel/libZLib.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libZLib.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libZLib.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/MinSizeRel/libZLib.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/RelWithDebInfo/libZLib.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libZLib.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libZLib.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentTools/RelWithDebInfo/libZLib.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/DiligentEngine/DiligentTools/ThirdParty/imgui" TYPE FILE FILES
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imconfig.h"
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imgui.h"
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imgui_internal.h"
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imstb_rectpack.h"
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imstb_textedit.h"
    "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/ThirdParty/imgui/imstb_truetype.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libjpeg-9a/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/libtiff/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/zlib-1.2.8/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/ThirdParty/lpng-1.6.17/cmake_install.cmake")

endif()

