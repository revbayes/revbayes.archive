#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxFunctionCall.h"
#include "SyntaxDivisionAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** 
 * Construct from variable and expression. 
 * Here only the object is instantiated and the variables set.
 *
 * \param[in]   var         The left hand side variable.
 * \param[in]   expr        The expression on the right hand side.
 */
SyntaxDivisionAssignment::SyntaxDivisionAssignment(SyntaxVariable* var, SyntaxElement* expr) : SyntaxElement(), 
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
SyntaxDivisionAssignment::SyntaxDivisionAssignment(SyntaxFunctionCall* fxnCall, SyntaxElement* expr) :
    SyntaxElement(),
    variable(NULL),
    functionCall(fxnCall),
    expression(expr)
{
}


/** 
 * Deep copy constructor.
 */
SyntaxDivisionAssignment::SyntaxDivisionAssignment(const SyntaxDivisionAssignment& x) :
    SyntaxElement(x)
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
SyntaxDivisionAssignment::~SyntaxDivisionAssignment() 
{
    delete variable;
    delete functionCall;
    delete expression;
}


/** 
 * Assignment operator performing a deep assignment.
 */
SyntaxDivisionAssignment& SyntaxDivisionAssignment::operator=(const SyntaxDivisionAssignment& x) 
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
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of myself
 */
SyntaxDivisionAssignment* SyntaxDivisionAssignment::clone () const 
{
    return new SyntaxDivisionAssignment(*this);
}


/** 
 * Evaluate the content of this syntax element.
 * This will perform a division assignment operation.
 */
RevPtr<Variable> SyntaxDivisionAssignment::evaluateContent( Environment& env ) 
{
#ifdef DEBUG_PARSER
    printf( "Evaluating division assignment\n" );
#endif
    
    // Get variable from lhs. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<Variable> theVariable = variable->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in subtraction assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid subtraction assignment to dynamic variable" );
    
    // Get a reference to the lhs value object
    const RevObject& lhs_value = theVariable->getRevObject();
    
    // Evaluate the rhs expression
    const RevPtr<Variable>& rhs = expression->evaluateContent( env );
    if ( rhs == NULL )
        throw RbException( "Invalid NULL variable returned by rhs expression in subtraction assignment" );
    
    // Get a reference to the rhs value object
    const RevObject& rhs_value = rhs->getRevObject();
    
    // Generate result of the multiplication
    RevObject *newValue = lhs_value.divide( rhs_value );
    
    // Fill the slot with the new variable
    theVariable->setRevObject( newValue );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    return theVariable;
}


/**
 * This is an assignment operation, so return true.
 *
 * \return     TRUE.
 */
bool SyntaxDivisionAssignment::isAssignment( void ) const 
{
    return true;
}


/** 
 * Print info about the syntax element 
 */
void SyntaxDivisionAssignment::printValue(std::ostream& o) const 
{
    o << "SyntaxDivisionAssignment:" << std::endl;
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
void SyntaxDivisionAssignment::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    expression->replaceVariableWithConstant(name, c);
    variable->replaceVariableWithConstant(name, c);
}


