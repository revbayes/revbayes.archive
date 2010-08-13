/**
 * @file
 * Test parser
 *
 */

#include <iostream>

#include "Parser.h"
#include "Workspace.h"

int main(int argc, char **argv) {

    // Print a nifty message
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "                            RevBayes" << std::endl;
    std::cout << std::endl;
    std::cout << "          An R-like environment for evolutionary analysis" << std::endl;
    std::cout << "                     using Bayesian inference" << std::endl;
    std::cout << std::endl;
    std::cout << "             Released under the GPL license, version 3" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    Workspace::globalWorkspace().initializeGlobalWorkspace();

#if defined DEBUG_PARSER
    std::cerr << "Global workspace after initialization:" << std::endl;
    Workspace::globalWorkspace().printValue(std::cerr);
    std::cerr << std::endl;
#endif

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
            if (result != 1)
                std::cout << "RevBayes > ";
            else if (result == 1)
                std::cout << "RevBayes + ";
            getline(std::cin, line);
        }

        // Process the command line
        result = Parser::getParser().processCommand(line);
    }

    return 0;
}


