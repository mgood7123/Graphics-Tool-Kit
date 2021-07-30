# Install script for directory: /Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp/DiligentEngine/DiligentTools/HLSL2GLSLConverter

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
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Debug" TYPE EXECUTABLE FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/HLSL2GLSLConverter/Debug/HLSL2GLSLConverter")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Debug/HLSL2GLSLConverter" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Debug/HLSL2GLSLConverter")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Debug/HLSL2GLSLConverter")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Release" TYPE EXECUTABLE FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/HLSL2GLSLConverter/Release/HLSL2GLSLConverter")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Release/HLSL2GLSLConverter" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Release/HLSL2GLSLConverter")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/Release/HLSL2GLSLConverter")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/MinSizeRel" TYPE EXECUTABLE FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/HLSL2GLSLConverter/MinSizeRel/HLSL2GLSLConverter")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/MinSizeRel/HLSL2GLSLConverter" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/MinSizeRel/HLSL2GLSLConverter")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/MinSizeRel/HLSL2GLSLConverter")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/RelWithDebInfo" TYPE EXECUTABLE FILES "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/HLSL2GLSLConverter/RelWithDebInfo/HLSL2GLSLConverter")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/RelWithDebInfo/HLSL2GLSLConverter" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/RelWithDebInfo/HLSL2GLSLConverter")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -u -r "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/DiligentEngine/DiligentTools/RelWithDebInfo/HLSL2GLSLConverter")
      endif()
    endif()
  endif()
endif()

