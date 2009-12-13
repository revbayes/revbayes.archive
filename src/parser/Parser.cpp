/**
 * @file
 * This file contains the implementation of Parser, which is
 * used to execute syntax trees and get help info.
 *
 * @brief Implementation of Parser
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Parser.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbString.h"
#include "SyntaxElement.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxVariable.h"
#include "UserInterface.h"

#include <sstream>

/** Trigger printing of PRINTF debug statements by defining DEBUG_PARSER */
#ifndef NDEBUG
#ifdef DEBUG_PARSER
#define PRINTF printf
#else
#define PRINTF(...)
#endif
#endif


/** This function causes recursive execution of a syntax tree by calling the root to get its value */
int Parser::execute(SyntaxElement *root) const {

#ifdef DEBUG_PARSER
    /* No implemeneted recursive print yet, sorry
    //! Print syntax tree
    std::cerr << std::endl;
    std::cerr << "Syntax tree before execution:\n";
    root->print(cerr);
    std::cerr << std::endl;
    */
#endif

    // Declare a variable for the result
    RbObject *result;
    
    //! Execute syntax tree
    try {
        result = root->getValue();
    }
    catch(RbException& rbException) {

        // Catch a quit request
        if (rbException.getExceptionType() == RbException::QUIT)
            exit(0);

        // All other exceptions
        std::ostringstream msg;
        rbException.printValue(msg);
        msg << std::endl;
        UserInterface::userInterface().output(msg.str());

        // Return failure
        return 1;
    }

    // Print result if the root is not an assign expression
    if (result != NULL && !root->isType(SyntaxAssignExpr_name)) {
        std::ostringstream msg;
        result->printValue(msg);
        msg << std::endl;
        UserInterface::userInterface().output(msg.str());
    }

    // Delete syntax tree and result
    delete result;
    delete root;

    // Return success
    return 0;
}


/** This function gets info about a symbol */
int Parser::info(RbString *symbol) const {

    std::ostringstream msg;

#if defined DEBUG_PARSER
    /*
    Not yet, sorry;
    // Print syntax tree recursively
    std::cerr << std::endl;
    std::cerr << "Syntax tree before info:\n";
    root->print(std::cerr);
    std::cerr << std::endl;
    */
#endif

    // Get some help when we have some help to get

    // Print some silly stuff for now
    msg << "Sorry, I don't know anything about '" << std::string(*symbol);
    msg << "'; I wish I did because it looks interesting" << std::endl;

    // Output the message
    UserInterface::userInterface().output(msg.str());

    // Delete the symbol
    delete symbol;

    // Return success
    return 0;
}


/** This function prints info about a function if it sees a function call */
int Parser::info(SyntaxElement *root) const {

    std::ostringstream msg;

#if defined DEBUG_PARSER
    /*
    Not yet, sorry;
    // Print syntax tree recursively
    std::cerr << std::endl;
    std::cerr << "Syntax tree before info:\n";
    root->print(std::cerr);
    std::cerr << std::endl;
    */
#endif

    RbString symbol;

    if ( root->isType(SyntaxFunctionCall_name) ) {
        symbol = *((SyntaxFunctionCall*)(root))->getFunctionName();
    }
    else {
        msg << "I have no clue -- Bison was not supposed to ask me about this!";
        UserInterface::userInterface().output(msg.str());
        delete root;
        return 1;
    }

    // Get some help

    // Print some silly stuff for now
    msg << "Sorry, I don't know anything about '" << std::string(symbol);
    msg  << "'; I wish I did because it looks interesting" << std::endl;

    // Output the message
    UserInterface::userInterface().output(msg.str());

    // Delete syntax tree
    delete root;

    // Return success
    return 0;
}

