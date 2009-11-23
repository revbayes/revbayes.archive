### add test for distributions

### normal distribution
add_executable(testDistNormal ../../utils/RbMath.cpp ../../utils/RbStatistics.cpp ../../main/RbObject.cpp ../../datatypes/primitive/RbDouble.cpp ../../distributions/Distribution.cpp ../../distributions/statistics/DistNormal.cpp testDistNormal.cpp)
