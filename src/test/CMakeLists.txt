add_executable(testDistNormal testDistNormal.cpp ..\distribution\DistNormal.cpp)

ENABLE_TESTING()
ADD_TEST(testDistNormal testDistNormal)
