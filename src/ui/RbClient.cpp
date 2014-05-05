
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

enum modeOption {
    MODE_DEFAULT, MODE_CHOOSE, MODE_NEW_FILE, MODE_EDIT_FILE
};
modeOption mode = MODE_DEFAULT;

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

    if (mode == MODE_DEFAULT) {
        mode = MODE_CHOOSE;


        printf("\n\rEnter your choise, ESC again to exit this menu:\n\r%s", esc_prompt);
        printf("\n\r'n' : Create a new file\n\r%s", esc_prompt);


        char c = getchar();
        switch (c) {
            case 'n':
                mode = MODE_NEW_FILE;
                printf("\n\rPlease enter the name of new file: ");
        }

    } else {
        if (mode == MODE_EDIT_FILE) {
            fprintf(stdout, "\n\rSave: y/n\n\r");
            if (tolower(getchar()) == 'y') {
                fprintf(stdout, "Saving file...");
            } else {
                fprintf(stdout, "Discarding file ...");
            }
        }

    }

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

        if (mode == MODE_NEW_FILE) {
            settings.newFilename = line;
            printf("\n\rNew file name = %s\n\r", line);
            fflush(stdout);
            mode = MODE_EDIT_FILE;
        }

        
        free(line);
    }
}