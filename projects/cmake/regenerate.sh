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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg -static")

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)

# TODO Split these up based on sub-package dependency
include_directories(' > $HERE/CMakeLists.txt
find core revlanguage test -type d | grep -v "svn" | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> $HERE/CMakeLists.txt
echo ')

add_subdirectory(core)   # Split into much smaller libraries
add_subdirectory(revlanguage)
add_subdirectory(test)

add_executable(rb ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(rb rb-parser rb-core rb-test)
' >> $HERE/CMakeLists.txt


echo 'set(CORE_FILES' > $HERE/core/CMakeLists.txt
find core | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/core/CMakeLists.txt
echo ')
add_library(rb-core ${CORE_FILES})'  >> $HERE/core/CMakeLists.txt


echo 'set(PARSER_FILES' > $HERE/revlanguage/CMakeLists.txt
find revlanguage | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/revlanguage/CMakeLists.txt
echo ')
add_library(rb-parser ${PARSER_FILES})'  >> $HERE/revlanguage/CMakeLists.txt


echo 'set(TEST_FILES' > $HERE/test/CMakeLists.txt
find test | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> $HERE/test/CMakeLists.txt
echo ')
add_library(rb-test ${TEST_FILES})'  >> $HERE/test/CMakeLists.txt

