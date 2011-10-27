/**
 * @file
 * This file contains the declaration of Parser, which is
 * used to execute syntax trees and get help information.
 *
 * The parser relies on automatically generated code produced
 * by Bison and Flex, in the files grammar.tab.c, grammar.tab.h
 * and lex.yy.cpp. When the parser gets a command to process,
 * it first stores it in rrommand. Then it invokes Bison with
 * the call yyparse(). Bison calls flex, which calls the global
 * function rrinput, which calls the getline function of the
 * parser, which retrieves the command input, line by line, from
 * rrcommand.
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


#include "RbOptions.h"
#include "RbPtr.h"
#include "RbString.h"

#include <string>
#include <sstream>

class RbString;
class SyntaxElement;
class SyntaxFunctionCall;

class Parser {

    public:
        // Regular functions
        int                 execute(RbPtr<SyntaxElement> root) const;   //!< Execute the syntax tree
        void                getline(char* buf, size_t maxsize);         //!< Give flex one line to process
        int                 help(RbPtr<RbString> symbol) const;         //!< Get help for a symbol
        int                 help(RbPtr<SyntaxFunctionCall> root) const;      //!< Get help for a function call
        int                 processCommand(std::string& command);       //!< Process command with help from Bison
        void                reset(void);                                //!< Reset the parser

        /** Get singleton parser */
        static Parser& getParser() {
            static Parser theParser;
            return theParser;
        }

   private:
                            Parser(void) {}                             //!< Prevent construction
                            Parser(const Parser& x) {}                  //!< Prevent copy construction

        Parser&             operator=(const Parser& w) { return (*this); }  //! Prevent assignment

        std::stringstream   rrcommand;                                  //!< Keep the command here
};


// Global call-back function for flex-generated code
void rrinput(char* buf, int& result, int maxsize);

// Global flag indicating whether flex found newline
extern bool foundNewline;
extern bool foundErrorBeforeEnd;

#endif

