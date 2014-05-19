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

extern "C" {
#include "linenoise.h"
}
#include "EditorMachine.h"
#include "EditorState.h"
#include "lineeditUtils.h"
#include "EditorMachineObserver.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
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

int pseudoPos = 0;

EditorMachine em;
WorkspaceUtils wu;
Options opt;
Configuration config;

/**
 * Print an arbitrary number of columns to stdout.
 * 
 * @param v             Data to print
 * @param columnWidth   Width for each column
 * @param indentation   Number of spaces in beginning of line
 * @param columns       Number of columns to organize
 */
void printColumns(StringVector v, int columnWidth = 20, int indentation = 4, int columns = 2) {
    int i = 0;
    std::cout.flush();

    BOOST_FOREACH(std::string s, v) {
        linenoiceSetCursorPos((i++ % columns) * columnWidth + indentation);
        std::cout << s;
        if (i % columns == 0) {
            std::cout << nl;
        }
        std::cout.flush();
    }
}

void printGeneralHelp() {

    linenoiceSetCursorPos(0);

    std::cout << nl << nl << "Type ?<function or object name> to display help of the object." << nl << nl;
    std::cout.flush();

    StringVector cmd;
    cmd.push_back("?");
    cmd.push_back("Print this screen");
    cmd.push_back("clr");
    cmd.push_back("Clear screen");
    cmd.push_back("(ESC)");
    cmd.push_back("Show a list over available completions");
    cmd.push_back("(Ctrl + r)");
    cmd.push_back("Recursive search of previous commands");
    cmd.push_back("(Ctrl + a)");
    cmd.push_back("Go to beginning of line");
    cmd.push_back("(Ctrl + e)");
    cmd.push_back("Go to end of line");
    cmd.push_back("UP / DOWN arrows");
    cmd.push_back("Browse command history");


    std::cout << nl << TerminalFormatter::makeUnderlined("Available commands") << nl;
    std::cout.flush();
    printColumns(cmd);

}

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

    // TODO: check that current state is supposed to have arguments

    // start on a empty completion buffer when typing a separator
    pseudoPos = std::string(buf).size() - em.getLinePos();
    
    std::string _s(buf);

    // get the interesting part after the (
    unsigned int start = _s.rfind("(");
    if ((start + 1) > _s.size()) {
        return 0;
    }
    std::string s = _s.substr(start + 1);

    // split on list separator to get all arguments
    StringVector args;
    boost::split(args, s, boost::is_any_of(","));

    // parse the parameter names
    StringVector params;

    BOOST_FOREACH(std::string param, args) {
        StringVector tmp;
        boost::split(tmp, param, boost::is_any_of("="));
        std::string p = tmp.at(0);
        boost::algorithm::trim(p);
        params.push_back(p);
    }

    // add parameters that haven't yet been used to completions
    StringVector completions;

    BOOST_FOREACH(std::string comp, em.getCurrentState()->getCompletions()) {
        bool used = false;

        BOOST_FOREACH(std::string p, params) {
            if (p == comp) {
                used = true;
                continue;
            }
        }
        if (!used) {
            completions.push_back(comp);
        }
    }

    em.getCurrentState()->setCompletions(completions);

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
 * Add parameters to tab complete if the corresponding function or method is known.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int bracketCallback(const char *buf, size_t len, char c) {
    std::string type = "";
    // query current state before switching to new
    if (em.getCurrentState()->getType() == ST_LISTMEMBERS) {
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

        BOOST_FOREACH(std::string s, Filesystem::getFileList(opt.getIncludePaths(), ".Rev")) {
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
    std::cout.flush();

    unsigned int startPos = em.getLinePos(); // place in buffer to start match    
    while (buf[startPos] == ' ') { // discard extra spaces
        startPos++;
    }

    StringVector v;

    BOOST_FOREACH(std::string comp, em.getCurrentState()->getCompletions()) {
        if (boost::starts_with(comp, (buf + startPos + pseudoPos))) {
            v.push_back(comp);
        }
    }
    printColumns(v, 36);

    // debug
    if (debug) {
        std::cout << nl << TerminalFormatter::makeUnderlined("Editor state") << nl;
        std::cout.flush();
        std::cout << em.getMessage() << nl;
        std::cout.flush();
        
        std::cout << nl << TerminalFormatter::makeUnderlined("Debug info") << nl;
        std::cout.flush();
        StringVector dv;
        
        dv.push_back("pseudoPos");
        dv.push_back(boost::lexical_cast<std::string>(pseudoPos));
        dv.push_back("linePos");
        dv.push_back(boost::lexical_cast<std::string>(em.getLinePos()));
        dv.push_back("comp match");
        dv.push_back(buf + startPos + pseudoPos);
        
        printColumns(dv);
       
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
    if(pseudoPos > 0){
        pseudoPos --;
    }
    
    em.deleteChar();
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

    unsigned int startPos = em.getLinePos() + pseudoPos;
    //unsigned int startPos = std::string(buf).size();

    // discard extra spaces
    while (buf[startPos] == ' ' || buf[startPos] == ',') {
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
void RbClient::startInterpretor(IHelp *help, Options options, Configuration configuration) {
    
    em.setObserver(this);

    opt = options;
    config = configuration;

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
        } else {
            // interpret Rev statement
            if (result == 0 || result == 2) {
                commandLine = cmd;
                em.reset();

            } else if (result == 1) {
                prompt = incomplete_prompt;
                commandLine += cmd;
            }
            //std::cout << "Issuing command: " << commandLine << nl;
            //std::cout.flush();)
            result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
        }

        /* The typed string is returned as a malloc() allocated string by
         * linenoise, so the user needs to free() it. */
        free(line);
    }
}

/**
 * This method is called by EditorStateMachine whenever state changes
 */
void RbClient::eventStateChanged(){
    pseudoPos = 0;
}