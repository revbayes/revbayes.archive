# The name of our project is "REvBayes". CMakeLists files in this project can
# refer to the root source directory of the project as ${REVBAYES_SOURCE_DIR} and
# to the root binary directory of the project as ${REVBAYES_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (REVBAYES)

# include path to all subdirectories
include_directories(main)
include_directories(datatypes)
#SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH};main)
#CMAKE_INCLUDE_PATH (main)
#CMAKE_INCLUDE_PATH (dag)
#CMAKE_INCLUDE_PATH (datatypes)
#CMAKE_INCLUDE_PATH (distributions)
#CMAKE_INCLUDE_PATH (factory)
#CMAKE_INCLUDE_PATH (functions)
#CMAKE_INCLUDE_PATH (inference)
#CMAKE_INCLUDE_PATH (modelLanguage)
#CMAKE_INCLUDE_PATH (moves)
#CMAKE_INCLUDE_PATH (probabilityCalculators)
#CMAKE_INCLUDE_PATH (test)
#CMAKE_INCLUDE_PATH (ui)
#CMAKE_INCLUDE_PATH (utils)

# Recurse into the subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (main)
add_subdirectory (dag)
add_subdirectory (datatypes)
add_subdirectory (distributions)
add_subdirectory (factory)
add_subdirectory (functions)
add_subdirectory (inference)
add_subdirectory (modelLanguage)
add_subdirectory (moves)
#add_subdirectory (probabilityCalculators)
add_subdirectory (test)
add_subdirectory (ui)
add_subdirectory (utils)

#enable the testing framework
ENABLE_TESTING()