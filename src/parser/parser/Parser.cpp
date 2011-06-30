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
 * $Id:$
 */


#include "Help.h"
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


// Global flags indicating whether flex found a newline or EOF, and what type of error occurred
bool foundNewline;
bool foundEOF;
bool foundErrorBeforeEnd;


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

        std::ostringstream msg;

        // Catch a quit request
        if (rbException.getExceptionType() == RbException::QUIT)
            exit(0);

        // Catch a missing variable exception that might be interpreted as a request for
        // usage help on a function
        SyntaxVariable* theVariable = dynamic_cast<SyntaxVariable*>( root );
        if ( rbException.getExceptionType() == RbException::MISSING_VARIABLE && theVariable != NULL && !theVariable->isMemberVariable() ) {

            RbString* fxnName = theVariable->getIdentifier()->clone();
            std::vector<const RbFunction*> functions = Workspace::userWorkspace().getFunctionTable()->findFunctions( *fxnName );
            if ( functions.size() != 0 ) {
                RBOUT( "Usage:" );
                for ( std::vector<const RbFunction*>::iterator i=functions.begin(); i!=functions.end(); i++ ) {
                    RBOUT( (*i)->briefInfo() );
                }
            }
            return 0;
        }

        // All other exceptions
        PRINTF("Caught an exception\n");
        msg << "Error: ";
        rbException.printValue(msg);
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
    if ( result != NULL && result->numRefs() == 0 )
        delete result;
    delete root;

    // Return success
    return 0;
}


/** Give flex a line to parse */
void Parser::getline(char* buf, size_t maxsize) {

    if (!rrcommand.good()) {
        foundEOF = true;
        buf[0] = EOF;
        buf[1] = '\0';
    }
    else {
        foundNewline = false;
        rrcommand.getline(buf, maxsize-3);
        size_t i = strlen(buf);
        // Deal with line endings
        if ( i >= 1 && buf[i-1] == '\r' )
            buf[i-1] = '\n';
        else if ( i >= 2 && buf[i-1] == '\n' && buf[i-2] == '\r' ) {
            buf[i-2] = '\n';
            i--;
        }
        else if ( i == 0 || (i >= 1 && buf[i-1] != '\n') ) {
            buf[i++] = '\n';
        }
        buf[i] = '\0';
    }

#	if defined (DEBUG_PARSER)
    if (foundEOF == true)
        PRINTF("Parser gave flex EOF\n");
    else
        PRINTF("Parser gave flex line(s):\n %s", buf);
#	endif
}


/** This function gets help info about a symbol */
int Parser::help(RbString *symbol) const {

    std::ostringstream msg;

#	if defined DEBUG_PARSER
    // Print syntax tree 
    std::cerr << std::endl;
    std::cerr << "Parser trying to get help for symbol '" << std::string(*symbol) << "'";
    std::cerr << std::endl;
#	endif

    // Get some help
    Help& userHelp = Help::getHelp();
    if ( userHelp.isUserHelpAvailable() == true && userHelp.isHelpAvailableForQuery( std::string( *symbol ) ) == true )
        {
        std::string hStr = userHelp.formatHelpString(std::string(*symbol), 100);
        UserInterface::userInterface().output(hStr, false);
        }
    else {
        if (userHelp.isUserHelpAvailable() == false)
            RBOUT("User help is unavailable");
        else if ( userHelp.isHelpAvailableForQuery(std::string(*symbol)) == false )
            RBOUT("Help unavailable for \"" + std::string(*symbol) + "\"");

        std::vector<const RbFunction*> functions = Workspace::userWorkspace().getFunctionTable()->findFunctions( *symbol );
        if ( functions.size() != 0 ) {
            RBOUT( "Usage:" );
            for ( std::vector<const RbFunction*>::iterator i=functions.begin(); i!=functions.end(); i++ ) {
                RBOUT( (*i)->briefInfo() );
            }
        }
    }

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

    RbString* symbol;

    if ( root->isType(SyntaxFunctionCall_name) ) {
        symbol = static_cast<SyntaxFunctionCall*>( root )->getFunctionName()->clone();
    }
    else {
        msg << "I have no clue -- Bison was not supposed to ask me about this!";
        RBOUT(msg.str());
        delete root;
        return 1;
    }

    delete root;
    return help( symbol );
}


/**
 * Process command with the help of the Bison-generated code.
 *
 * @param   command     command string
 * @return  integer flag indicating status:
 *          1 - command is incomplete
 *          2 - syntax error
 *          0 - command syntactically correct
 *
 * @note If an exception occurs on a syntactically correct
 *       statement, the exception msg is printed here and
 *       the return signal is set to 0. This might change
 *       in the future.
 */
int Parser::processCommand(std::string& command) {

    extern int yyparse(void);   // Defined in grammar.tab.cpp (from gammar.y)

    // Append command to stream where flex can find it
    rrcommand.str(rrcommand.str() + command);

    // Call Bison code, which calls Flex code, which calls rrinput
    PRINTF("\nCalling bison with rrcommand:\n'%s'\n", rrcommand.str().c_str());
    foundNewline = false;
    foundEOF = false;
    foundErrorBeforeEnd = false;

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

        // We printed a message so we dealt with the problem
        return 0;
    }

    if (result == 0 || result == 2) {
        PRINTF("Parser resetting command string\n\n");
        rrcommand.str("");
        rrcommand.clear();  // Clear any error flags
        return 0;
    }
    else if ( foundErrorBeforeEnd == true ) {
        PRINTF("Syntax error occurred in parsing or executing the statement; resetting command string\n");
        RBOUT( "Syntax error" );
        rrcommand.str("");
        rrcommand.clear();
        return 2;
    }
    else if ( foundNewline == true && foundEOF == false ) {
        PRINTF("Incomplete command ending with inappropriate newline; stripping newline and appending to command string\n");
        rrcommand.str(rrcommand.str() + " ");
        rrcommand.clear();
        return 1;
    }
    else /* if ( foundNewline == true && foundEOF == true ) */ {
        PRINTF("Incomplete command ending with appropriate newline; keeping newline and appending to command string\n");
        rrcommand.str(rrcommand.str() + "\n ");
        rrcommand.clear();
        return 1;
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


