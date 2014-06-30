//#include "Func_source.h"
#include "Help.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "UserInterface.h"
#include <string>
#import <Cocoa/Cocoa.h>

#include "RevLanguageMain.h"



int main(int argc, char *argv[]) {

    /* old initialization
    RbSettings::userSettings().initializeUserSettings();
    Help::getHelp().initializeHelp("~/help/");
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();*/

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    
    /* initialize environment */
    Help help;
    
    RevLanguageMain rl(&help);
    rl.startRevLanguageEnvironment(sourceFiles);

    return NSApplicationMain( argc, (const char **) argv );
}
