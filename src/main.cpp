
#include "RevLanguageMain.h"
#include "RbOptions.h"
#include "Test.h"
#include <iostream>

int main (int argc, const char * argv[])
{

#ifdef TESTING
    Test t = Test();
    t.performTests();
#else
    RevLanguageMain rl;
    rl.startRevLanguageEnvironment();
#endif
    
    std::cout << "Done!!!" << std::endl;
    
    return 0;
}

