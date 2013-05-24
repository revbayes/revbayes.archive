
#include "Test.h"
#include <iostream>

int main (int argc, const char * argv[])
{
    
    Test t = Test();
    t.performTests();
    
    std::cout << "Done!!!" << std::endl;
    
    return 0;
}

