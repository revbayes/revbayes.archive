#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>

#include "RevLanguageMain.h"
#include "RbOptions.h"
#include "Parser.h"
#include "Workspace.h"
#include "workspace/FunctionTable.h"
#include "functions/RlFunction.h"
//#include "WorkspaceUtils.h"
#include "CommandLineUtils.h"


int main(int argc, const char * argv[]) {

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    int argIndex = 1;
    while (argIndex < argc) {
        sourceFiles.push_back(std::string(argv[argIndex++]));
    }
        
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);

    /* Declare things we need */
    int result = 0;
    std::string commandLine;
    std::string line;

    for (;;) {

        /* Print prompt based on state after previous iteration */
        if (result == 0 || result == 2)
            std::cout << "> ";
        else
            std::cout << "+ ";

        /* Get the line */
        std::istream& retStream = getline(std::cin, line);

        if (!retStream)
            exit(0);

        if (result == 0 || result == 2) {
            commandLine = line;
        } else if (result == 1) {
            commandLine += line;
        }

        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
    }

    return 0;

}

