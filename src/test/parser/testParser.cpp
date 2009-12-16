/**
 * @file
 * Test parser
 *
 */

#include <iostream>

#include "Parser.h"

int main(int argc, char **argv) {

    // Print a nifty message
    std::cout << std::endl;
    std::cout << "RevBayes: An R-like environment for evolutionary analysis using Bayesian inference" << std::endl;
    std::cout << std::endl;
    std::cout << "Released under the GPL license, version 3" << std::endl;
    std::cout << std::endl;

    // Create command line variable
    std::string commandLine;

    // Process input; exit is managed by Parser
    for (;;) {

        std::string line;

        // Read the command
        std::cout << "RevBayes > ";
        getline(std::cin, line);

        // Process the command line
        Parser::getParser().processCommand(line);
    }

    return 0;
}


