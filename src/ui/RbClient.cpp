
#include "RbClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo> 

extern "C" {
#include "linenoise.h"
}
#include "lineedit/EditorMachine.h"
#include "lineedit/EditorState.h"
#include "lineedit/lineeditUtils.h"

#define ctrl(C) ((C) - '@')

bool debug = true;

const char* default_prompt = (char*) "RevBayes > ";
const char* incomplete_prompt = (char*) "RevBayes + ";
const char* esc_prompt = (char*) "> ";
const char* prompt = default_prompt;

struct Settings {
    std::string newFilename;

};

Settings settings;

char *line; // what is entered at command line

typedef std::vector<std::string> StringVector;


/* callback for 'ESC' */
int escapeCallback(const char *buf, size_t len, char c) {

    

    fflush(stdout);

    return 0;

}

/* callback for 'Ctrl + S' */
int saveCallback(const char *buf, size_t len, char c) {

    printf("\r\n%s\r\n", "in save callback");

    prompt = default_prompt;

    return 0;

}

/* tab completion callback */

void completion(const char *buf, linenoiseCompletions *lc) {
    
}

void RbClient::startInterpretor() {
    
    /* Set callback functions*/
    linenoiseSetCompletionCallback(completion);
    //    linenoiseSetCharacterCallback(dotCallback, '.');
    //    linenoiseSetCharacterCallback(bracketCallback, '(');
    //    linenoiseSetCharacterCallback(bracketCallback, ')');
    //    linenoiseSetCharacterCallback(assignCallback, '=');
    //    linenoiseSetCharacterCallback(stringCallback, '"');
    //    linenoiseSetCharacterCallback(saveCallback, ctrl('S'));
    linenoiseSetCharacterCallback(escapeCallback, 27);


    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */


    /* The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */


    while ((line = linenoise(prompt)) != NULL) {

        linenoiseHistoryAdd(line); /* Add to the history. */
        linenoiseHistorySave("history.txt"); /* Save the history on disk. */

        

        
        free(line);
    }
}