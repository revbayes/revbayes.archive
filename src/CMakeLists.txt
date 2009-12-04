# The name of our project is "REvBayes". CMakeLists files in this project can
# refer to the root source directory of the project as ${REVBAYES_SOURCE_DIR} and
# to the root binary directory of the project as ${REVBAYES_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (REVBAYES)

# dag
SET ( dag ../../dag/DAGNode.cpp ../../dag/ConstantNode.cpp ../../dag/DeterministicNode.cpp ../../dag/StochasticNode.cpp )

# data types
SET ( model ../../datatypes/model/RbModel.cpp )
SET ( complex ../../datatypes/complex/RbComplex.cpp ../../datatypes/complex/RbBitset.cpp ../../datatypes/complex/RbList.cpp ../../datatypes/complex/RbString.cpp ../../datatypes/complex/StringVector.cpp )
SET ( primitive../../datatypes/primitive/RbPrimitive.cpp ../../datatypes/primitive/RbBool.cpp ../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp )
SET ( data_types ${primitive} ${complex} ${model} ../../datatypes/RbObject.cpp ../../datatypes/RbUndefined.cpp )

# distributions
SET ( dist_phylogenetic )
#SET ( dist_statistic ../../distributions/statistics/DistBeta.cpp ../../distributions/statistics/DistDirichlet.cpp ../../distributions/statistics/DistExponential.cpp ../../distributions/statistics/DistNormal.cpp ../../distributions/statistics/DistUniform.cpp )
SET ( dist_statistic ../../distributions/statistics/DistExponential.cpp ../../distributions/statistics/DistNormal.cpp ../../distributions/statistics/DistUniform.cpp )
SET ( distribution ../../distributions/Distribution.cpp ${dist_phylogenetic} ${dist_statistic})

# functions
SET ( function_builtin )
SET ( function_command )
SET ( function_distribution ../../functions/distribution/RbFunction_dbeta.cpp ../../functions/distribution/RbFunction_dchisq.cpp ../../functions/distribution/RbFunction_dexp.cpp ../../functions/distribution/RbFunction_dgamma.cpp ../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pbeta.cpp ../../functions/distribution/RbFunction_pchisq.cpp ../../functions/distribution/RbFunction_pexp.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qbeta.cpp ../../functions/distribution/RbFunction_qchisq.cpp ../../functions/distribution/RbFunction_qexp.cpp ../../functions/distribution/RbFunction_qnorm.cpp ../../functions/distribution/RbFunction_rbeta.cpp ../../functions/distribution/RbFunction_rchisq.cpp ../../functions/distribution/RbFunction_rexp.cpp ../../functions/distribution/RbFunction_rnorm.cpp )
SET ( function_math ../../functions/math/RbFunction_ln.cpp ../../functions/math/RbFunction_pow.cpp ../../functions/math/RbFunction_sqrt.cpp )
SET ( function_model ../../functions/model/RbFunction_readCharacterMatrix.cpp )
SET ( function ${function_builtin} ${function_command} ${function_distribution} ${function_math} ${function_model} ../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp )

# inference
SET ( mcmc ../../inference/mcmc/RbMcmc.cpp )
SET ( inference ${mcmc} )

# main
SET ( main ../../main/RbMonitor.cpp )

# moves
SET ( move_function ../../moves/functions/RbMoveFunction.cpp)
SET ( moves_objects ../../moves/objects/MoveScale.cpp ../../moves/objects/MoveSlidingWindow.cpp ../../moves/objects/RbMove.cpp )
SET ( moves ${moves_function} ${moves_objects} )

# parser
SET ( parser )

# ui
SET ( ui )

# utils
SET ( utils ../../utils/RandomNumberGenerator.cpp ../../utils/RbException.cpp ../../utils/RbMath.cpp ../../utils/RbStatistics.cpp ../../utils/ )

# all sources
SET ( sources ${dag} ${data_types} ${distribution} ${function} ${inference} ${main} ${moves} ${parser} ${ui} ${utils})

# include path to all subdirectories
include_directories (main)
include_directories (dag)
include_directories (datatypes)
include_directories (datatypes/complex)
include_directories (datatypes/model)
include_directories (datatypes/primitive)
include_directories (distributions)
include_directories (distributions/statistics)
include_directories (factory)
include_directories (functions)
include_directories (functions/distribution)
include_directories (functions/command)
include_directories (functions/math)
include_directories (functions/model)
include_directories (inference)
include_directories (inference/mcmc)
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