#add_executable(testD ../distributions/statistics/DistNormal.cpp)
add_executable(testBranch ../main/RbObject.cpp ../datatypes/primary/RbDouble.cpp ../datatypes/model/Node.cpp ../datatypes/model/Branch.cpp testBranch.cpp)
add_executable(testDistNormal ../utils/RbMath.cpp ../utils/RbStatistics.cpp ../main/RbObject.cpp ../datatypes/primary/RbDouble.cpp ../distributions/Distribution.cpp ../distributions/statistics/DistNormal.cpp testDistNormal.cpp)

ENABLE_TESTING()
ADD_TEST(testDistNormal testDistNormal)
