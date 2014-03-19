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
extern "C" {
#include "../libs/linenoise/linenoise.h" //https://github.com/tadmarshall/linenoise
}

/* @return 
 *      all functions the user has access to */
std::vector<std::string> getFunctionTable(bool print) {
    typedef std::multimap<std::string, RevLanguage::Function*> FunctionMap;
    typedef std::vector<std::string> StringVector;    
 
    FunctionMap functionsMap = RevLanguage::Workspace::userWorkspace().getFunctionTable().getTableCopy(true);
    
    StringVector functions;
    for (FunctionMap::iterator it = functionsMap.begin(); it != functionsMap.end(); ++it) {
        functions.push_back(it->first);
    }
    
    StringVector result;
    for (StringVector::iterator it = functions.begin(); it < std::unique(functions.begin(), functions.end()); it++) {
        if (print) {
            std::cout << *it << std::endl;
        }
        result.push_back(*it);
    }
    return result;
}

// tab completion callback
// todo: add more sophisticated options, for now only function names are listed
void completion(const char *buf, linenoiseCompletions *lc) {
    std::vector<std::string> functions = getFunctionTable(false);
    int startpos = 0;
    int matchlen = std::strlen(buf + startpos);
    
    for (unsigned int i = 0; i < functions.size(); i++) {
        if (strncasecmp(buf + startpos, functions[i].c_str(), matchlen) == 0) {
            linenoiseAddCompletion(lc, functions[i].c_str());
        }
    }
}

int main(int argc, const char * argv[]) {

    /* initialize environment */
    RevLanguageMain rl;
    rl.startRevLanguageEnvironment(argc, argv);

    /* Declare things we need */
    char *default_prompt = (char *) "RevBayes > ";
    char *incomplete_prompt = (char *) "RevBayes + ";
    char *prompt = default_prompt;
    int result = 0;
    std::string commandLine;

#if defined (USE_LIB_LINENOISE)

    char *line;

    /* Set the tab completion callback.*/
    linenoiseSetCompletionCallback(completion);

     /* Load the history at startup */
    linenoiseHistoryLoad("history.txt");

    /* The call to linenoise() will block as long as the user types something
     * and presses enter.
     * The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */


    while ((line = linenoise(prompt)) != NULL) {

        linenoiseHistoryAdd(line); /* Add to the history. */
        linenoiseHistorySave("history.txt"); /* Save the history on disk. */

        //printf("echo: '%s'\n", line);
        //printf("\r\n  \x1b[1m%s\x1b[0m \x1b[90m%s\x1b[0m\r\n", "bold", "text");

        if (result == 0 || result == 2) {
            prompt = default_prompt;
            commandLine = line;
        } else if (result == 1) {
            prompt = incomplete_prompt;
            commandLine += line;
        }

        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());

        free(line);
    }
#else

    std::string line;

    for (;;) {
        
        std::cout << prompt;
        std::istream& retStream = getline( std::cin, line );

        if ( !retStream )
            exit( 0 );
 
        if (result == 0 || result == 2) {
            prompt = default_prompt;
            commandLine = line;
        } else if (result == 1) {
            prompt = incomplete_prompt;
            commandLine += line;
        }
        
        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
    }
    
#endif

    return 0;


}

