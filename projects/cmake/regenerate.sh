#!/bin/sh
HERE=$(pwd)
echo $HERE

#################
# parse command line arguments
while echo $1 | grep ^- > /dev/null; do
    eval $( echo $1 | sed 's/-//g' | tr -d '\012')=$2
    shift
    shift
done

# set default values
if [ ! -n "$boost" ] # not is set
then 
    boost="true"
fi

#################
# 1) build boost libraries separately

if [ "$boost" = "true" ]
then
    echo 'Building boost libraries'    
    echo 'you can turn this of with argument "-boost false"'
    
    cd ../../boost_1_55_0
    rm ./project-config.jam*  # clean up from previous runs
    ./bootstrap.sh --with-libraries=system,filesystem,regex,thread,date_time,program_options,math
    ./b2
    
else
    echo 'not building boost libraries'
fi

#################
# 2) collect some information that can be used in source
cd $HERE
cd ../../src/ui
echo "/* These values are automatically generated at installation by regenerate.sh script */" > constants.h
echo "static const char* INSTALL_DIR = \"$HERE\";" >> constants.h

cd ../revlanguage
echo "/* These values are automatically generated at installation by regenerate.sh script */" > constants.h
echo "static const char* INSTALL_DIR = \"$HERE\";" >> constants.h


#################
# 3) generate cmake configuration
cd $HERE
cd ../../src

echo 'generating revBayes build files'

# 1. CMakeLists.txt
echo 'cmake_minimum_required(VERSION 2.6)
project(RevBayes)

# Consider:
# -Wno-sign-compare
# -D_GLIBCXX_DEBUG

# Default compiler flags
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg -static")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -Wall -pg")
endif ()

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)

# TODO Split these up based on sub-package dependency
include_directories(' > "$HERE/CMakeLists.txt"
find ui libs core revlanguage -type d | grep -v "svn" | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> "$HERE/CMakeLists.txt"
echo ')

# Split into much smaller libraries
#add_subdirectory(ui)
add_subdirectory(libs)
add_subdirectory(core)   
add_subdirectory(revlanguage)



############# executables #################
# basic rev-bayes binary
add_executable(rb ${PROJECT_SOURCE_DIR}/revlanguage/main.cpp)
target_link_libraries(rb rb-parser rb-core )

# extended rev-bayes binary
add_executable(rb-extended ${PROJECT_SOURCE_DIR}/ui/main.cpp)
target_link_libraries(rb-extended rb-parser rb-core libs boost_system boost_filesystem boost_regex boost_thread boost_date_time boost_program_options boost_math_c99 boost_math_c99f boost_math_tr1f boost_math_tr1l)

# utility for generating help html files.
add_executable(help-html-generator ${PROJECT_SOURCE_DIR}/ui/HelpHtmlGenerator.cpp)
target_link_libraries(help-html-generator rb-parser rb-core libs boost_filesystem boost_system boost_regex)



########## boost section ##############

add_library(boost_system SHARED IMPORTED)
set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_system.a")
           
add_library(boost_filesystem SHARED IMPORTED)
set_target_properties(boost_filesystem PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_filesystem.a")

add_library(boost_regex SHARED IMPORTED)
set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_regex.a")

add_library(boost_thread SHARED IMPORTED)
set_target_properties(boost_thread PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_thread.a")

add_library(boost_date_time SHARED IMPORTED)
set_target_properties(boost_date_time PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_date_time.a")

add_library(boost_program_options SHARED IMPORTED)
set_target_properties(boost_program_options PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_program_options.a")

add_library(boost_math_c99 SHARED IMPORTED)
set_target_properties(boost_math_c99 PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_math_c99.a")

add_library(boost_math_c99f SHARED IMPORTED)
set_target_properties(boost_math_c99f PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_math_c99f.a")

add_library(boost_math_tr1f SHARED IMPORTED)
set_target_properties(boost_math_tr1f PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_math_tr1f.a")

add_library(boost_math_tr1l SHARED IMPORTED)
set_target_properties(boost_math_tr1l PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_math_tr1l.a")


######## end of boost #################



' >> $HERE/CMakeLists.txt

echo

if [ ! -d "$HERE/ui" ]; then
mkdir "$HERE/ui"
fi
echo 'set(UI_FILES' > "$HERE/ui/CMakeLists.txt"
find libs | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/ui/CMakeLists.txt"
echo ')
add_library(rb-ui ${UI_FILES})'  >> "$HERE/ui/CMakeLists.txt"

if [ ! -d "$HERE/libs" ]; then
mkdir "$HERE/libs"
fi
echo 'set(LIBS_FILES' > "$HERE/libs/CMakeLists.txt"
find libs | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/libs/CMakeLists.txt"
echo ')
add_library(libs ${LIBS_FILES})'  >> "$HERE/libs/CMakeLists.txt"


if [ ! -d "$HERE/core" ]; then
mkdir "$HERE/core"
fi
echo 'set(CORE_FILES' > "$HERE/core/CMakeLists.txt"
find core | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/core/CMakeLists.txt"
echo ')
add_library(rb-core ${CORE_FILES})'  >> "$HERE/core/CMakeLists.txt"


if [ ! -d "$HERE/revlanguage" ]; then
mkdir "$HERE/revlanguage"
fi
echo 'set(PARSER_FILES' > "$HERE/revlanguage/CMakeLists.txt"
find revlanguage | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/revlanguage/CMakeLists.txt"
echo ')
add_library(rb-parser ${PARSER_FILES})'  >> "$HERE/revlanguage/CMakeLists.txt"


