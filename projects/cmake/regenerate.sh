#!/bin/sh
HERE=$(pwd)
echo $HERE
cd ../../src

# 1. CMakeLists.txt
echo 'cmake_minimum_required(VERSION 2.6)
project(RevBayes)

# Consider:
# -Wno-sign-compare
# -D_GLIBCXX_DEBUG

# Default compiler flags
if (WIN32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg -static")
else ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg")
endif ()

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)

# TODO Split these up based on sub-package dependency
include_directories(' > $HERE/CMakeLists.txt
find core revlanguage -type d | grep -v "svn" | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> $HERE/CMakeLists.txt
echo ')

add_subdirectory(core)   # Split into much smaller libraries
add_subdirectory(revlanguage)

add_executable(rb ${PROJECT_SOURCE_DIR}/revlanguage/main.cpp)
target_link_libraries(rb rb-parser rb-core)
' >> $HERE/CMakeLists.txt

if [ ! -d $HERE/core ]; then
mkdir $HERE/core
fi
echo 'set(CORE_FILES' > $HERE/core/CMakeLists.txt
find core | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/core/CMakeLists.txt
echo ')
add_library(rb-core ${CORE_FILES})'  >> $HERE/core/CMakeLists.txt

if [ ! -d $HERE/revlanguage ]; then
mkdir $HERE/revlanguage
fi
echo 'set(PARSER_FILES' > $HERE/revlanguage/CMakeLists.txt
find revlanguage | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/revlanguage/CMakeLists.txt
echo ')
add_library(rb-parser ${PARSER_FILES})'  >> $HERE/revlanguage/CMakeLists.txt

