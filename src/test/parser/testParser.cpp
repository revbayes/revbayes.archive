/**
 * @file
 * Test parser
 *
 */

#include <iostream>
#include <cmath>
#include <assert.h>
#include <vector>

#include "Argument.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Distribution.h"
#include "DistExponential.h"
#include "DistNormal.h"
#include "DistUniform.h"
#include "MoveSlidingWindow.h"
#include "MoveScale.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbMcmc.h"
#include "RbModel.h"
#include "RbMonitor.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbObject.h"
#include "RbException.h"
#include "StochasticNode.h"

int main(int argc, char **argv) {

    // Print a nifty message
    std::cout << "Welcome to RevBayes" << std::endl;
    std::cout << std::endl;

    // Create command line variable
    std::string commandLine;

    // Process input; exit is managed by Parser
    for (;;) {

        std::string line;

        // Read the command
        std::cout << "RevBayes >";
        std::cin >> line;
        commandLine += line;

        // Process the command line

        // If incomplete statement, then delete end comment and newline
        // else clear commandLine
        
    }

    return 0;
}
