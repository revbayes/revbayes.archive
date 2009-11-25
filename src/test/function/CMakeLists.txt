### add test for functions

### sqrt
add_executable(testSqrt ../../datatypes/RbObject.cpp ../../datatypes/complex/StringVector.cpp ../../datatypes/complex/RbComplex.cpp \\
../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp ../../datatypes/primitive/RbPrimitive.cpp \\
../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
../../functions/math/RbFunction_sqrt.cpp testSqrt.cpp)

### functions
#add_executable(testFuncs ../../datatypes/RbObject.cpp ../../datatypes/complex/StringVector.cpp ../../datatypes/complex/RbComplex.cpp \\
#../../utils/RbException.cpp ../../dag/ConstantNode.cpp ../../dag/DAGNode.cpp ../../datatypes/primitive/RbPrimitive.cpp \\
#../../datatypes/primitive/RbDouble.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbBool.cpp \\
#../../functions/Argument.cpp ../../functions/ArgumentRule.cpp ../../functions/RbFunction.cpp \\
#../../functions/math/RbFunction_sqrt.cpp ../../functions/math/RbFunction_ln.cpp ../../functions/math/RbFunction_pow.cpp testFuncs.cpp)

