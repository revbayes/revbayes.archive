/**
 * @file
 * This file contains the implementation of Parser, which is
 * used to execute syntax trees and get help info.
 *
 * @brief Implementation of Parser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Parser.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbString.h"
#include "VectorString.h"
#include "SyntaxElement.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxVariable.h"
#include "UserInterface.h"
#include "Workspace.h"

#include <sstream>

/** Trigger printing of PRINTF debug statements by defining DEBUG_PARSER */
#ifdef DEBUG_PARSER
#define PRINTF printf
#else
#define PRINTF(...)
#endif


// Global flags indicating whether flex found a newline or EOF
bool foundNewline;
bool foundEOF;

/** This function gets help info about a symbol */
int Parser::help(RbString *symbol) const {

    std::ostringstream msg;

#	if defined DEBUG_PARSER
    // Print syntax tree 
    std::cerr << std::endl;
    std::cerr << "Parser trying to get help for symbol '" << std::string(*symbol) << "'";
    std::cerr << std::endl;
#	endif

    // Get some help when we have some help to get

    // Print some silly stuff for now
    msg << "Sorry, I don't know anything about '" << std::string(*symbol);
    msg << "'; I wish I did because it looks interesting." << std::endl;

    // Output the message
    RBOUT(msg.str());

    // Delete the symbol
    delete symbol;

    // Return success
    return 0;
}


/** This function prints help info about a function if it sees a function call */
int Parser::help(SyntaxElement *root) const {

    std::ostringstream msg;

#	if defined DEBUG_PARSER
    // Print syntax tree
    std::cerr << std::endl;
    std::cerr << "Syntax tree root before help:\n";
    root->print(std::cerr);
    std::cerr << std::endl;
#	endif

    RbString symbol;

    if ( root->isType(SyntaxFunctionCall_name) ) {
        symbol = *((SyntaxFunctionCall*)(root))->getFunctionName();
    }
    else {
        msg << "I have no clue -- Bison was not supposed to ask me about this!";
        RBOUT(msg.str());
        delete root;
        return 1;
    }

    return help(&symbol);
}


/** This function causes recursive execution of a syntax tree by calling the root to get its value */
int Parser::execute(SyntaxElement *root) const {

#	ifdef DEBUG_PARSER
    // Print syntax tree
    std::cerr << std::endl;
    std::cerr << "Syntax tree root before execution:\n";
    root->print(std::cerr);
    std::cerr << std::endl;
#	endif

    // Declare a variable for the result
    DAGNode *result;
    
    //! Execute syntax tree
    try {
        PRINTF("Parser getting the semantic value of the syntax tree...\n");
        result = root->getValue(&Workspace::userWorkspace());
    }
    catch(RbException& rbException) {

        // Catch a quit request
        if (rbException.getExceptionType() == RbException::QUIT)
            exit(0);

        // All other exceptions
        PRINTF("Caught an exception\n");
        std::ostringstream msg;
        rbException.printValue(msg);
        msg << std::endl;
        RBOUT(msg.str());

        // Return signal indicating problem
        return 2;
    }

    // Print result if the root is not an assign expression
    if (result != NULL && !root->isType(SyntaxAssignExpr_name)) 
		{
        std::ostringstream msg;
        result->printValue(msg);
        RBOUT(msg.str());
		}

    // Delete syntax tree and result
    delete result;
    delete root;

    // Return success
    return 0;
}


/** Give flex a line to parse */
void Parser::getline(char* buf, int maxsize) {

    if (!rrcommand.good()) {
        foundEOF = true;
        buf[0] = EOF;
        buf[1] = '\0';
    }
    else {
        foundNewline = false;
        rrcommand.getline(buf, maxsize-3);
        size_t i = strlen(buf);
        buf[i++] = '\n';
        buf[i] = '\0';
    }

#	if defined (DEBUG_PARSER)
    if (foundEOF == true)
        PRINTF("Parser gave flex EOF\n");
    else
        PRINTF("Parser gave flex line(s):\n %s", buf);
#	endif
}


/** Process command with the help of the Bison-generated code; return 1 if command is incomplete */
int Parser::processCommand(std::string& command) {

    extern int yyparse(void);   // Defined in grammar.tab.c (from gammar.y)

    // Append command to stream where flex can find it
    rrcommand.str(rrcommand.str() + command);

    // Call Bison code, which calls Flex code, which calls rrinput
    PRINTF("\nCalling bison with rrcommand:\n'%s'\n", rrcommand.str().c_str());
    foundNewline = false;
    foundEOF = false;
    int result;
    try {
        result = yyparse();
    }
    catch(RbException& rbException) {
        PRINTF("Caught an exception\n");

        // Catch a quit request
        if (rbException.getExceptionType() == RbException::QUIT)
            exit(0);

        // All other exceptions
        PRINTF("Parse error\n");
        std::ostringstream msg;
        rbException.printValue(msg);
        msg << std::endl;
        RBOUT(msg.str());

        // Return signal indicating problem
        return 1;
    }

    if (result == 0) {
        PRINTF("Parser resetting command string\n\n");
        rrcommand.str("");
        rrcommand.clear();  // Clear any error flags
        return 0;
    }
    else if (result == 1 && foundNewline == true && foundEOF == false && !rrcommand.good()) {
        PRINTF("Incomplete command ending with inappropriate newline; stripping newline and appending to command string\n");
        rrcommand.str(rrcommand.str() + " ");
        rrcommand.clear();
        return 1;
    }
    else if (result == 1 && foundNewline == true && foundEOF == true) {
        PRINTF("Incomplete command ending with appropriate newline; keeping newline and appending to command string\n");
        rrcommand.str(rrcommand.str() + "\n ");
        rrcommand.clear();
        return 1;
    }
    else {
        PRINTF("Syntax error\n");
        rrcommand.str("");
        while (foundNewline == false)
            yyparse();
        rrcommand.clear();
        return 2;
    }
}


/** Reset the parser */
void Parser::reset(void) {

    rrcommand.str("");
    rrcommand.clear();
}


/** Global call-back function for the flex-generated code in lex.yy.cpp */
void rrinput(char* buf, int& result, int maxsize) {

    buf[0] = '\0';
    Parser::getParser().getline(buf, maxsize);
    result = int(strlen(buf));
}


