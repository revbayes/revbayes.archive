//#include "Func_source.h"
#include "Help.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "UserInterface.h"
#include <string>
#import <Cocoa/Cocoa.h>



int main(int argc, char *argv[]) {

    RbSettings::userSettings().initializeUserSettings();
    Help::getHelp()->initializeHelp("~/help/");
    Workspace::globalWorkspace()->initializeGlobalWorkspace();

    return NSApplicationMain( argc, (const char **) argv );
}
