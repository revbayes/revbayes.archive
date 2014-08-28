#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxAdditionAssignment.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;


/** 
 * Construct from lhs and rhs expressions.
 * Here only the object is instantiated and the variables set.
 *
 * \param[in]   lhsExpr     The left-hand side expression.
 * \param[in]   rhsExpr     The right-hand side expression.
 */
SyntaxAdditionAssignment::SyntaxAdditionAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/**
 * Deep copy constructor.
 */
SyntaxAdditionAssignment::SyntaxAdditionAssignment( const SyntaxAdditionAssignment& x ) :
    SyntaxElement(x)
{
    lhsExpression = x.lhsExpression->clone();
    rhsExpression = x.rhsExpression->clone();
}


/** 
 * Destructor deletes operands 
 */
SyntaxAdditionAssignment::~SyntaxAdditionAssignment() 
{
    delete lhsExpression;
    delete rhsExpression;
}


/** 
 * Assignment operator performing deep assignment.
 */
SyntaxAdditionAssignment& SyntaxAdditionAssignment::operator=( const SyntaxAdditionAssignment& x )
{
    if ( this != &x )
    {
        SyntaxElement::operator=( x );
        
        delete lhsExpression;
        delete rhsExpression;
        
        lhsExpression = x.lhsExpression->clone();
        rhsExpression = x.rhsExpression->clone();
    }
    
    return ( *this );
}


/**
 * The clone function is a convenience function to create proper copies of inherited objects.
 * That is, a.clone() will create a clone of type 'b' if the 'a' instance is of derived type 'b'.
 *
 * \return A new copy of myself
 */
SyntaxAdditionAssignment* SyntaxAdditionAssignment::clone () const 
{
    return new SyntaxAdditionAssignment( *this );
}


/** 
 * Evaluate the content of this syntax element. This will perform an
 * addition assignment operation.
 */
RevPtr<Variable> SyntaxAdditionAssignment::evaluateContent( Environment& env ) 
{
#ifdef DEBUG_PARSER
    printf( "Evaluating addition assignment\n" );
#endif
    
    // Get variable from lhs. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<Variable> theVariable = lhsExpression->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in addition assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid addition assignment to dynamic variable" );
    
    // Record whether it is a control variable
    bool isControlVar = theVariable->isControlVar();
    
    // Get a reference to the lhs value object
    const RevObject& lhs_value = theVariable->getRevObject();
    
    // Evaluate the rhs expression
    const RevPtr<Variable>& rhs = rhsExpression->evaluateContent( env );
    if ( rhs == NULL )
        throw RbException( "Invalid NULL variable returned by rhs expression in addition assignment" );
    
    // Get a reference to the rhs value object
    const RevObject& rhs_value = rhs->getRevObject();
    
    // Generate result of the multiplication
    RevObject *newValue = lhs_value.add( rhs_value );
    
    // Fill the slot with the new variable
    theVariable->setRevObject( newValue );

    // Reset it as control variable, if it was a control variable before the assignment.
    // When we fill the slot, the control variable property is reset to false by default.
    if ( isControlVar )
        theVariable->setControlVarState( true );

#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    // Return the variable for further assignment
    return theVariable;
}


/**
 * Is this an assignment operation?
 * Sure it is.
 *
 * \return     TRUE.
 */
bool SyntaxAdditionAssignment::isAssignment( void ) const 
{
    return true;
}


/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxAdditionAssignment::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    // Check lhs and rhs expressions
    if ( !lhsExpression->isFunctionSafe( env, localVars ) || !rhsExpression->isFunctionSafe( env, localVars ) )
        return false;
    
    // Check whether assignment is to external variable (not function safe)
    if ( lhsExpression->retrievesExternVar( env, localVars, true ) )
        return false;

    // All tests passed
    return true;
}


/**
 * Print info about the syntax element 
 */
void SyntaxAdditionAssignment::printValue(std::ostream& o) const 
{
    o << "SyntaxAdditionAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue(o);
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue(o);
    o << std::endl;
}


