/*!
 * \file
 * This file contains the implementation of FunctionCall, which is
 * used to hold function calls in the syntax tree.
 *
 * \brief Implementation of FunctionCall
 *
 * (c) Copyright 2009- under GPL version 3
 * \date Last modified: $Date$
 * \author The REvBayes core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "SyntaxFunctionCall.h"

using namespace std;

//! Constructor set the member variable operator. Then it checks that the
//  function exists and that the arguments are valid. A local copy of the
//  function is created and set up with its arguments.
SyntaxFunctionCall::SyntaxFunctionCall (SyntaxFunctionCall::operatorT op, const string functionName, list<SyntaxElement*> arguments)
    : SyntaxElement(), operator(op) {

    /* Look up the function in the symbol table */
    if ( operator == DISTRIBUTION ) {
        function = (RbDistributionFunction*) symbolTable.find(functionName);
        if (!function) {
            // msg <<  "No distribution function '" << functionName << "' exists";
        }
    else if ( operator == ACCESSOR ) {
        function = (RbAccessorFunction*) symbolTable.find(functionName);
        if (!function) {
            // msg <<  "No accessor function '" << functionName << "' exists";
        }
    else if ( operator == FUNCTION ) {
        function = (RbFunction*) symbolTable.find(functionName);
        if (!function) {
            // msg <<  "No function '" << functionName << "' exists";
        }
    }

    /* Make a local copy */
    function = function->copy();

    /* Set and check arguments */
    if ( !function->setArguments(arguments) {
        delete function;    // unless destructor is called??
        // msg << "Nonmatching arguments";  // more error messages in the function itself
    }

    /* Finally connect syntax tree */
    for (list<SyntaxElement *>::iterator i=arguments.begin(); i!= arguments.end(); i++) {
        addParentNode (i);
        i->addChildNode(this);
    }
}

//! Destructor deletes the function. Everything else is dealt with by the DAGNode
//  class.
SyntaxFunctionCall::~SyntaxFunctionCall() {

    delete function;
}

RbObject *SyntaxFunctionCall::getValue(void) {

    if ( touched ) {
        changed = true;
        delete storedValue;
        storedValue = value;
        value = function->execute();
    }
    return value;
}
