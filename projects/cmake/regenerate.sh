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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -g -pg")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg")

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)

# TODO Split these up based on sub-package dependency
include_directories(' > $HERE/CMakeLists.txt
find core parser -type d | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> $HERE/CMakeLists.txt
echo ')

add_subdirectory(core)   # Split into much smaller libraries
add_subdirectory(parser)

add_executable(rb ${PROJECT_SOURCE_DIR}/parser/RbMain.cpp)
target_link_libraries(rb rb-parser rb-core)
' >> $HERE/CMakeLists.txt


echo 'set(CORE_FILES' > $HERE/core/CMakeLists.txt
find core | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/core/CMakeLists.txt
echo ')
add_library(rb-core ${CORE_FILES})'  >> $HERE/core/CMakeLists.txt


echo 'set(PARSER_FILES' > $HERE/parser/CMakeLists.txt
find parser | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/parser/CMakeLists.txt
echo ')
add_library(rb-parser ${PARSER_FILES})'  >> $HERE/parser/CMakeLists.txt

