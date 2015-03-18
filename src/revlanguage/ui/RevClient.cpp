#include "FunctionTable.h"
#include "RbFileManager.h"
#include "RevClient.h"
#include "RlFunction.h"
#include "Parser.h"
#include "Workspace.h"
#include "TerminalFormatter.h"
#include "RbSettings.h"


#ifdef RB_MPI
#include <mpi.h>
#endif

extern "C" {
#include "linenoise.h"
}

#include "lineeditUtils.h"


#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "boost/algorithm/string_regex.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/assign/std/vector.hpp>

//#define ctrl(C) ((C) - '@')

const char* default_prompt = (char*) "> ";
const char* incomplete_prompt = (char*) "+ ";
const char* prompt = default_prompt;

using namespace RevLanguage;

std::vector<std::string> getFileList(const std::string &path)
{
    std::vector<std::string> v;
    
    RbSettings& s = RbSettings::userSettings();
    const std::string& wd = s.getWorkingDirectory();
    
    RevBayesCore::RbFileManager fm = RevBayesCore::RbFileManager(wd, path);
    fm.setStringWithNamesOfFilesInDirectory( v );
    
    return v;
}



std::vector<std::string> getDefaultCompletions( void )
{
    std::set<std::string> c;

    const FunctionTable& ft = Workspace::userWorkspace().getFunctionTable();
    
    for (std::multimap<std::string, Function*>::const_iterator it = ft.begin(); it != ft.end(); ++it)
    {
        c.insert(it->first);
    }
    
    VariableTable v = Workspace::userWorkspace().getVariableTable();
    
    for (VariableTable::iterator it = v.begin(); it != v.end(); ++it)
    {
        c.insert(it->first);
    }
    
    
    v = RevLanguage::Workspace::globalWorkspace().getVariableTable();
        
    for (VariableTable::iterator it = v.begin(); it != v.end(); ++it)
    {
        c.insert(it->first);
    }

    const TypeTable& t_user = RevLanguage::Workspace::userWorkspace().getTypeTable();
    
    for (TypeTable::const_iterator it = t_user.begin(); it != t_user.end(); ++it)
    {
        c.insert(it->first);
    }
    
    const TypeTable& t_global = RevLanguage::Workspace::globalWorkspace().getTypeTable();
    
    for (TypeTable::const_iterator it = t_global.begin(); it != t_global.end(); ++it)
    {
        c.insert(it->first);
    }
    
    std::vector<std::string> vec;
    for (std::set<std::string>::iterator it = c.begin(); it != c.end(); ++it)
    {
        vec.push_back( *it );
    }

    return vec;
}

/**
 * tab completion callback
 * 
 * Update list of available completions.
 * 
 * @param buf
 * @param lc
 */
void completeOnTab(const char *buf, linenoiseCompletions *lc)
{
    //bool debug = true;

    std::string cmd = buf;
    std::vector<std::string> completions;

    // parse command
    RevLanguage::ParserInfo pi = RevLanguage::Parser::getParser().checkCommand(cmd, &RevLanguage::Workspace::userWorkspace());

    if (pi.inComment)
    {
        // no completions available in comments
        return;
    }

    // set completions and position on command line where to start matching completions
    size_t commandPos = 0;
    if (pi.inQuote)
    {
        // ---------- in quote ------------
        // search for files with portion after the opening quote                
        commandPos = cmd.rfind("\"") + 1;
        completions = getFileList(cmd.substr(commandPos, cmd.size()));
    }
    else
    {

        std::vector<std::string> expressionSeparator;
        
        expressionSeparator.push_back(" ");
        expressionSeparator.push_back("%");
        expressionSeparator.push_back("~");
        expressionSeparator.push_back("=");
        expressionSeparator.push_back("&");
        expressionSeparator.push_back("|");
        expressionSeparator.push_back("+");
        expressionSeparator.push_back("-");
        expressionSeparator.push_back("*");
        expressionSeparator.push_back("/");
        expressionSeparator.push_back("^");
        expressionSeparator.push_back("!");
        expressionSeparator.push_back("=");
        expressionSeparator.push_back(",");
        expressionSeparator.push_back("<");
        expressionSeparator.push_back(">");
        expressionSeparator.push_back(")");
        expressionSeparator.push_back("[");
        expressionSeparator.push_back("]");
        expressionSeparator.push_back("{");
        expressionSeparator.push_back("}");

        // find position of right most expression separator in cmd

        BOOST_FOREACH(std::string s, expressionSeparator)
        {
            commandPos = std::max(commandPos, cmd.rfind(s));
        }

        // special hack: for some reason, baseVariable is only set by the parser when there is no trailing characters after the dot
        // find position of right most dot
        // Sebastian: Currently unused
//        size_t dotPosition = cmd.rfind(".");

        if (pi.functionName != "")
        {
            // ---------- function defined ------------
            if (pi.argumentLabel != "") // assigning an argument label
            {
                commandPos = cmd.rfind("=") + 1;
                // not sure exactly what should be here... setting completions to everything
                completions = getDefaultCompletions();

            }
            else // break on either '(' or ','
            {
                commandPos = std::max(cmd.rfind("("), cmd.rfind(",")) + 1;
                
                std::vector<Function *> v = Workspace::globalWorkspace().getFunctionTable().findFunctions(pi.functionName);
                
                for (std::vector<Function *>::iterator it = v.begin(); it != v.end(); it++)
                {
                    const RevLanguage::ArgumentRules& argRules = (*it)->getArgumentRules();
                    for (size_t i = 0; i < argRules.size(); i++)
                    {
                        completions.push_back(argRules[i].getArgumentLabel());
                    }
                }
            }
        }
        else
        {
            // ---------- default -----------            
            if (commandPos > 0)
            {
                commandPos++;
            }
            completions = getDefaultCompletions();

        }
    }

    // discard any extra space in beginning of the string that is used to match against completions
    while (buf[commandPos] == ' ')
    {
        commandPos++;
    }

    // match partial command and pass filtered completions to linenoise
    std::string previousCommands;
    for (int i = 0; i < commandPos; i++)
    {
        previousCommands += buf[i];
    }

    // the string the matching is made against
    std::string compMatch(buf + commandPos);

    // populate linenoise completions

    BOOST_FOREACH(std::string m, completions)
    {
        if (boost::starts_with(m, compMatch))
        {
            std::string c = previousCommands + m;
            linenoiseAddCompletion(lc, c.c_str());
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
int printFunctionParameters(const char *buf, size_t len, char c)
{
    std::string cmd = buf;
    RevLanguage::ParserInfo pi = RevLanguage::Parser::getParser().checkCommand(cmd, &RevLanguage::Workspace::userWorkspace());
    if ( Workspace::globalWorkspace().existsFunction(pi.functionName) )
    {

        std::vector<Function *> functions = Workspace::globalWorkspace().getFunctionTable().findFunctions(pi.functionName);
        
        for (std::vector<Function *>::iterator it = functions.begin(); it != functions.end(); ++it)
        {
            Function *f = *it;
            std::cout << "\n\r" + f->getReturnType().getType() + " " + pi.functionName + " (";

            const RevLanguage::ArgumentRules& argRules = (*it)->getArgumentRules();
            for (size_t i = 0; i < argRules.size(); i++)
            {
                std::cout << argRules[i].getArgumentLabel();
                if (i < argRules.size() - 1) {
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
 */
void RevClient::startInterpretor( void )
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    /* Set tab completion callback */
    linenoiseSetCompletionCallback( completeOnTab );

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    if ( pid == 0 )
    {
        linenoiseHistoryLoad("history.txt"); /* Load the history at startup */
    }
    
    /* callback for printing function signatures on opening bracket*/
    linenoiseSetCharacterCallback(printFunctionParameters, '(');

    int result = 0;
    std::string commandLine;
    std::string cmd;

    while (true)
    {
        
        char *line = NULL;
        
        // set prompt
        if (result == 0 || result == 2)
        {
            prompt = default_prompt;
        }
        else //if (result == 1)
        {
            prompt = incomplete_prompt;
        }


        // process command line
        if ( pid == 0 )
        {
            line = linenoise(prompt);
            
            linenoiseHistoryAdd(line);              /* Add to the history. */
            linenoiseHistorySave("history.txt");    /* Save the history on disk. */
        
            cmd = line;
            boost::trim(cmd);

            if (cmd == "clr" || cmd == "clear")
            {
                linenoiseClearScreen();
            }
            else
            {
                // interpret Rev statement
                if (result == 0 || result == 2)
                {
                    commandLine = cmd;
                }
                else //if (result == 1)
                {
                    commandLine += "; " + cmd;
                }
            }
            
        }
        
        size_t bsz = commandLine.size();
#ifdef RB_MPI
        MPI::COMM_WORLD.Bcast(&bsz, 1, MPI_INT, 0);
#endif
            
        char * buffer = new char[bsz+1];
        buffer[bsz] = 0;
        for (int i = 0; i < bsz; i++)
            buffer[i] = commandLine[i];
#ifdef RB_MPI
        MPI::COMM_WORLD.Bcast(buffer, bsz, MPI_CHAR, 0);
#endif
            
        std::string tmp = std::string( buffer );
            
        result = RevLanguage::Parser::getParser().processCommand(tmp, &RevLanguage::Workspace::userWorkspace());


        /* The typed string is returned as a malloc() allocated string by
         * linenoise, so the user needs to free() it. */
        
        if ( pid == 0 )
        {
            free(line);
        }
        
    }
    
    
}
