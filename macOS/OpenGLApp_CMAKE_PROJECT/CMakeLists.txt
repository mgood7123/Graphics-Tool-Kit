cmake_minimum_required(VERSION 3.10.2)
project(CMAKE_OPENGL_APP_BUNDLE)

# Generate XCode schemes
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Application Bundle

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(../../app/src/main/cpp ${CMAKE_CURRENT_BINARY_DIR}/GRAPHICS)

set(cube_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/AppDelegate.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderer.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/GLEssentialsGLView.mm
)
set(cube_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/AppDelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GLEssentialsGLView.h
)
set(cube_RESOURCES
# Storyboard's here
# Xib's here
    ${CMAKE_CURRENT_SOURCE_DIR}/Base.lproj/MainMenu.xib
)

set(CMAKE_WARN_DEPRECATED FALSE)

add_executable(cube MACOSX_BUNDLE
    ${cube_SRCS}
    ${cube_HDRS}
    ${cube_RESOURCES}
)

# do not warn about GL's deprecated api's
set_property(
    TARGET cube
     APPEND PROPERTY
      COMPILE_DEFINITIONS GL_SILENCE_DEPRECATION
)

# do not warn about API deprecation
set_property(
    TARGET cube
     APPEND PROPERTY
      COMPILE_OPTIONS -Wno-deprecated-declarations
)

# Include demo source code dir because the MacOS cube's Objective-C source includes
# the "original" cube application C source code.
# Also include the MoltenVK helper files.
target_include_directories(cube PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    cube
    "-framework Cocoa"
    "-framework CoreVideo"
    "-framework OpenGL"
    AppInstance
)

set_target_properties(cube PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
    XCODE_EMBED_FRAMEWORKS AppInstance
    XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY TRUE
)

# The RESOURCE target property cannot be used in conjunction with the MACOSX_PACKAGE_LOCATION
# property.  We need fine-grained control over the Resource directory, so we have to specify
# the destination of all the resource files on a per-destination-directory basis.
# If all the files went into the top-level Resource directory, then we could simply set
# the RESOURCE property to a list of all the resource files.
set_source_files_properties(${cube_RESOURCES} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

# Fix up the library search path in the executable to find (loader) libraries in the bundle.
install(CODE "
    include(BundleUtilities)
    fixup_bundle(${CMAKE_INSTALL_PREFIX}/cube.app \"\" \"\")
    verify_app(${CMAKE_INSTALL_PREFIX}/cube.app)
    " COMPONENT Runtime
)
