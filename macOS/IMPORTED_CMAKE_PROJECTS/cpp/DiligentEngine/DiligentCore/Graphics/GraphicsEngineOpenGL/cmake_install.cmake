# Install script for directory: /Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug" TYPE SHARED_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/Debug/libGraphicsEngineOpenGL.dylib")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libGraphicsEngineOpenGL.dylib" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libGraphicsEngineOpenGL.dylib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Debug/libGraphicsEngineOpenGL.dylib")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release" TYPE SHARED_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/Release/libGraphicsEngineOpenGL.dylib")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libGraphicsEngineOpenGL.dylib" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libGraphicsEngineOpenGL.dylib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/Release/libGraphicsEngineOpenGL.dylib")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel" TYPE SHARED_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/MinSizeRel/libGraphicsEngineOpenGL.dylib")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libGraphicsEngineOpenGL.dylib" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libGraphicsEngineOpenGL.dylib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/MinSizeRel/libGraphicsEngineOpenGL.dylib")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/RelWithDebInfo/libGraphicsEngineOpenGL.dylib")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libGraphicsEngineOpenGL.dylib" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libGraphicsEngineOpenGL.dylib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/DiligentEngine/DiligentCore/RelWithDebInfo/libGraphicsEngineOpenGL.dylib")
      endif()
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/" TYPE DIRECTORY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/interface")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/" TYPE DIRECTORY FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentCore/Graphics/GraphicsEngineOpenGL/interface")
endif()

