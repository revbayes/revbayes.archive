#include <stdio.h>
#include <string>
#include <vector>

#include "RbClient.h"
#include "RevLanguageMain.h"
#include "Parser.h"
#include "Workspace.h"

int main(int argc, const char* argv[]) {
    
    
    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    int argIndex = 1;
    while (argIndex < argc) {
        sourceFiles.push_back(std::string(argv[argIndex++]));
    }
    
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);
    
    
#ifdef RB_XCODE
    /* Declare things we need */
    int result = 0;
    std::string commandLine;
    std::string line;
        
    for (;;) {
            
        /* Print prompt based on state after previous iteration */
        if (result == 0 || result == 2)
        {
            std::cout << "> ";
        }
        else
        {
            std::cout << "+ ";
        }
        
        /* Get the line */
        std::istream& retStream = getline(std::cin, line);
            
        if (!retStream)
        {
            exit(0);
        }
        
        if (result == 0 || result == 2)
        {
            commandLine = line;
        }
        else if (result == 1)
        {
            commandLine += line;
        }
            
        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
        
    }
    
# else
    
    RbClient c;
    c.startInterpretor();
    
#endif

    return 0;

}


