# The name of our project is "REvBayes". CMakeLists files in this project can
# refer to the root source directory of the project as ${REVBAYES_SOURCE_DIR} and
# to the root binary directory of the project as ${REVBAYES_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (REVBAYES)

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