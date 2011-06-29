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



int main(int argc, char **argv) {

    // Print a nifty message
	RBOUT("\n\n");
	RBOUT("                             RevBayes\n");
	RBOUT("          An R-like environment for phylogenetic analysis");
	RBOUT("                     using Bayesian inference\n");
	RBOUT("             Released under the GPL license, version 3\n\n");

    RbSettings::userSettings().initializeUserSettings();
    Help::getHelp().initializeHelp("~/help/");
    Workspace::globalWorkspace().initializeGlobalWorkspace();

    /* Add stuff that Sebastian does not want to have in the core */
    Workspace::globalWorkspace().addFunction( "source", new Func_source() ); 

#	if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    Workspace::globalWorkspace().printValue(std::cerr);
    std::cerr << std::endl;
#	endif

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
            getline(std::cin, line);
        }

        // Process the command line
        result = Parser::getParser().processCommand(line);

        // We just hope for better input next time
        if ( result == 2 ) {
            result = 0;
        }
    }

    return 0;
}


