### add test for distributions

### normal distribution mcmc
add_executable(testMCMC \\
../../datatypes/RbObject.cpp ../../datatypes/primitive/RbPrimitive.cpp ../../datatypes/complex/RbComplex.cpp \\
../../datatypes/complex/StringVector.cpp ../../utils/RbException.cpp  \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp  ../../dag/StochasticNode.cpp ../../dag/DeterministicNode.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../moves/objects/MoveSlidingWindow.cpp ../../moves/objects/MoveScale.cpp \\
../../distributions/Distribution.cpp ../../distributions/statistics/DistExponential.cpp ../../distributions/statistics/DistNormal.cpp \\
../../distributions/statistics/DistUniform.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp ../../main/RbMonitor.cpp \\
../../utils/RbStatistics.cpp ../../utils/RbNames.h ../../utils/RbMath.cpp ../../datatypes/model/RbModel.cpp \\
../../inference/mcmc/RbMcmc.cpp testMCMC.cpp)


### gtr mcmc
add_executable(testGTR \\
../../datatypes/RbObject.cpp ../../datatypes/primitive/RbPrimitive.cpp ../../datatypes/complex/RbComplex.cpp \\
../../datatypes/complex/StringVector.cpp ../../utils/RbException.cpp  \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp  ../../dag/StochasticNode.cpp ../../dag/DeterministicNode.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../moves/objects/MoveSlidingWindow.cpp ../../moves/objects/MoveScale.cpp \\
../../distributions/Distribution.cpp ../../distributions/statistics/DistExponential.cpp ../../distributions/statistics/DistNormal.cpp \\
../../distributions/statistics/DistUniform.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp ../../main/RbMonitor.cpp \\
../../utils/RbStatistics.cpp ../../utils/RbNames.h ../../utils/RbMath.cpp ../../datatypes/model/RbModel.cpp \\
../../inference/mcmc/RbMcmc.cpp testGTR.cpp)
