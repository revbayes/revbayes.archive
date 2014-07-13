/**
 * @file
 * This file contains the implementation of SyntaxAssignExpr, which is
 * used to hold binary expressions in the syntax tree.
 *
 * @brief Implementation of SyntaxAssignExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-09-07 12:47:31 +0200 (Fri, 07 Sep 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: SyntaxAssignExpr.cpp 1801 2012-09-07 10:47:31Z hoehna $
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlDistribution.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxDeterministicAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(SyntaxVariable* var, SyntaxElement* expr) :
    SyntaxElement(), variable(var), functionCall(NULL), expression(expr)
{
}


/** Construct from operator type, function call and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr) :
    SyntaxElement(), variable(NULL), functionCall(fxnCall), expression(expr)
{
}


/** Deep copy constructor */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment(const SyntaxDeterministicAssignment& x) : SyntaxElement(x)
{
    if ( x.variable != NULL )
        variable   = x.variable->clone();
    
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    
    expression = x.expression->clone();
}


/** Destructor deletes operands */
SyntaxDeterministicAssignment::~SyntaxDeterministicAssignment()
{
    delete variable;
    delete functionCall;
    delete expression;
}


/** Assignment operator */
SyntaxDeterministicAssignment& SyntaxDeterministicAssignment::operator=(const SyntaxDeterministicAssignment& x)
{
    if ( this != &x ) {
        
        functionCall = NULL;
        variable = NULL;
        
        if ( x.variable != NULL )
            variable   = x.variable;
        
        if ( x.functionCall != NULL )
            functionCall = x.functionCall;
        
        expression = x.expression;
    }
    
    return (*this);
}


/** Clone syntax element */
SyntaxDeterministicAssignment* SyntaxDeterministicAssignment::clone () const
{
    return new SyntaxDeterministicAssignment(*this);
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RevPtr<Variable> SyntaxDeterministicAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating equation assignment\n" );
#endif
    
    // Declare variable storing the return value of the assignment expression
    RevPtr<Variable> theVariable;

    // Get the rhs expression wrapped and executed into a variable.
    // We need to call evaluateIndirectReferenceContent in case the rhs
    // is a variable. If not, evaluateIndirectReferenceContent will
    // fall back to evaluateDynamicContent.
    theVariable = expression->evaluateIndirectReferenceContent(env);
    
    // Get variable slot from lhs
    RevPtr<Variable> theSlot;
    if ( variable != NULL )
        theSlot = variable->evaluateLHSContent( env, theVariable->getRevObject().getType() );
    else
        theSlot = functionCall->evaluateContent( env );
    
    // Fill the slot with a clone of the variable. The variable itself
    // will be passed on as the semantic value of the statement and can
    // be used in further assignments.
    theSlot->setRevObject( theVariable->getRevObject().clone() );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    return theVariable;
}


/** This is an assignment statement, so return true. */
bool SyntaxDeterministicAssignment::isAssignment( void ) const
{
    return true;
}



/** Print info about the syntax element */
void SyntaxDeterministicAssignment::printValue(std::ostream& o) const
{
    o << "SyntaxDeterministicAssignment:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = ";
    expression->printValue(o);
    o << std::endl;
}


