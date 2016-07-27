#define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0

#include "RevLanguageMain.h"
#include "Parser.h"
#include "Workspace.h"
#include <string>
#import <Cocoa/Cocoa.h>



int main(int argc, char *argv[]) {

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    int argIndex = 1;
    while (argIndex < argc)
        {
        sourceFiles.push_back(std::string(argv[argIndex++]));
        }
    
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);

//    return NSApplicationMain( 0, (const char **) argv );
    return NSApplicationMain( argc, (const char **) argv );
}
