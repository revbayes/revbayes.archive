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
#include "WorkspaceUtils.h"
#include "CommandLineUtils.h"


int main(int argc, const char * argv[]) {

    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    int argIndex = 1;
    while (argIndex < argc) {
        sourceFiles.push_back(std::string(argv[argIndex++]));
    }
        
    /* initialize environment */
    RevLanguageMain rl;
    rl.startRevLanguageEnvironment(sourceFiles);

    /* Declare things we need */
    char *default_prompt = (char *) "RevBayes > ";
    char *incomplete_prompt = (char *) "RevBayes + ";
    char *prompt = default_prompt;
    int result = 0;
    std::string commandLine;
    std::string line;

    for (;;) {

        std::cout << prompt;
        std::istream& retStream = getline(std::cin, line);

        if (!retStream)
            exit(0);

        if (result == 0 || result == 2) {
            prompt = default_prompt;
            commandLine = line;
        } else if (result == 1) {
            prompt = incomplete_prompt;
            commandLine += line;
        }

        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
    }

    return 0;

}

