#include "RbClient.h"
#include "Parser.h"
#include "Workspace.h"
#include "WorkspaceUtils.h"
#include "TerminalFormatter.h"


#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo> 
#include <sstream>
#include  <iomanip>

extern "C" {
#include "linenoise.h"
}
#include "EditorMachine.h"
#include "EditorState.h"
#include "lineeditUtils.h"
#include "EditorMachineObserver.h"
#include "RepoClient.h"
#include "IRepoObserver.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp> 
#include "boost/algorithm/string_regex.hpp"
#include <boost/lexical_cast.hpp>


#define ctrl(C) ((C) - '@')
typedef std::vector<std::string> StringVector;

bool debug = false;

const char* nl = (char*) "\n\r";
const char* default_prompt = (char*) "RevBayes > ";
const char* incomplete_prompt = (char*) "RevBayes + ";
const char* esc_prompt = (char*) "? > ";
const char* prompt = default_prompt;
char *line;

EditorMachine em;
WorkspaceUtils wu;
Options *opt;
Configuration *config;

/**
 * Print an arbitrary number of columns to stdout.
 * 
 * @param v             Data to print
 * @param columnWidth   Width for each column
 * @param indentation   Number of spaces in beginning of line
 * @param columns       Number of columns to organize
 */
void printData(StringVector v, int columns, int columnWidth = 20, int indentation = 4) {
    int i = 0;

    std::cout.flush();
    linenoiceSetCursorPos(0);

    BOOST_FOREACH(std::string s, v) {
        linenoiceSetCursorPos((i++ % columns) * columnWidth + indentation);
        std::cout << s;
        if (i % columns == 0) {
            std::cout << nl;
        }
        std::cout.flush();
    }

    linenoiceSetCursorPos(0);
    std::cout.flush();
}

/**
 * General guidance how to use the terminal
 */
void printGeneralHelp() {
    linenoiseClearScreen();
    linenoiceSetCursorPos(0);

    std::cout << nl << nl << "Type ?" << TerminalFormatter::makeBold("<function or object name>") << " to display help of the object" << nl;

    StringVector cmd;
    cmd.push_back("?");
    cmd.push_back("Print this screen");
    cmd.push_back("clr");
    cmd.push_back("Clear screen");
    cmd.push_back("");
    cmd.push_back("");
    cmd.push_back("(ESC)");
    cmd.push_back("Show a list over available completions");
    cmd.push_back("(TAB)");
    cmd.push_back("Auto complete command");
    cmd.push_back("");
    cmd.push_back("");
    cmd.push_back("UP / DOWN arrows");
    cmd.push_back("Browse command history");
    cmd.push_back("(Ctrl+R)");
    cmd.push_back("Recursive search of previous commands");
    cmd.push_back("");
    cmd.push_back("");
    cmd.push_back("repo-list");
    cmd.push_back("Show files in web repositories");
    cmd.push_back("repo-get <int>");
    cmd.push_back("Download a file with the given index number");
    cmd.push_back("");
    cmd.push_back("");
    cmd.push_back("(Ctrl+A)");
    cmd.push_back("Go to beginning of line");
    cmd.push_back("(Ctrl+E)");
    cmd.push_back("Go to end of line");
    cmd.push_back("(Ctrl+W)");
    cmd.push_back("Delete one word to the left");
    cmd.push_back("...");
    cmd.push_back("");



    std::cout << nl << TerminalFormatter::makeUnderlined("Available commands") << nl;
    printData(cmd, 2);

}

/**
 * Basically everything
 */
void setDefaultCompletions() {

    BOOST_FOREACH(std::string function, wu.getFunctions()) {
        em.getCurrentState()->addCompletion(function);
    }

    BOOST_FOREACH(std::string obj, wu.getObjects(true)) {
        em.getCurrentState()->addCompletion(obj);
    }
}

/**
 * Retain arguments in tab complete but try to remove already used ones.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */

int listSeparatorCallback(const char *buf, size_t len, char c) {

    if (em.processInput(buf)) {

        // shouldn't be here if not previous state was defining a list
        EditorState *func = em.getStateQueue()->at(em.getStateQueue()->size() - 2);
        std::string usedArgument = em.getCurrentState()->getSubject();

        // pop the used argument from completions        
        StringVector v;

        BOOST_FOREACH(std::string s, func->getCompletions()) {
            if (s != usedArgument) {
                v.push_back(s);
            }
        }
        func->setCompletions(v);

        // use pruned completions also in current state
        em.getCurrentState()->setCompletions(v);

        // restoration of pruned arguments is handled in eventChange callback
    }
    return 0;
}

/**
 * callback for '.'
 * 
 * List object members
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int dotCallback(const char *buf, size_t len, char c) {

    if (em.processInput(buf)) {
        std::string subject = em.getCurrentState()->getSubject();
        if (wu.isType(subject)) {
            StringVector sv = wu.getTypeMembers(subject);

            BOOST_FOREACH(std::string member, sv) {
                em.getCurrentState()->addCompletion(member);
            }
        }
    }
    return 0;
}

/**
 * callback for '(' and ')' 
 * 
 * Add arguments to tab complete if the function or method is known.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int bracketCallback(const char *buf, size_t len, char c) {
    std::string type = "";
    // query current state before switching to new
    if (em.getCurrentState()->getType() == ST_ACCESSING_MEMBER) {
        type = em.getCurrentState()->getSubject();
    }
    if (em.processInput(buf)) {
        std::string subject = em.getCurrentState()->getSubject();
        // add arguments to completions
        if (type == "") { // ordinary function               

            BOOST_FOREACH(std::string param, wu.getFunctionParameters(subject)) {
                em.getCurrentState()->addCompletion(param);
            }
        } else { // member method

            BOOST_FOREACH(std::string param, wu.getMethodParameters(type, subject)) {
                em.getCurrentState()->addCompletion(param);
            }
        }
    }
    return 0;
}

/**
 * Callback for '='
 * 
 * noop
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */

int assignCallback(const char *buf, size_t len, char c) {
    //setDefaultCompletions();    
    return 0;
}

/**
 * callback for '"' 
 * 
 * Scan include directories for .rev files an make them available in tab complete.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int quotationCallback(const char *buf, size_t len, char c) {

    if (em.processInput(buf)) {

        BOOST_FOREACH(std::string s, Filesystem::getFileList(opt->getIncludePaths(), ".Rev")) {
            em.getCurrentState()->addCompletion(s);
        }
    }
    return 0;
}

/**
 * Show information about environment, completions etc...
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int escapeCallback(const char *buf, size_t len, char c) {

    linenoiseClearScreen();

    // general info
    if (len <= 0) {
        printGeneralHelp();
    }

    // completions
    linenoiceSetCursorPos(0);
    std::cout << nl << TerminalFormatter::makeUnderlined("Available completions") << nl;

    unsigned int startPos = em.getLinePos(); // place in buffer to start match    
    while (buf[startPos] == ' ') { // discard extra spaces
        startPos++;
    }

    StringVector v;

    BOOST_FOREACH(std::string comp, em.getCurrentState()->getCompletions()) {
        if (boost::starts_with(comp, (buf + startPos))) {
            v.push_back(comp);
        }
    }
    printData(v, 2, 36);

    // debug
    if (debug) {
        std::cout << nl << TerminalFormatter::makeUnderlined("Editor state") << nl;

        std::cout << em.getMessage() << nl;


        std::cout << nl << TerminalFormatter::makeUnderlined("Debug info") << nl;

        StringVector dv;

        dv.push_back("em-linePos");
        dv.push_back(boost::lexical_cast<std::string>(em.getLinePos()));
        dv.push_back("comp match");
        dv.push_back(buf + startPos);

        printData(dv, 2);

    }

    // restore prompt
    linenoiceSetCursorPos(0);
    std::cout << nl << nl << prompt << buf;
    std::cout.flush();

    return 0;
}

/**
 * callback for back space
 * 
 * Notify state machine that a char was deleted.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int backspaceCallback(const char *buf, size_t len, char c) {
    em.deleteChar(buf);
    return 0;
}

/**
 * tab completion callback
 * 
 * Update list of available completions.
 * 
 * @param buf
 * @param lc
 */
void completion(const char *buf, linenoiseCompletions *lc) {

    unsigned int startPos = em.getLinePos();

    // discard extra spaces
    while (buf[startPos] == ' ') {
        startPos++;
    }

    //pad previous complete commands
    std::string newBuffer;
    for (unsigned int i = 0; i < startPos; i++) {
        newBuffer += buf[i];
    }

    StringVector completions = em.getCurrentState()->getCompletions();

    for (unsigned int i = 0; i < completions.size(); i++) {
        if (boost::starts_with(completions[i], (buf + startPos))) {
            linenoiseAddCompletion(lc, (newBuffer + completions[i]).c_str());
        }
    }
}

/**
 * Main application loop.
 * 
 * @param help
 * @param options
 * @param configuration
 */
void RbClient::startInterpretor(IHelp *help, Options *options, Configuration *configuration) {

    em.setObserver(this);

    opt = options;
    config = configuration;
    RepoClient repoClient;
    repoClient.setObserver(this);

    /* Set tab completion callback */
    linenoiseSetCompletionCallback(completion);

    /* Set character callback functions*/
    linenoiseSetCharacterCallback(dotCallback, '.');
    linenoiseSetCharacterCallback(bracketCallback, ')');
    linenoiseSetCharacterCallback(bracketCallback, '(');
    linenoiseSetCharacterCallback(assignCallback, '=');
    linenoiseSetCharacterCallback(quotationCallback, '"');
    linenoiseSetCharacterCallback(listSeparatorCallback, ',');

    linenoiseSetCharacterCallback(escapeCallback, 27);
    linenoiseSetCharacterCallback(backspaceCallback, ctrl('H')); // works in netbeans pseudo terminal
    linenoiseSetCharacterCallback(backspaceCallback, 0x7f); // works in gnome terminal


    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */


    em.reset();
    setDefaultCompletions();
    int result = 0;
    std::string commandLine;
    std::string cmd;

    while ((line = linenoise(prompt)) != NULL) {

        linenoiseHistoryAdd(line); /* Add to the history. */
        linenoiseHistorySave("history.txt"); /* Save the history on disk. */

        prompt = default_prompt;
        cmd = line;
        boost::trim(cmd);

        if (cmd == "clr") {
            linenoiseClearScreen();
        } else if (cmd == "?") {
            printGeneralHelp();
        } else if (cmd == "debug=false") {
            debug = false;
        } else if (cmd == "debug=true") {
            debug = true;
        } else if (repoClient.processCommand(config->getRepositories(), cmd)) {

        } else {
            // interpret Rev statement
            if (result == 0 || result == 2) {
                commandLine = cmd;
                em.reset();

            } else if (result == 1) {
                prompt = incomplete_prompt;
                commandLine += cmd;
            }
            result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
        }

        /* The typed string is returned as a malloc() allocated string by
         * linenoise, so the user needs to free() it. */
        free(line);
    }
}

/**
 * This method is fired by EditorStateMachine whenever state changes
 */
void RbClient::eventStateChanged(EditorState *state, EditorStateChangeType type) {

    // restore pruned arguments
    if (state->getType() == ST_DEF_ARGUMENT && type == STATE_CANCELLED) {
        em.getCurrentState()->addCompletion(state->getSubject());
    }
}

/**
 * fired by repo client when some error occurs
 * @param error
 */
void RbClient::notifyError(std::string error) {
    std::cout << nl << "Repository Client Error: " << error << nl;
}

/**
 * fired by repo client when some data is available
 */
void RbClient::notifyGetIndexComplete(HttpResponse httpResponse, RepositoryInfo revRepository) {

    if (httpResponse.code != 200 || httpResponse.exception.size() > 0) {
        std::cout << nl << "The request to fetch index from '" << revRepository.GetName() << "' failed: \n" << httpResponse.exception << nl;

    } else {
        linenoiseClearScreen();
        std::cout << nl << TerminalFormatter::makeUnderlined("File index of '" + revRepository.GetName() + "'") << nl;

        for (int i = 0; i < httpResponse.data.size() - 1; i += 2) {
            std::cout << std::setw(8) << httpResponse.data.at(i) << httpResponse.data.at(i + 1) << nl;
        }
    }
    std::cout << nl;
}

/**
 * fired by repo client when some data is available
 */
void RbClient::notifyGetFileComplete(HttpResponse httpResponse, RepositoryInfo revRepository) {

    std::cout << nl;
    if (httpResponse.code != 200 || httpResponse.exception.size() > 0) {
        std::cout << "The request to fetch file '" << httpResponse.data.at(0) << "' from '" << revRepository.GetName() << "' failed: \n" << httpResponse.exception << nl;

    } else {
        std::cout << nl << nl << TerminalFormatter::makeUnderlined("Content of '" + httpResponse.data.at(0) + "'") << nl << nl;
        std::string tmp = "    " + boost::regex_replace(httpResponse.data.at(1), boost::regex("\n"), "\n    ");
        std::cout << tmp << nl << nl;

        std::string filename = config->getDownloadDir() + Filesystem::directorySeparator() + httpResponse.data.at(0);
        std::cout << "Save file to '" + filename + "' Y/N ?:";
        char ch;
        std::cin.get(ch);
        if (ch == 'y' || ch == 'Y') {
            if (Filesystem::saveToFile(httpResponse.data.at(0), config->getDownloadDir(), httpResponse.data.at(1))) {
                std::cout << "File saved!" << nl;
            } else {
                std::cout << "Something went wrong and the file couldn't be saved!" << nl;
            }
        }
    }
    std::cout << nl;
}