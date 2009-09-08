#add_executable(testD ../distributions/DistNormal.cpp)
add_executable(testDistNormal ../datatypes/primary/RbDouble.cpp ../distributions/Distribution.cpp ../distributions/DistNormal.cpp testDistNormal.cpp)

ENABLE_TESTING()
ADD_TEST(testDistNormal testDistNormal)
