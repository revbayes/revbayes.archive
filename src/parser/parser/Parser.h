/**
 * @file
 * This file contains the declaration of Parser, which is
 * used to execute syntax trees and get help information.
 *
 * @brief Declaration of Parser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Parser_H
#define Parser_H

#include "Environment.h"
#include "RbOptions.h"
#include "RbString.h"

#include <list>
#include <string>
#include <sstream>

class RbString;
class SyntaxElement;
class SyntaxFunctionCall;

/**
 * @brief Singleton Parser class
 *
 * The parser relies on automatically generated code produced
 * by bison and flex, in the files grammar.tab.cpp, grammar.tab.h
 * and lex.yy.cpp.
 *
 * You use the parser by calling the processCommand function with
 * a string buffer containing one or more, possibly incomplete, Rev
 * lines. The function breaks the buffer into Rev lines using the
 * breakIntoLines help function. Then it cycles through the lines,
 * using the bison and flex code through the call to bison's yyparse
 * function. The call to yyparse generates one or more calls to flex's
 * yylex function, which in turn calls the global rrinput function,
 * defined in the Parser.cpp file. The rrinput function uses the parser's
 * getline function to get a single line of input from the global
 * stringstream rrcommand, which is loaded with one Rev line at a time by
 * the processCommand function before the call to yyparse.
 * 
 * The call to yyparse generates an error code if there is a syntax error
 * in the command buffer. For each complete and syntactically correct statement
 * found in the buffer, the bison code generates a call to the execute or help
 * function of the parser. The execute function executes the syntax tree
 * corresponding to the statement, while the help function gets appropriate help
 * information if the statement is a help request. This behavior is defined in
 * the grammar.y file.
 *
 * Note that there can be nested calls to the parser, for instance
 * if the 'source' function starts parsing a file as part of the
 * execution of a statement resulting from the parsing of a
 * command buffer containing a 'source' call. Rather than using
 * multiple parsers to handle this, each with its own flex buffer, we
 * use a unique global parser. This works because we rely on the fact that
 * each call to processCommand generates its own command buffer with its
 * constituent Rev lines. By making sure we only call the bison and flex
 * code with one Rev line at a time, we avoid any issues with global state
 * flags or buffers that have to be maintained across calls to yyparse.
 * Nested calls to processCommand work because each call maintains its own set
 * of Rev lines. Once an inner call to processCommand has finished, the outer
 * call can continue processing its remaining lines, if any.
 *
 * If multiple Rev lines had been fed to a single flex buffer, then the remaining
 * lines of the outer call would have been discarded by the time the inner call had
 * finished. For instance, this problem would occur with a command string
 * such as 'source("somefile"); <this statement would be discarded>'
 */
class Parser {

    public:
        // Regular functions
        int                 execute(SyntaxElement* root, Environment &env) const;   //!< Execute the syntax tree
        void                getline(char* buf, size_t maxsize);                     //!< Give flex one line to process
        int                 help(const RbString& symbol) const;                     //!< Get help for a symbol
        int                 help(const SyntaxFunctionCall& root) const;             //!< Get help for a function call
        int                 processCommand(std::string& command, Environment *env); //!< Process command with help from Bison

        /** Get singleton parser */
        static Parser& getParser() {
            static Parser theParser;
            return theParser;
        }

   private:
                            Parser(void) {}                                         //!< Prevent construction
                            Parser(const Parser& x) {}                              //!< Prevent copy construction

        Parser&             operator=(const Parser& w) { return (*this); }          //! Prevent assignment

        void                breakIntoLines(const std::string& cmd, std::list<std::string>& lines) const;    //!< Break a command string buffer into Rev lines
};


// Global call-back function for flex-generated code
void rrinput(char* buf, size_t& result, size_t maxsize);

// Global command buffer string stream for rrinput
extern std::stringstream rrcommand;

// Global flags indicating flex state
extern bool foundNewline;
extern bool foundErrorBeforeEnd;
extern bool foundEOF;

// Global column and token length variables for flex
extern size_t yycolumn;
extern size_t yyleng;

#endif
