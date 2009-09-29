### add test for data types

### double
add_executable(testDouble ../../main/RbObject.cpp ../../utils/RbException.cpp ../../datatypes/RbAbstractDataType.cpp ../../datatypes/RbTypeInfo.cpp ../../datatypes/primitive/RbDouble.cpp testDouble.cpp)

### int
add_executable(testInt ../../main/RbObject.cpp ../../utils/RbException.cpp ../../datatypes/RbAbstractDataType.cpp ../../datatypes/RbTypeInfo.cpp ../../datatypes/primitive/RbInt.cpp ../../datatypes/primitive/RbDouble.cpp testInt.cpp)

### bool
add_executable(testBool ../../main/RbObject.cpp ../../utils/RbException.cpp ../../datatypes/RbAbstractDataType.cpp ../../datatypes/RbTypeInfo.cpp ../../datatypes/primitive/RbBool.cpp testBool.cpp)

### branch
add_executable(testBranch ../../main/RbObject.cpp ../../utils/RbException.cpp ../../datatypes/RbAbstractDataType.cpp ../../datatypes/RbTypeInfo.cpp ../../datatypes/primitive/RbDouble.cpp ../../datatypes/model/Node.cpp ../../datatypes/model/Branch.cpp testBranch.cpp)
