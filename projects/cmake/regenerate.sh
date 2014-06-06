#!/bin/sh
HERE=$(pwd)
echo $HERE

#################
# command line options
# set default values
boost="true"
mavericks="false"
win="false"


# parse command line arguments
while echo $1 | grep ^- > /dev/null; do
    # intercept help while parsing "-key value" pairs
    if [ "$1" = "--help" ] || [ "$1" = "-h" ]    
    then 
        echo '
        The minimum steps to build RevBayes after running this script is:
            cmake .
            make
        
        Command line options are: 
            -h | -help                  : print this help and exit.
            -boost      <true|false>    : true (re)compiles boost libs, false dont. Defaults to true.
            -mavericks  <true|false>    : set to true if you are building on a OS X - Mavericks system. Defaults to false.
            -win        <true|false>    : set to true if you are building on a Windows system. Defaults to false.
            '
        exit
    fi

    # parse pairs
    eval $( echo $1 | sed 's/-//g' | tr -d '\012')=$2
    shift
    shift
done


#################
# build boost libraries separately

if [ "$boost" = "true" ]
then
    echo 'Building boost libraries'
    echo 'you can turn this of with argument "-boost false"'
    
    cd ../../boost_1_55_0
    rm ./project-config.jam*  # clean up from previous runs
    ./bootstrap.sh --with-libraries=system,filesystem,regex,thread,date_time,program_options,math,iostreams,serialization,context,signals

    if [ "$mavericks" = "true" ]
    then
        ./b2 cxxflags="-stdlib=libstdc++" linkflags="-stdlib=libstdc++ -lpthread"
    else
        ./b2 linkflags="-lpthread"
    fi

else
    echo 'not building boost libraries'
fi


#################
# generate cmake configuration
cd "$HERE"
cd ../../src

echo 'cmake_minimum_required(VERSION 2.6)
project(RevBayes)

# Consider:
# -Wno-sign-compare
# -D_GLIBCXX_DEBUG

# Default compiler flags
#if (WIN32)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg -static")
#else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -stdlib=libstdc++")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
#endif ()

' > "$HERE/CMakeLists.txt"

if [ "$mavericks" = "true" ]
then
    echo '
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -stdlib=libstdc++")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
    '  >> "$HERE/CMakeLists.txt"

elif [ "$win" = "true" ]
then
    echo '
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -static -msse -msse2 -msse3")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg -static")
    '  >> "$HERE/CMakeLists.txt"
else
    echo '
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g -pg -msse -msse2 -msse3 -lpthread")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -g -pg")
    '  >> "$HERE/CMakeLists.txt"
fi  

echo '
# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../src)

# TODO Split these up based on sub-package dependency
include_directories(' >> "$HERE/CMakeLists.txt"
find ui libs core revlanguage -type d | grep -v "svn" | sed 's|^|    ${PROJECT_SOURCE_DIR}/|g' >> "$HERE/CMakeLists.txt"
echo ')

# Split into much smaller libraries
add_subdirectory(ui)
add_subdirectory(libs)
add_subdirectory(core)   
add_subdirectory(revlanguage)

############# executables #################
# basic rev-bayes binary
add_executable(rb ${PROJECT_SOURCE_DIR}/revlanguage/main.cpp)
target_link_libraries(rb rb-parser rb-core)

# extended rev-bayes binary
add_executable(rb-extended ${PROJECT_SOURCE_DIR}/ui/main.cpp)

' >> "$HERE/CMakeLists.txt"

if [ "$win" = "true" ]
then
    echo '
    target_link_libraries(rb-extended rb-ui rb-parser rb-core libs boost_system boost_filesystem boost_regex boost_thread boost_date_time boost_program_options boost_math_c99 boost_math_c99f boost_math_tr1f boost_math_tr1l boost_iostreams boost_serialization)
    '  >> "$HERE/CMakeLists.txt"
else
    echo '
    target_link_libraries(rb-extended rb-ui rb-parser rb-core libs boost_system boost_filesystem boost_regex boost_thread boost_date_time boost_program_options boost_math_c99 boost_math_c99f boost_math_tr1f boost_math_tr1l boost_iostreams boost_serialization pthread)
    '  >> "$HERE/CMakeLists.txt"
fi  

echo '


# utility for generating help html files.
add_executable(help-html-generator ${PROJECT_SOURCE_DIR}/ui/utils/HelpHtmlGenerator.cpp)
target_link_libraries(help-html-generator rb-parser rb-core libs boost_filesystem boost_system boost_regex)

########## boost section ##############

add_library(boost_signals SHARED IMPORTED)
set_target_properties(boost_signals PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_signals.a")

add_library(boost_context SHARED IMPORTED)
set_target_properties(boost_context PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_context.a")

add_library(boost_serialization SHARED IMPORTED)
set_target_properties(boost_serialization PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_serialization.a")

add_library(boost_iostreams SHARED IMPORTED)
set_target_properties(boost_iostreams PROPERTIES IMPORTED_LOCATION "../../boost_1_55_0/stage/lib/libboost_iostreams.a")

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
find ui | grep -v "svn" | sed 's|^|${PROJECT_SOURCE_DIR}/|g' >> "$HERE/ui/CMakeLists.txt"
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

