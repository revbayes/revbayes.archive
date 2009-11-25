# The name of our project is "REvBayes". CMakeLists files in this project can
# refer to the root source directory of the project as ${REVBAYES_SOURCE_DIR} and
# to the root binary directory of the project as ${REVBAYES_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (REVBAYES)

# include path to all subdirectories
include_directories (main)
include_directories (dag)
include_directories (datatypes)
include_directories (datatypes/complex)
include_directories (datatypes/model)
include_directories (datatypes/primitive)
include_directories (distributions)
include_directories (factory)
include_directories (functions)
include_directories (functions/distribution)
include_directories (functions/command)
include_directories (functions/math)
include_directories (functions/model)
include_directories (inference)
include_directories (moves)
include_directories (moves/functions)
include_directories (moves/objects)
include_directories (parser)
include_directories (probabilityCalculators)
include_directories (test)
include_directories (ui)
include_directories (utils)

# Recurse into the subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (main)
#add_subdirectory (dag)
#add_subdirectory (datatypes)
#add_subdirectory (distributions)
#add_subdirectory (factory)
#add_subdirectory (functions)
#add_subdirectory (inference)
#add_subdirectory (moves)
#add_subdirectory (probabilityCalculators)
add_subdirectory (test)
#add_subdirectory (ui)
#add_subdirectory (utils)

#enable the testing framework
ENABLE_TESTING()