#add_executable(testD ../distributions/statistics/DistNormal.cpp)
add_executable(testDistNormal ../datatypes/primary/RbDouble.cpp ../distributions/Distribution.cpp ../distributions/statistics/DistNormal.cpp testDistNormal.cpp)

ENABLE_TESTING()
ADD_TEST(testDistNormal testDistNormal)
