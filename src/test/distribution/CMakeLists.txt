### add test for distributions

### normal distribution
add_executable(testDistNormal ../../datatypes/RbObject.cpp ../../datatypes/complex/RbComplex.cpp \\
../../datatypes/RbInternal.cpp ../../datatypes/complex/RbString.cpp \\
../../datatypes/complex/MethodTable.cpp ../../datatypes/complex/MethodDescr.cpp \\
../../parser/FunctionTable.cpp ../../datatypes/RbUndefined.cpp \\
../../dag/RbObjectWrapper.cpp ../../dag/ContainerIterator.cpp ../../dag/DAGNodeContainer.cpp \\
../../dag/StochasticNode.cpp \\
../../distributions/Distribution.cpp \\
../../datatypes/model/RbNode.cpp ../../datatypes/model/RbUnrootedTopology.cpp \\
../../datatypes/model/RbTopology.cpp \\
../../factory/RandomNumberFactory.cpp ../../datatypes/complex/RbBitSet.cpp \\
../../datatypes/complex/IntVector.cpp ../../datatypes/complex/StringVector.cpp \\
../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qnorm.cpp \\
../../functions/distribution/RbFunction_rnorm.cpp ../../utils/RbStatistics.cpp ../../utils/RbMath.cpp ../../main/RbMonitor.cpp testDistNormal.cpp)


add_executable(testDists ../../datatypes/RbObject.cpp ../../datatypes/complex/RbComplex.cpp \\
../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../datatypes/RbUndefined.cpp ../../datatypes/RbInternal.cpp \\
../../datatypes/complex/MethodTable.cpp ../../datatypes/complex/MethodDescr.cpp \\
../../parser/FunctionTable.cpp ../../datatypes/complex/RbString.cpp \\
../../datatypes/model/RbNode.cpp ../../datatypes/model/RbUnrootedTopology.cpp \\
../../datatypes/model/RbTopology.cpp ../../dag/StochasticNode.cpp \\
../../factory/RandomNumberFactory.cpp ../../datatypes/complex/RbBitSet.cpp \\
../../datatypes/complex/IntVector.cpp ../../datatypes/complex/StringVector.cpp \\
../../distributions/Distribution.cpp \\
../../dag/RbObjectWrapper.cpp ../../dag/ContainerIterator.cpp ../../dag/DAGNodeContainer.cpp \\
../../utils/RbStatistics.cpp ../../utils/RbMath.cpp \\
../../moves/objects/RbMove.cpp ../../moves/RbMoveSchedule.cpp ../../utils/RandomNumberGenerator.cpp \\
../../functions/distribution/RbFunction_dexp.cpp ../../functions/distribution/RbFunction_pexp.cpp ../../functions/distribution/RbFunction_qexp.cpp ../../functions/distribution/RbFunction_rexp.cpp \\
../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qnorm.cpp ../../functions/distribution/RbFunction_rnorm.cpp \\
../../main/RbMonitor.cpp testDists.cpp)
