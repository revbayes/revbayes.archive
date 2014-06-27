#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "VariableSlot.h"
#include "SyntaxMultiplicationAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** 
 * Construct from variable and expression. 
 * Here only the object is instanciated and the variables set.
 *
 * \param[in]   var         The left hand side variable.
 * \param[in]   expr        The expression on the right hand side.
 */
SyntaxMultiplicationAssignment::SyntaxMultiplicationAssignment(SyntaxVariable* var, SyntaxElement* expr) : SyntaxElement(), 
variable(var), 
functionCall(NULL), 
expression(expr) 
{
    
}



/** 
 * Construct from function call and expression. 
 * Here only the object is instanciated and the variables set.
 *
 * \param[in]   fxnCall     The function call returning the left hand side variable.
 * \param[in]   expr        The expression on the right hand side.
 */
SyntaxMultiplicationAssignment::SyntaxMultiplicationAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr) : SyntaxElement(),
variable(NULL), 
functionCall(fxnCall), 
expression(expr) 
{
    
}


/** 
 * Deep copy constructor.
 */
SyntaxMultiplicationAssignment::SyntaxMultiplicationAssignment(const SyntaxMultiplicationAssignment& x) : SyntaxElement(x) 
{
    
    if ( x.variable != NULL )
        variable   = x.variable->clone();
    
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    
    expression = x.expression->clone();
}


/** 
 * Destructor deletes operands 
 */
SyntaxMultiplicationAssignment::~SyntaxMultiplicationAssignment() 
{
    
    delete variable;
    delete functionCall;
    delete expression;
    
}


/** 
 * Assignment operator performing a deep assignment.
 */
SyntaxMultiplicationAssignment& SyntaxMultiplicationAssignment::operator=(const SyntaxMultiplicationAssignment& x) 
{
    
    if ( this != &x ) 
    {
        
        delete functionCall;
        delete variable;
        delete expression;
        
        functionCall = NULL;
        variable = NULL;
        
        if ( x.variable != NULL )
            variable   = x.variable->clone();
        
        if ( x.functionCall != NULL )
            functionCall = x.functionCall->clone();
        
        expression = x.expression->clone();
    }
    
    return (*this);
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
SyntaxMultiplicationAssignment* SyntaxMultiplicationAssignment::clone () const 
{
    
    return new SyntaxMultiplicationAssignment(*this);
}


/** 
 * Evaluate the content of this syntax element.
 * This will perform an Multiplication assignment operation.
 */
RbPtr<Variable> SyntaxMultiplicationAssignment::evaluateContent( Environment& env ) 
{
    
#ifdef DEBUG_PARSER
    printf( "Evaluating multiplication assign expression\n" );
#endif
    
    // Get variable info from lhs
    RbPtr<Variable> theVariable = variable->createVariable( env );
    
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in multiplication assignment" );
    
    const RevObject& lhs_value = theVariable->getValue();
    
    
    // Calculate the value of the rhs expression
    const RbPtr<Variable>& rhs = expression->evaluateContent( env );
    if ( rhs == NULL )
        throw RbException( "Invalid NULL variable returned by rhs expression in multiplication assignment" );
    
    // fill the slot with the new variable
    const RevObject& rhs_value = rhs->getValue();
    
    RevObject *newValue = lhs_value.multiply( rhs_value );
    
    // set the value of the variable
    theVariable->setValue( newValue );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    return theVariable;
}


/**
 * Is this an assignment operation?
 * Sure it is.
 *
 * \return     TRUE.
 */
bool SyntaxMultiplicationAssignment::isAssignment( void ) const 
{
    return true;
}



/** 
 * Print info about the syntax element 
 */
void SyntaxMultiplicationAssignment::printValue(std::ostream& o) const 
{
    
    o << "SyntaxMultiplicationAssignment:" << std::endl;
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
void SyntaxMultiplicationAssignment::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    expression->replaceVariableWithConstant(name, c);
    variable->replaceVariableWithConstant(name, c);
}


