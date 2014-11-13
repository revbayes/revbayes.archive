//#include "Func_source.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "UserInterface.h"
#include <string>
#import <Cocoa/Cocoa.h>

#include "RevLanguageMain.h"



int main(int argc, char *argv[]) {

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);

    return NSApplicationMain( argc, (const char **) argv );
}
