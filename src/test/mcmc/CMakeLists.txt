### add test for distributions

### normal distribution
add_executable(testMCMC \\
../../datatypes/RbObject.cpp ../../datatypes/primitive/RbPrimitive.cpp ../../datatypes/complex/RbComplex.cpp \\
../../datatypes/complex/StringVector.cpp ../../utils/RbException.cpp  \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp  ../../dag/StochasticNode.cpp ../../dag/DeterministicNode.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../moves/objects/MoveSlidingWindow.cpp ../../moves/objects/MoveScale.cpp \\
../../distributions/Distribution.cpp ../../distributions/statistics/DistExponential.cpp ../../distributions/statistics/DistNormal.cpp \\
../../distributions/statistics/DistUniform.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../utils/RbStatistics.cpp ../../utils/RbMath.cpp testMCMC.cpp)
