### add test for parser

### normal parser
add_executable(testParser \\
../../parser/ObjectSlot.cpp \\
../../parser/Frame.cpp \\
../../parser/FunctionTable.cpp \\
../../parser/Workspace.cpp \\
../../parser/SyntaxAssignExpr.cpp \\
../../parser/SyntaxBinaryExpr.cpp \\
../../parser/SyntaxClassDef.cpp \\
../../parser/SyntaxConstant.cpp \\
../../parser/SyntaxForCondition.cpp \\
../../parser/SyntaxFormal.cpp \\
../../parser/SyntaxFunctionCall.cpp \\
../../parser/SyntaxFunctionDef.cpp \\
../../parser/SyntaxLabeledExpr.cpp \\
../../parser/SyntaxStatement.cpp \\
../../parser/SyntaxUnaryExpr.cpp \\
../../parser/SyntaxVariable.cpp \\
../../datatypes/RbObject.cpp \\
../../datatypes/RbInternal.cpp \\
../../datatypes/RbUndefined.cpp \\
../../datatypes/primitive/RbPrimitive.cpp \\
../../datatypes/primitive/RbDouble.cpp \\
../../datatypes/primitive/RbInt.cpp \\
../../datatypes/primitive/RbBool.cpp \\
../../datatypes/complex/RbComplex.cpp \\
../../datatypes/complex/MethodDescr.cpp \\
../../datatypes/complex/MethodTable.cpp \\
../../datatypes/complex/StringVector.cpp \\
../../datatypes/complex/IntVector.cpp \\
../../datatypes/complex/RbBitset.cpp \\
../../functions/Argument.cpp \\
../../functions/ArgumentRule.cpp \\
../../functions/Ellipsis.cpp \\
../../functions/RbFunction.cpp \\
../../functions/UserFunction.cpp \\
../../dag/RbObjectWrapper.cpp \\
../../dag/DAGNodeContainer.cpp \\
../../dag/ContainerIterator.cpp \\
../../dag/DAGNode.cpp  \\
../../dag/ConstantNode.cpp \\
../../dag/StochasticNode.cpp \\
../../dag/DeterministicNode.cpp \\
../../utils/RbException.cpp \\
../../utils/RbStatistics.cpp \\
../../utils/RbNames.h \\
../../utils/RbMath.cpp \\
../../moves/objects/MoveSlidingWindow.cpp \\
../../moves/objects/RbMove.cpp \\
../../moves/RbMoveSchedule.cpp \\
../../moves/objects/MoveScale.cpp
../../distributions/Distribution.cpp
../../distributions/statistics/DistExponential.cpp
../../distributions/statistics/DistNormal.cpp \\
../../distributions/statistics/DistUniform.cpp \\
../../main/RbMonitor.cpp \\
../../inference/mcmc/RbMcmc.cpp \\
testParser.cpp)
