/**
 * @file
 * Test parser
 *
 */

#include "Func_source.h"
#include "Help.h"
#include "Parser.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "UserInterface.h"
#include <iostream>
#include <string>
#include <cstdlib>


int main(int argc, char **argv) {

    // Print a nifty message
    UserInterface::userInterface().output("\n\n", false);
    UserInterface::userInterface().output("                             RevBayes\n", false);
    UserInterface::userInterface().output("          An R-like environment for phylogenetic analysis", false);
    UserInterface::userInterface().output("                     using Bayesian inference\n", false);
    UserInterface::userInterface().output("             Released under the GPL license, version 3\n\n", false);

    RbSettings::userSettings().initializeUserSettings();
    Help::getHelp()->initializeHelp("~/help/");
    Workspace::globalWorkspace()->initializeGlobalWorkspace();

    /* Add stuff that Sebastian does not want to have in the core */
    Workspace::globalWorkspace()->addFunction( "source", RbPtr<RbFunction>( new Func_source() ) );

#	if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    Workspace::globalWorkspace()->printValue(std::cerr);
    std::cerr << std::endl;
#	endif

    // Print an extra line to separate prompt from possible output from help call
    UserInterface::userInterface().output("\n");

    // Create command line variable
    std::string commandLine;

    // Process input; exit is managed by Parser
    int argIndex = 1;
    int result = 0;
    for (;;) {

        std::string line;

        // Read the command
        if (argIndex < argc) {
            line = "source(\"" + std::string(argv[argIndex++]) + "\")";
            std::cout << "RevBayes > " << line << std::endl;
		}
        else {
            if (result == 0)
                std::cout << "RevBayes > ";
            else /* if (result == 1) */
                std::cout << "RevBayes + ";
            std::istream& retstream = getline(std::cin, line);
            if (!retstream) {
                // line = "q()";
                exit(0);
            }
        }

        // Process the command line
        if ( result == 1 )
            commandLine += line;
        else
            commandLine = line;
        result = Parser::getParser().processCommand(commandLine);

        // We just hope for better input next time
        if ( result == 2 ) {
            result = 0;
        }
    }

    return 0;
}


