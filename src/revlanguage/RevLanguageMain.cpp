/**
 * @file
 * Test parser
 *
 */

#include "ModuleSystem.h"
#include "RevLanguageMain.h"
#include "Parser.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "RlUserInterface.h"
#include "RbVersion.h"
#include <iostream>
#include <string>
#include <cstdlib>

RevLanguageMain::RevLanguageMain(void)
{

}


void RevLanguageMain::startRevLanguageEnvironment(std::vector<std::string> sourceFiles)
{
    
    // load the modules
    RevLanguage::ModuleSystem::getModuleSystem().loadModules("modules");

    // Print a nifty message
    RbVersion version = RbVersion();
    RevLanguage::UserInterface::userInterface().output(version.getHeader(), false);
    RevLanguage::UserInterface::userInterface().output("", false);
    
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();

#if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    RevLanguage::Workspace::globalWorkspace().printValue(std::cerr);
    std::cerr << std::endl;
#endif

    // process the command line arguments as source file names    
    std::string line;
    std::string commandLine;
    int result = 0;

    for (unsigned int i =0 ; i < sourceFiles.size(); i++)
    {
        line = "source(\"" + sourceFiles[i] + "\")";
        std::cout << "> " << line << std::endl;

        // Process the command line
        if (result == 1)
            commandLine += line;
        else
            commandLine = line;
        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());

        // We just hope for better input next time
        if (result == 2)
        {
            result = 0;
        }
    }
}


