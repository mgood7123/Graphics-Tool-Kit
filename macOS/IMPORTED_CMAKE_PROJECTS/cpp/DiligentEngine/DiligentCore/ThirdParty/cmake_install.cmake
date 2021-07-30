# Install script for directory: /Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty

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
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/Debug/libglew-static.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libglew-static.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libglew-static.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libglew-static.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/Release/libglew-static.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libglew-static.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libglew-static.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libglew-static.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/MinSizeRel/libglew-static.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libglew-static.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libglew-static.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libglew-static.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/RelWithDebInfo/libglew-static.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libglew-static.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libglew-static.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libglew-static.a")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE RENAME "GLEW-License.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/LICENSE.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE RENAME "googletest-License.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/googletest/LICENSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/stb/stb_image_write_license.txt")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE RENAME "Volk-License.md" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/volk/LICENSE.md")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE RENAME "DXC-License.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/DirectXShaderCompiler/LICENSE.TXT")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Licenses/ThirdParty/DiligentEngine/DiligentCore" TYPE FILE RENAME "DXC-ThirdPartyNotices.txt" FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/ThirdParty/DirectXShaderCompiler/ThirdPartyNotices.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/ThirdParty/glew/cmake_install.cmake")

endif()

