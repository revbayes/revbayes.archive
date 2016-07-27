#define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0

#include "RevLanguageMain.h"
#include "Parser.h"
#include "Workspace.h"
#include <string>
#import <Cocoa/Cocoa.h>



int main(int argc, char *argv[]) {

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);

    return NSApplicationMain( argc, (const char **) argv );
}
