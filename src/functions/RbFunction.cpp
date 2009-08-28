/**
 * @file
 * This file contains the implementation of some functions in
 * RbFunction, the interface for functions in REvBayes.
 *
 * @brief Implementation of RbFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#include "RbDataType.h"
#include "RbFunction.h"
#include "SyntaxLabeledExpr.h"
#include <list>

using namespace std;

/** Constructor that checks argument rules for consistency. */
RbFunction::RbFunction(const ArgumentRule *argRules)
    : RbObject(), result(NULL) {

}

/** Set and check arguments based on argument rules. We use the
 *  following logic:
 *
 *  1. Check argument rules for consistency (programming errors).
 *  2. Check if there are too many arguments.
 *  3. Create a null list of expressions of the desired length.
 *     This list will hold the argument expressions in the
 *     order expected by the function in later calls.
 *  4. Process the provided labeled expressions in order. If the
 *     label is empty, then the expression goes directly into
 *     the corresponding argument expression. If that expression
 *     has already been filled, it is an error. If the label is
 *     not empty, then put the expression in the position corres-
 *     ponding to the label. If the label does not exist among
 *     the rules, or if this position has already been filled,
 *     it is an error.
 *  5. Finally use any default values to fill in the remaining
 *     expressions. If there are still NULL expressions, it is
 *     an error.
 *
 *  Only override this if your function uses a non-standard set
 *  of arguments, for instance a variable number of arguments.
 */
bool RbFunction::setArguments(list<SyntaxLabeledExpr*> args) {

    ArgumentRule*   argRules = getArgumentRules();

    /* 1. Check for duplicate labels */
    for (int i=0; argRules[i]!=ArgumentRule(); i++) {
        for (int j=1; argRules[j]!=ArgumentRule(); j++) {
            if ( argRules[i].label() != "" && argRules[i].label() == argRules[j].label() )
                ; // errmsg << "Duplicate argument labels '" << argRules[i].label() << "'.";
        }
    }

    /* 2. Check if there are too many arguments */
    if ( args.size() > argumentRules.size() ) {
        if ( args.size() == 0 )
            ; // errmsg << "Function does not take any arguments." << endl;
        else if ( args.size() == 1 )
            ; // errmsg << "Function only takes one argument." << endl;
        else if ( args.size() > 1 )
            ; // errmsg << "Function expects " << argumentRules.size() << " arguments; you provided " << args.size() << "." << endl;
    }

    /* 3. Create null list of expressions */
    arguments = list<SyntaxExpression*>(argumentRules.size());
    for (list<SyntaxExpression*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
        (*i) = NULL;

    /* 4. Fill in with the provided argument expressions */
    int count = 0;
    for (list<SyntaxLabeledExpr*>::iterator i=args.begin(); i!=args.end(); i++, count++) {
        
        /* First get the expression and find the relevant argument rule and index */
        int argIndex        = -1;
        SyntaxElement* expr = (*i)->getExpression();
        string label        = (*i)->getLabel();
        ArgumentRule* theRule;
        if ( label == "" ) {
            theRule = &argumentRules[count];
        }
        else /* if ( label != "" ) */ {
            for (argIndex=0; argumentRules[argIndex]!=ArgumentRule(); argIndex++) {
                if ( argumentRules[argIndex].getLabel() == label )
                    break;
            }
            if ( argumentRules[argIndex]==ArgumentRule() )
                ; // errmsg << "No argument with label '" << label "' expected." << endl;
            argIndex = count;
            theRule  = &argumentRules[argIndex];
        }

        /* Check that the argument expression is empty */
        if ( arguments[argIndex] != NULL )
            ; // errmsg << "Argument '" << theRule->getLabel() << "' is provided more than once." << endl;
 
        /* Now check the type */
        if ( expr->getDataType()->getType() != theRule->geType() )
            ; // errmsg << "Argument '" << theRule->getLabel() << "' should be of type '" << theRule->getType() << "'.";
            ; // errmsg << "The provided value is of type '" << expr->getDataType().getType() << "'." << endl;

        /* Finally fill in the argument expression */
        arguments[argIndex] = expr;
    }

    /* 5. Fill in any remaining arguments with default values */
    for (int index = 0, list<SyntaxElement*>::iterator i=arguments.begin(); i!=arguments.end(); i++, index++) {
        if ( (*i) == NULL ) {
            if ( argumentRules[index].defaultValue() == NULL )
                ; // errmsg << "No default value for argument '" << argumentRules[index] << "'.";
            (*i) = new SyntaxConstant(argumentRules[count].defaultValue());
        }
    }
}
