### add test for distributions

### normal distribution
add_executable(testMCMC ../../datatypes/RbObject.cpp ../../datatypes/complex/StringVector.cpp ../../datatypes/complex/RbComplex.cpp \\
../../utils/RbException.cpp ../../datatypes/primitive/RbPrimitive.cpp \\
../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp  ../../dag/StochasticNode.cpp ../../dag/DeterministicNode.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qnorm.cpp \\
../../functions/distribution/RbFunction_rnorm.cpp ../../utils/RbStatistics.cpp ../../utils/RbMath.cpp testMCMC.cpp)
