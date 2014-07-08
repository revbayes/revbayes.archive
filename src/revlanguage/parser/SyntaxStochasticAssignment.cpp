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
#include "SyntaxStochasticAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxStochasticAssignment::SyntaxStochasticAssignment(SyntaxVariable* var, SyntaxElement* expr) :
    SyntaxElement(), variable(var), functionCall(NULL), expression(expr)
{
}


/** Construct from operator type, function call and expression */
SyntaxStochasticAssignment::SyntaxStochasticAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr) :
    SyntaxElement(), variable(NULL), functionCall(fxnCall), expression(expr)
{
}


/** Deep copy constructor */
SyntaxStochasticAssignment::SyntaxStochasticAssignment(const SyntaxStochasticAssignment& x) : SyntaxElement(x), variable( NULL ), functionCall( NULL )
{
    if ( x.variable != NULL )
        variable   = x.variable->clone();
    
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    
    expression = x.expression->clone();
}


/** Destructor deletes operands */
SyntaxStochasticAssignment::~SyntaxStochasticAssignment()
{
    delete variable;
    delete functionCall;
    delete expression;
    
}


/** Assignment operator */
SyntaxStochasticAssignment& SyntaxStochasticAssignment::operator=(const SyntaxStochasticAssignment& x)
{
    if ( this != &x )
    {
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
SyntaxStochasticAssignment* SyntaxStochasticAssignment::clone () const
{
    return new SyntaxStochasticAssignment(*this);
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RevPtr<Variable> SyntaxStochasticAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating tilde assignment\n" );
#endif
    
    // Get variable from lhs
    RevPtr<Variable> theSlot;
    if ( variable != NULL )
        theSlot = variable->evaluateLHSContent( env );
    else
        theSlot = functionCall->evaluateContent( env );
    
    // Evaluate the rhs expression and wrap it into a dynamic variable
    RevPtr<Variable> theVariable = expression->evaluateDynamicContent(env);
        
    // Get distribution, which should be the return value of the rhs function
    const RevObject& exprValue = theVariable->getRevObject();
    if ( !exprValue.isTypeSpec( Distribution::getClassTypeSpec() ) ) 
    {
        throw RbException( "Expression on the right-hand-side of '~' did not return a distribution object." );
    }
    const Distribution &dist = dynamic_cast<const Distribution &>( exprValue );
        
    // Create new stochastic variable
    RevObject* rv = dist.createRandomVariable();
        
    // Fill the slot with the new stochastic variable
    theSlot->setRevObject( rv );
        
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    return theVariable;
}


/** This is an assignment, so return true */
bool SyntaxStochasticAssignment::isAssignment( void ) const
{
    return true;
}


/** Print info about the syntax element */
void SyntaxStochasticAssignment::printValue(std::ostream& o) const
{
    o << "SyntaxAssignExpr:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = ";
    expression->printValue(o);
    o << std::endl;
    
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the element on our right-hand-side and also to the variable itself (lhs).
 */
void SyntaxStochasticAssignment::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    expression->replaceVariableWithConstant(name, c);
    variable->replaceVariableWithConstant(name, c);
    
}


