### add test for distributions

### normal distribution
add_executable(testDistNormal ../../datatypes/RbObject.cpp ../../datatypes/complex/StringVector.cpp ../../datatypes/complex/RbComplex.cpp \\
../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp ../../datatypes/primitive/RbPrimitive.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qnorm.cpp \\
../../functions/distribution/RbFunction_rnorm.cpp ../../utils/RbStatistics.cpp ../../utils/RbMath.cpp testDistNormal.cpp)


add_executable(testDists ../../datatypes/RbObject.cpp ../../datatypes/complex/StringVector.cpp ../../datatypes/complex/RbComplex.cpp \\
../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp ../../datatypes/primitive/RbPrimitive.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../utils/RbStatistics.cpp ../../utils/RbMath.cpp \\
../../functions/distribution/RbFunction_dbeta.cpp ../../functions/distribution/RbFunction_pbeta.cpp ../../functions/distribution/RbFunction_qbeta.cpp ../../functions/distribution/RbFunction_rbeta.cpp \\
../../functions/distribution/RbFunction_dchisq.cpp ../../functions/distribution/RbFunction_pchisq.cpp ../../functions/distribution/RbFunction_qchisq.cpp ../../functions/distribution/RbFunction_rchisq.cpp \\
../../functions/distribution/RbFunction_dexp.cpp ../../functions/distribution/RbFunction_pexp.cpp ../../functions/distribution/RbFunction_qexp.cpp ../../functions/distribution/RbFunction_rexp.cpp \\
../../functions/distribution/RbFunction_dnorm.cpp ../../functions/distribution/RbFunction_pnorm.cpp ../../functions/distribution/RbFunction_qnorm.cpp ../../functions/distribution/RbFunction_rnorm.cpp \\
testDists.cpp)