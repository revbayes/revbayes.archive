
#include "Test.h"
#include <iostream>

int main (int argc, const char * argv[])
{
    Test t = Test(argc, argv);
    bool passed = t.performTests(argc, argv);
    std::cout << "Done!!!" << std::endl;
    return (passed ? 0 : 1);
}

