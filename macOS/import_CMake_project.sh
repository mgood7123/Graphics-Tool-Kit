#!/bin/bash

#  import_CMake_project.sh
#  MacOS
#
#  Created by Matthew Good on 28/7/21.
#

showUsage() {
        echo "usage: $0 [-h, --help] [[-l, --list] [directory]] [-c, --clean] cmake_project_directory [-c, --clean]"
        echo " -h, --help                   display this help"
        echo " -c, --clean                  cleans the generated project"
        echo "                              directory before importing"
        echo "                              "
        echo "                              this is commonly used to clean"
        echo "                              the project's build directory"
        echo "                              as XCode errors with"
        echo "                              could not delete '<FOLDER>' because it was not created by the build system."
        echo "                              "
        echo " -l, --list    [directory]    displays a list of found"
        echo "                              directories which contain"
        echo "                              a CMakeLists.txt file"
        echo "                              "
        echo "                              if [directory] is given"
        echo "                              then [directory] will be searched"
}

if [[ $# == 0 ]]
    then
        echo a cmake project must be specified
        showUsage
        exit
fi

if [[ "$1" == "-h" || "$1" == "--help" ]]
    then
        showUsage
        exit
fi

if [[ "$1" == "-l" || "$1" == "--list" ]]
    then
        if [[ $# == 2 ]]
            then
                find "$2" -name CMakeLists.txt
            else
                find . -name CMakeLists.txt
            fi
        exit
fi

if [[ "$1" == "-c" || "$1" == "--clean" ]]
    then
        TARGET="$2"
    else
        TARGET="$1"
fi

if [[ ! -e "$TARGET" ]]
    then
        echo "The directory '$TARGET' does not exist"
        exit
fi

if [[ ! -e IMPORTED_CMAKE_PROJECTS ]]
    then
        mkdir IMPORTED_CMAKE_PROJECTS
fi

if [[ -L "$TARGET" ]]
  then
    TARGET="$(readlink "$TARGET")"
fi

FILE="$(basename "$TARGET")"

if [[ "$1" == "-c" || "$1" == "--clean" || "$2" == "-c" || "$2" == "--clean" ]]
    then
        if [[ -e "IMPORTED_CMAKE_PROJECTS/$FILE" ]]
            then
                echo "removing project in 'IMPORTED_CMAKE_PROJECTS/$FILE' ..."
                rm -rf "IMPORTED_CMAKE_PROJECTS/$FILE"
        fi
        if [[ -L "SYMLINK_CMAKE_PROJECT_${FILE}" ]]
            then
                echo "removing symlink 'SYMLINK_CMAKE_PROJECT_${FILE}' ..."
                rm "SYMLINK_CMAKE_PROJECT_${FILE}"
        fi
fi

echo "generating project in 'IMPORTED_CMAKE_PROJECTS/$FILE' ..."
if [[ ! -L "SYMLINK_CMAKE_PROJECT_${FILE}" ]]
    then
        ln -s "$TARGET" "SYMLINK_CMAKE_PROJECT_${FILE}"
fi
cmake "$TARGET" -G Xcode -B "IMPORTED_CMAKE_PROJECTS/$FILE"
echo
echo "You can access the source files via the following directory:"
echo
echo "SYMLINK_CMAKE_PROJECTS/${FILE}"
echo
echo "You can import the project and source files in XCode by:"
echo
echo "Right-click on your project inside XCode"
echo "then click 'Add Files to' and navigate to the following directory and click 'Add'"
echo
echo "SYMLINK_CMAKE_PROJECTS/${FILE}"
echo
echo "Right-click on your project inside XCode"
echo "then click 'Add Files to' and navigate to the following directory and click 'Add'"
find "IMPORTED_CMAKE_PROJECTS/$FILE" -name \*.xcodeproj -depth 1
