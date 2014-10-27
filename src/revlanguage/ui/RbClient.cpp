
#include <iterator>

#include "RbClient.h"
#include "RbFileManager.h"
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
#include <iomanip>
#include <ctype.h>

extern "C" {
#include "linenoise.h"
}

#include "lineeditUtils.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp> 
#include "boost/algorithm/string_regex.hpp"
#include "RevLanguageMain.h"
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range/algorithm/count.hpp>

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


WorkspaceUtils workspaceUtils;

void setDefaultCompletions();
StringVector getDefaultCompletions();

std::string getWd() {
    
    RbSettings& s = RbSettings::userSettings();
    const std::string& wd = s.getWorkingDirectory();
    return wd;
}

StringVector getFileList(std::string path)
{
    StringVector v;
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(getWd(), path);
    fm.setStringWithNamesOfFilesInDirectory( v );
    
    return v;
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
        //editorMachine.getCurrentState()->addCompletion(s);
    }
}

/**
 * tab completion callback
 * 
 * Update list of available completions.
 * 
 * @param buf
 * @param lc
 */
void completeOnTab(const char *buf, linenoiseCompletions *lc) {
    bool debug = false;

    std::string cmd = buf;
    StringVector completions;

    // parse command
    RevLanguage::ParserInfo pi = RevLanguage::Parser::getParser().checkCommand(cmd, &RevLanguage::Workspace::userWorkspace());

    if (pi.inComment) {
        // no completions available in comments
        return;
    }

    // set completions and position on command line where to start matching completions
    int commandPos = 0;
    if (pi.inQuote) {
        // ---------- in quote ------------
        // search for files with portion after the opening quote                
        commandPos = cmd.rfind("\"") + 1;
        completions = getFileList(cmd.substr(commandPos, cmd.size()));
    } else {        
        // special hack: for some reason, baseVariable is only set by the parser when there is no trailing characters after the dot
        StringVector expressionSeparator;
        expressionSeparator += " ", "%", "~", "=", "&", "|", "~", "+", "-", "*", "/", "^", "!", "=", ",", "<", ">", ")", "[", "]", "{", "}";

        // find position of right most expression separator in cmd

        BOOST_FOREACH(std::string s, expressionSeparator) {
            commandPos = std::max(commandPos, (int) cmd.rfind(s));
        }

        // find position of right most dot
        int dotPosition = (int) cmd.rfind(".");
        
        
        if ((pi.baseVariable != NULL) || (dotPosition > commandPos)) {
            // ---------- object defined ------------
            std::string baseVariable;
            if (commandPos > 0) {
                commandPos++;
            }
            if (pi.baseVariable == NULL) {
                baseVariable = cmd.substr(commandPos, dotPosition - commandPos );                
            } else {
                baseVariable = pi.baseVariable->getName();
            }

            commandPos = std::max(commandPos, dotPosition) + 1;            
            if (workspaceUtils.isObject(baseVariable)) {
                StringVector sv = workspaceUtils.getObjectMembers(baseVariable);

                BOOST_FOREACH(std::string member, sv) {
                    completions.push_back(member);
                }
            }
        } else if (pi.functionName != "") {
            // ---------- function defined ------------
            if (pi.argumentLabel != "") { // assigning an argument label                
                commandPos = cmd.rfind("=") + 1;
                // not sure exactly what should be here... setting completions to everything
                completions = getDefaultCompletions();

            } else { // break on either '(' or ','                
                int tmp1 = cmd.rfind("(");
                int tmp2 = cmd.rfind(",");
                commandPos = std::max(tmp1, tmp2) + 1;

                BOOST_FOREACH(std::string param, workspaceUtils.getFunctionParameters(pi.functionName)) {
                    completions.push_back(param);
                }
            }
        } else {
            // ---------- default -----------            
            if (commandPos > 0) {
                commandPos++;                
            }
            completions = getDefaultCompletions();

        }
    }

    // discard any extra space in beginning of the string that is used to match against completions
    while (buf[commandPos] == ' ') {
        commandPos++;
    }

    // match partial command and pass filtered completions to linenoise
    std::string previousCommands;
    for (int i = 0; i < commandPos; i++) {
        previousCommands += buf[i];
    }

    // the string the matching is made against
    std::string compMatch(buf + commandPos);

    // populate linenoise completions

    BOOST_FOREACH(std::string m, completions) {
        if (boost::starts_with(m, compMatch)) {
            std::string c = previousCommands + m;
            linenoiseAddCompletion(lc, c.c_str());
        }
    }

    // debug
    if (debug) {
        std::cout << "\n\r--------------- available completions --------------";

        BOOST_FOREACH(std::string m, completions) {
            std::cout << "\n\r" << m;
        }

        std::string baseVariable = "";
        if (pi.baseVariable != NULL) {
            baseVariable = pi.baseVariable->getName();
        }
        if (!pi.inQuote) {
            std::cout << "\n\r------------- buffer info ----------------"
                    << "\n\rmatching against: " << compMatch
                    << "\n\r"
                    << "\n\rinComment: " << pi.inComment
                    << "\n\rinQuote: " << pi.inQuote
                    << "\n\rresult: " << pi.result
                    << "\n\rfunctionName: " << pi.functionName
                    << "\n\rbaseVariable: " << baseVariable
                    << "\n\rargumentLabel: " << pi.argumentLabel
                    << "\n\r" << pi.message
                    << "\n\rlines:";

            BOOST_FOREACH(std::string l, pi.lines) {
                std::cout << "\n\r\t" << l;
            }
        }
    }
}

/**
 * Print out function signatures
 * @param buf
 * @param len
 * @param c
 * @return 
 */
int printFunctionParameters(const char *buf, size_t len, char c) {
    std::string cmd = buf;
    RevLanguage::ParserInfo pi = RevLanguage::Parser::getParser().checkCommand(cmd, &RevLanguage::Workspace::userWorkspace());
    if (workspaceUtils.isFunction(pi.functionName)) {

        BOOST_FOREACH(WorkspaceUtils::FunctionSignature sign, workspaceUtils.getFunctionSignatures(pi.functionName)) {
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
    return 0;
}

/**
 * Main application loop.
 * 
 * @param help
 * @param options
 * @param configuration
 */
void RbClient::startInterpretor( void )
{

    /* Set tab completion callback */
    linenoiseSetCompletionCallback(completeOnTab);

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    /* callback for printing function signatures on opening bracket*/
    linenoiseSetCharacterCallback(printFunctionParameters, '(');

    int result = 0;
    std::string commandLine;
    std::string cmd;

    while (true) {

        // set prompt
        if (result == 0 || result == 2) {
            prompt = default_prompt;
        } else //if (result == 1) 
        {
            prompt = incomplete_prompt;
        }

        line = linenoise(prompt);

        // process command line
        linenoiseHistoryAdd(line); /* Add to the history. */
        linenoiseHistorySave("history.txt"); /* Save the history on disk. */

        cmd = line;
        boost::trim(cmd);

        if (cmd == "clr" || cmd == "clear") {
            linenoiseClearScreen();
        } else if (cmd == "debug=false") {
            debug = false;
        } else if (cmd == "debug=true") {
            debug = true;
        } else {
            // interpret Rev statement
            if (result == 0 || result == 2) {
                commandLine = cmd;
            } else //if (result == 1) 
            {
                commandLine += cmd;
            }
            result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
        }

        /* The typed string is returned as a malloc() allocated string by
         * linenoise, so the user needs to free() it. */
        free(line);

    }
}
