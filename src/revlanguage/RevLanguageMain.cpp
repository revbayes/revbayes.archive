/**
 * @file
 * Test parser
 *
 */

#include "RevLanguageMain.h"
#include "Help.h"
#include "Parser.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "RlUserInterface.h"
#include "RbVersion.h"
#include <iostream>
#include <string>
#include <cstdlib>

RevLanguageMain::RevLanguageMain() {

}

int RevLanguageMain::startRevLanguageEnvironment(const int argc, const char* argv[]) {

    // Print a nifty message
    RbVersion version = RbVersion();
    RevLanguage::UserInterface::userInterface().output(version.getHeader(), false);
    RevLanguage::UserInterface::userInterface().output("", false);

    // Initialize help singleton
    Help::getHelp().initializeHelp();
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();

    // Print an extra line to separate prompt from possible output from help call
    RevLanguage::UserInterface::userInterface().output("\n");

#if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    RevLanguage::Workspace::globalWorkspace().printValue(std::cerr);
    std::cerr << std::endl;
#endif

    // process the command line arguments as source file names
    int argIndex = 1;
    std::string line;
    std::string commandLine;
    int result = 0;

    while (argIndex < argc) {
        line = "source(\"" + std::string(argv[argIndex++]) + "\")";
        std::cout << "RevBayes > " << line << std::endl;

        // Process the command line
        if (result == 1)
            commandLine += line;
        else
            commandLine = line;
        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());

        // We just hope for better input next time
        if (result == 2) {
            result = 0;
        }
    }

    return 0;
}


