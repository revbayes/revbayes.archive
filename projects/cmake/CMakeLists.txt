cmake_minimum_required(VERSION 2.6)
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
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../trunk/src)

# TODO Split these up based on sub-package dependency
include_directories(
    ${PROJECT_SOURCE_DIR}/core/Xml		
    ${PROJECT_SOURCE_DIR}/core/dag		
    ${PROJECT_SOURCE_DIR}/core/distributions
    ${PROJECT_SOURCE_DIR}/core/distributions/phylogenetics
    ${PROJECT_SOURCE_DIR}/core/distributions/statistics
    ${PROJECT_SOURCE_DIR}/core/help		
    ${PROJECT_SOURCE_DIR}/core/io		
    ${PROJECT_SOURCE_DIR}/core/math
    ${PROJECT_SOURCE_DIR}/core/math/distributions
    ${PROJECT_SOURCE_DIR}/core/ncl		
    ${PROJECT_SOURCE_DIR}/core/utils
    ${PROJECT_SOURCE_DIR}/core/convergence	
    ${PROJECT_SOURCE_DIR}/core/datatypes
    ${PROJECT_SOURCE_DIR}/core/datatypes/phylogenetics/characters
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/container
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/container/Matrix
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/container/Vector
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/container/Vector/VectorCharData
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/memberobject
    ${PROJECT_SOURCE_DIR}/core/datatypes/complex/tree
    ${PROJECT_SOURCE_DIR}/core/datatypes/model
    ${PROJECT_SOURCE_DIR}/core/datatypes/phylogenetics
    ${PROJECT_SOURCE_DIR}/core/datatypes/primitive
    ${PROJECT_SOURCE_DIR}/core/functions
    ${PROJECT_SOURCE_DIR}/core/functions/basic
    ${PROJECT_SOURCE_DIR}/core/functions/builtin
    ${PROJECT_SOURCE_DIR}/core/functions/io
    ${PROJECT_SOURCE_DIR}/core/functions/math
    ${PROJECT_SOURCE_DIR}/core/functions/model
    ${PROJECT_SOURCE_DIR}/core/functions/moves
    ${PROJECT_SOURCE_DIR}/core/functions/phylogenetics
    ${PROJECT_SOURCE_DIR}/core/inference
    ${PROJECT_SOURCE_DIR}/core/inference/mcmc     
    ${PROJECT_SOURCE_DIR}/core/main		
    ${PROJECT_SOURCE_DIR}/core/moves
    ${PROJECT_SOURCE_DIR}/core/moves/simple
    ${PROJECT_SOURCE_DIR}/core/moves/tree
    ${PROJECT_SOURCE_DIR}/core/ui		
    ${PROJECT_SOURCE_DIR}/core/workspace   
    ${PROJECT_SOURCE_DIR}/parser
    ${PROJECT_SOURCE_DIR}/parser/functions
    ${PROJECT_SOURCE_DIR}/parser/parser
)

add_subdirectory(core)   # Split into much smaller libraries
add_subdirectory(parser)

add_executable(rb ${PROJECT_SOURCE_DIR}/parser/RbMain.cpp)
target_link_libraries(rb rb-parser rb-core)

