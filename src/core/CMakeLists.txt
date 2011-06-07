# The name of our project is "RevBayes". CMakeLists files in this project can
# refer to the root source directory of the project as ${REVBAYES_SOURCE_DIR} and
# to the root binary directory of the project as ${REVBAYES_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (REVBAYES)

# dag
SET ( dag \\
../../dag/ConstantNode.cpp \\
../../dag/DAGNode.cpp \\
../../dag/DAGNodeContainer.cpp \\
../../dag/DeterministicNode.cpp \\
../../dag/StochasticNode.cpp \\
../../dag/VariableNode.cpp )

# data types
SET ( complex \\
../../datatypes/complex/Intvector.cpp \\
../../datatypes/complex/RbBitset.cpp \\
../../datatypes/complex/RbComplex.cpp \\
../../datatypes/complex/RbString.cpp \\
../../datatypes/complex/StringVector.cpp \\
../../datatypes/complex/Vector.cpp)

SET ( model \\
../../datatypes/model/RbBranch.cpp \\
../../datatypes/model/RbCharacterMatrix.cpp \\
../../datatypes/model/RbDnaAlignment.cpp \\
../../datatypes/model/Model.cpp \\
../../datatypes/model/RbNode.cpp \\
../../datatypes/model/RbTopology.cpp \\
../../datatypes/model/RbUnrootedTopology.cpp)

SET ( primitive \\
../../datatypes/primitive/RbBool.cpp \\
../../datatypes/primitive/RbDouble.cpp \\
../../datatypes/primitive/RbInt.cpp )

SET ( data_types \\
${primitive} ${complex} ${model} \\
../../datatypes/RbInternal.cpp \\
../../datatypes/RbObject.cpp \\
../../datatypes/RbUndefined.cpp )

# distributions
SET ( dist_phylogenetic \\
../../distributions/phylogenetic/Distribution_Nucleotide.cpp \\
../../distributions/phylogenetic/DistUnifUnrootedTopology.cpp)

SET ( dist_statistic
../../distributions/statistics/DistBeta.cpp
../../distributions/statistics/DistDirichlet.cpp
../../distributions/statistics/Dist_exp.cpp
../../distributions/statistics/Dist_norm.cpp
../../distributions/statistics/Dist_unif.cpp )

SET ( distribution \\
../../distributions/Distribution.cpp \\
${dist_phylogenetic} ${dist_statistic})

# factory
SET ( factory \\
../../factory/RandomNumberFactory.cpp)


# functions
SET ( function_basic
../../functions/basic/func__lookup.cpp
../../functions/basic/func__range.cpp )
SET ( function_builtin )
SET ( function_command )
SET ( function_math
../../functions/math/RbFunction_ln.cpp
../../functions/math/RbFunction_pow.cpp
../../functions/math/RbFunction_sqrt.cpp )
SET ( function_model
../../functions/model/RbFunction_GTR.cpp
../../functions/model/RbFunction_readCharacterMatrix.cpp )
SET ( function
${function_basic}
${function_builtin}
${function_command}
${function_math}
${function_model}
../../functions/Argument.cpp
../../functions/ArgumentRule.cpp
../../functions/RbFunction.cpp )

# inference
SET ( mcmc
../../inference/mcmc/Mcmc.cpp )
SET ( inference ${mcmc} )

# main
SET ( main
../../main/Monitor.cpp )

# moves
SET ( moves_simple
../../moves/simple/Move_mscale.cpp
../../moves/simple/Move_mslide.cpp)
SET ( moves ${moves_simple}
../../moves/MoveSchedule.cpp
../../moves/Move.cpp
../../moves/SimpleMove.cpp)

# parser
SET ( parser ../../parser/FunctionTable.cpp)

# ui
SET ( ui )

# utils
SET ( utils_phylogenetic
../../utils/phylogenetic/NewickConverter.cpp)
SET ( utils ${utils_phylogenetic}
../../utils/ContainerIterator.cpp
../../utils/RandomNumberGenerator.cpp
../../utils/RbException.cpp
../../utils/RbMath.cpp
../../utils/RbStatistics.cpp )

# all sources
SET ( sources ${dag} ${data_types} ${distribution} ${factory} ${function} ${inference} ${main} ${moves} ${parser} ${ui} ${utils})

# include path to all subdirectories
include_directories (main)
include_directories (dag)
include_directories (datatypes)
include_directories (datatypes/complex)
include_directories (datatypes/model)
include_directories (datatypes/phylogenetic)
include_directories (datatypes/primitive)
include_directories (distributions)
include_directories (distributions/phylogenetic)
include_directories (distributions/statistics)
include_directories (factory)
include_directories (functions)
include_directories (functions/basic)
include_directories (functions/builtin)
include_directories (functions/distribution)
include_directories (functions/command)
include_directories (functions/math)
include_directories (functions/model)
include_directories (inference)
include_directories (inference/mcmc)
include_directories (moves)
include_directories (moves/simple)
include_directories (parser)
include_directories (probabilityCalculators)
include_directories (test)
include_directories (ui)
include_directories (utils)
include_directories (utils/phylogenetic)

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
