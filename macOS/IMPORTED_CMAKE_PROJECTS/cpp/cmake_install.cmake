# Install script for directory: /Users/smallville7123/StudioProjects/Graphical-Tool-Kit/app/src/main/cpp

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentCore/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentEngine/DiligentTools/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/Utils/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/MotionEvent/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/TimeEngine/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/DiligentHelpers/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/PlatformBase/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/WINAPI_KERNEL/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/VertexEngine/cmake_install.cmake")
  include("/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/Objects/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/smallville7123/StudioProjects/Graphical-Tool-Kit/MacOS/IMPORTED_CMAKE_PROJECTS/cpp/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
