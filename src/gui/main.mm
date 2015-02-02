#include "RbClient.h"
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

    return NSApplicationMain( argc, (const char **) argv );
}
