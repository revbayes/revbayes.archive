
#include <iterator>

#include "RbClient.h"
#include "Parser.h"
#include "Workspace.h"
#include "WorkspaceUtils.h"
#include "TerminalFormatter.h"
#include "RbSettings.h"


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
#include <boost/assign/std/vector.hpp>

using namespace boost::assign; // bring 'operator+=()' into scope

#define ctrl(C) ((C) - '@')
typedef std::vector<std::string> StringVector;

bool debug = false;

const char* nl = (char*) "\n\r";
const char* default_prompt = (char*) "RevBayes > ";
const char* incomplete_prompt = (char*) "RevBayes + ";
const char* esc_prompt = (char*) "? > ";
const char* prompt = default_prompt;
char *line;

EditorMachine editorMachine;
WorkspaceUtils workspaceUtils;
Options *opt;
Configuration *config;

void setDefaultCompletions();
StringVector getDefaultCompletions();

std::string getWd() {
    RbSettings& s = RbSettings::userSettings();
    const std::string& wd = s.getWorkingDirectory();
    return wd;
}

StringVector getFileList(std::string path) {
    StringVector v;

    BOOST_FOREACH(std::string s, Filesystem::getFileList(getWd(), path)) {
        v.push_back(s);
    }
    return v;
}

/**
 * Assemble tab completion data
 * 
 * @param buf
 * @return 
 */
tabCompletionInfo getTabCompletionInfo(const char *buf) {

    unsigned int startPos = editorMachine.getLinePos();

    StringVector specialStrings;
    // important: assign in descending order from longest to shortest strings
    specialStrings += "<-&", "<-", "<<-", ":=", "++", "--", "+=", "-=", "*=", "/=", "&&", "||", "~", "+", "-", "*", "/", "^", "!", "=", ",", "(", ")", " ", "#";

    std::string specialMatch = "";
    int specialMatchType = 0; // 0 = no match against any operator, 1 = reference assignment operator, 2 = other
    std::string stringBuf = buf + startPos;
    int rPos = 0, _rPos, tmpInt;
    bool endWithSpace = false;

    // find the most current operator or control character 
    // and move cursor (linePos) to the point where it is relevant to match for completion 

    BOOST_FOREACH(std::string s, specialStrings) {

        _rPos = tmpInt = stringBuf.find(s, rPos); // first match 

        while (tmpInt != -1) { // find last match
            _rPos = tmpInt + s.length();
            tmpInt = stringBuf.find(s, tmpInt + s.length());
        }

        if (_rPos > rPos) {
            if (s == "<-&") {
                specialMatchType = 1;
                specialMatch = s;
            } else if (s == " ") {
                endWithSpace = true;
            } else if (s == "#") {
                specialMatchType = 3;
                specialMatch = s;
                endWithSpace = false;
                break;
            } else {
                specialMatchType = 2;
                specialMatch = s;
                endWithSpace = false;
            }

            rPos = _rPos;
        }
    }
    startPos += rPos;

    // discard extra space in beginning of string to match against completions
    //    while (buf[startPos] == ' ') {
    //        startPos++;
    //    }

    // the string the matching is made against
    std::string compMatch(buf + startPos);

    //pad previous complete commands so the whole command line can be built
    std::string previousCommands;
    for (unsigned int i = 0; i < startPos; i++) {
        previousCommands += buf[i];
    }

    // assemble completions
    // let state release
    if (endWithSpace) {
        if (editorMachine.getCurrentState()->getType() == ST_ACCESSING_MEMBER) {
            editorMachine.tryRelease(buf);
        }
    }

    // completions are either already set by the character callback or by some of the special cases here
    StringVector completions;
    if (specialMatchType == 0) {
        completions = editorMachine.getCurrentState()->getCompletions();

    } else if (specialMatchType == 3) { // a comment
        completions.clear();
    } else {
        if (specialMatchType == 1) { //reference assignment operator
            // completion only on user defined objects
            completions = workspaceUtils.getObjects(false);

        } else if (editorMachine.getCurrentState()->getType() == ST_IDLE) {
            completions = getDefaultCompletions();

        } else if (editorMachine.getCurrentState()->getType() == ST_DEF_ARGUMENT
                || editorMachine.getCurrentState()->getType() == ST_DEF_LIST) {
            if (specialMatch == ",") {
                completions = editorMachine.getCurrentState()->getCompletions();

            } else {
                completions = getDefaultCompletions();
            }

        } else if (editorMachine.getCurrentState()->getType() == ST_ACCESSING_MEMBER) {
            editorMachine.tryRelease(buf);
        }
    }

    // file lookup is dynamic and has to be set for each tab
    if (editorMachine.getCurrentState()->getType() == ST_DEF_STRING) {
        completions = getFileList(buf + editorMachine.getLinePos());
    }

    // print function signature(s) if applicable    
    if (editorMachine.getCurrentState()->getType() == ST_DEF_LIST) {
        std::string subject = editorMachine.getCurrentState()->getSubject();
        if (workspaceUtils.isFunction(subject)) {

            BOOST_FOREACH(WorkspaceUtils::FunctionSignature sign, workspaceUtils.getFunctionSignatures(subject)) {
                std::cout << "\n\r" + sign.returnType + " " + sign.name + " (";

                for (size_t i = 0; i < sign.arguments.size(); i++) {
                    std::cout << sign.arguments[i];
                    if (i < sign.arguments.size() - 1) {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << ")\n\r";
            linenoiceSetCursorPos(0);
            std::cout << prompt << buf;
            std::cout.flush();
        }
    }





    StringVector matches;
    for (unsigned int i = 0; i < completions.size(); i++) {
        if (boost::starts_with(completions[i], compMatch)) {
            matches.push_back(previousCommands + completions[i]);
        }
    }

    tabCompletionInfo bufferInfo;
    bufferInfo.specialMatchType = specialMatchType;
    bufferInfo.startPos = startPos;
    bufferInfo.completions = completions;
    bufferInfo.matchingCompletions = matches;
    bufferInfo.compMatch = compMatch;

    return bufferInfo;
}

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
int printGeneralHelp(const char *buf, size_t len, char c) {
    linenoiseClearScreen();
    linenoiceSetCursorPos(0);

    std::cout << nl << nl << "Type ?" << TerminalFormatter::makeBold("<function or object name>") << " to display help of the object" << nl;

    StringVector cmd;
    cmd.push_back("?");
    cmd.push_back("Print this screen");
    cmd.push_back("clr or clear");
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
    cmd.push_back("repo_list");
    cmd.push_back("Show files in web repositories");
    cmd.push_back("repo_get <index_number>");
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

    return 0;

}

StringVector getDefaultCompletions() {
    StringVector c;

    BOOST_FOREACH(std::string function, workspaceUtils.getFunctions()) {
        c.push_back(function);
    }

    BOOST_FOREACH(std::string obj, workspaceUtils.getObjects(true)) {
        c.push_back(obj);
    }

    c.push_back("repo_list");
    c.push_back("repo_get");

    return c;
}

/**
 * Basically everything
 */
void setDefaultCompletions() {

    BOOST_FOREACH(std::string s, getDefaultCompletions()) {
        editorMachine.getCurrentState()->addCompletion(s);
    }
}

int spaceCallback(const char *buf, size_t len, char c) {
    editorMachine.tryRelease(buf);
    return 0;
}

/**
 * Retain arguments in tab complete but remove already used ones.
 * 
 * @param buf
 * @param len
 * @param c
 * @return 
 */

int listSeparatorCallback(const char *buf, size_t len, char c) {

    if (editorMachine.processInput(buf)) {

        // shouldn't be here if not previous state was defining a list
        EditorState *func = editorMachine.getStateQueue()->at(editorMachine.getStateQueue()->size() - 2);
        std::string usedArgument = editorMachine.getCurrentState()->getSubject();

        // pop the used argument from completions        
        StringVector v;

        BOOST_FOREACH(std::string s, func->getCompletions()) {
            if (s != usedArgument) {
                v.push_back(s);
            }
        }
        func->setCompletions(v);

        // use pruned completions also in current state
        editorMachine.getCurrentState()->setCompletions(v);

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

    if (editorMachine.processInput(buf)) {
        std::string subject = editorMachine.getCurrentState()->getSubject();
        if (workspaceUtils.isType(subject)) {
            StringVector sv = workspaceUtils.getTypeMembers(subject);

            BOOST_FOREACH(std::string member, sv) {
                editorMachine.getCurrentState()->addCompletion(member);
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
    if (editorMachine.getCurrentState()->getType() == ST_ACCESSING_MEMBER) {
        type = editorMachine.getCurrentState()->getSubject();
    }
    if (editorMachine.processInput(buf)) {
        std::string subject = editorMachine.getCurrentState()->getSubject();
        // add arguments to completions
        if (type == "") { // ordinary function               

            BOOST_FOREACH(std::string param, workspaceUtils.getFunctionParameters(subject)) {
                editorMachine.getCurrentState()->addCompletion(param);
            }

        } else { // member method

            BOOST_FOREACH(std::string param, workspaceUtils.getMethodParameters(type, subject)) {
                editorMachine.getCurrentState()->addCompletion(param);
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

    if (editorMachine.processInput(buf)) {

        BOOST_FOREACH(std::string s, getFileList("")) {
            editorMachine.getCurrentState()->addCompletion(s);
        }

        //std::cout << "\n\rcmd: " << editorMachine.getCmd() << "\n\r";
        //std::cout << "\n\rlinepos: " << editorMachine.getLinePos() << "\n\r";        
        //std::cout.flush();

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
        printGeneralHelp(buf, len, c);
    }

    // completions
    linenoiceSetCursorPos(0);
    std::cout << nl << TerminalFormatter::makeUnderlined("Available completions") << nl;

    tabCompletionInfo tInfo = getTabCompletionInfo(buf);
    printData(tInfo.completions, 2, 36);

    // debug
    if (debug) {
        std::cout << nl << TerminalFormatter::makeUnderlined("Editor state") << nl;

        std::cout << editorMachine.getMessage() << nl;


        std::cout << nl << TerminalFormatter::makeUnderlined("Debug info") << nl;

        StringVector dv;

        dv.push_back("em linePos");
        dv.push_back(boost::lexical_cast<std::string>(editorMachine.getLinePos()));
        dv.push_back("match start");
        dv.push_back(boost::lexical_cast<std::string>(tInfo.startPos));
        dv.push_back("comp match");
        dv.push_back("'" + tInfo.compMatch + "'");

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
    editorMachine.deleteChar(buf);
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

    tabCompletionInfo tInfo = getTabCompletionInfo(buf);

    BOOST_FOREACH(std::string m, tInfo.matchingCompletions) {
        linenoiseAddCompletion(lc, m.c_str());
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

    editorMachine.setObserver(this);

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
    //linenoiseSetCharacterCallback(assignCallback, '=');
    linenoiseSetCharacterCallback(quotationCallback, '"');
    linenoiseSetCharacterCallback(listSeparatorCallback, ',');
    //linenoiseSetCharacterCallback(spaceCallback, ' ');

    linenoiseSetCharacterCallback(escapeCallback, 27);
    linenoiseSetCharacterCallback(backspaceCallback, ctrl('H')); // works in netbeans pseudo terminal
    linenoiseSetCharacterCallback(backspaceCallback, 0x7f); // works in gnome terminal

    /* help */
    //linenoiseSetCharacterCallback(printGeneralHelp, '?');


    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */


    editorMachine.reset();
    setDefaultCompletions();
    int result = 0;
    std::string commandLine;
    std::string cmd;

    while ((line = linenoise(prompt)) != NULL) {

        linenoiseHistoryAdd(line); /* Add to the history. */
        linenoiseHistorySave("history.txt"); /* Save the history on disk. */
        
        //linenoiseSetPrompt("hej");

        cmd = line;
        boost::trim(cmd);

        if (cmd == "clr" || cmd == "clear") {
            linenoiseClearScreen();
        } else if (cmd == "debug=false") {
            debug = false;
        } else if (cmd == "debug=true") {
            debug = true;
        } else if (repoClient.processCommand(config->getRepositories(), cmd)) {

        } else if (cmd == "?") {
            printGeneralHelp("", 0, 'c');
        } else {            
            
            // interpret Rev statement
            if (result == 0 || result == 2) {
                
                prompt = default_prompt;
                
                commandLine = cmd;
                editorMachine.reset();

            } else //if (result == 1) 
            {                
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
        editorMachine.getCurrentState()->addCompletion(state->getSubject());
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
        //linenoiseClearScreen();
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

        fs::path saveDir = getWd();
        saveDir /= "downloads";
        std::cout << "Save file to '" + saveDir.string() + "' Y/N ?: (Y) ";
        char answer;
        std::cin.get(answer);
        if (answer == 'y' || answer == 'Y' || answer == '\n') {

            if (Filesystem::saveToFile(httpResponse.data.at(0), saveDir.string(), httpResponse.data.at(1))) {
                std::cout << "File saved!" << nl;
            } else {
                std::cout << "Something went wrong and the file couldn't be saved!" << nl;
            }
        }
    }
    std::cout << nl;
}