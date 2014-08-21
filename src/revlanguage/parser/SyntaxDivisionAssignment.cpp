#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxDivisionAssignment.h"

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
SyntaxDivisionAssignment::SyntaxDivisionAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/**
 * Deep copy constructor.
 */
SyntaxDivisionAssignment::SyntaxDivisionAssignment( const SyntaxDivisionAssignment& x ) :
SyntaxElement(x)
{
    lhsExpression = x.lhsExpression->clone();
    rhsExpression = x.rhsExpression->clone();
}


/**
 * Destructor deletes operands
 */
SyntaxDivisionAssignment::~SyntaxDivisionAssignment()
{
    delete lhsExpression;
    delete rhsExpression;
}


/**
 * Assignment operator performing deep assignment.
 */
SyntaxDivisionAssignment& SyntaxDivisionAssignment::operator=( const SyntaxDivisionAssignment& x )
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
SyntaxDivisionAssignment* SyntaxDivisionAssignment::clone () const
{
    return new SyntaxDivisionAssignment( *this );
}


/**
 * Evaluate the content of this syntax element. This will perform a
 * division assignment operation.
 */
RevPtr<Variable> SyntaxDivisionAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating division assignment\n" );
#endif
    
    // Get variable from lhs. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<Variable> theVariable = lhsExpression->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in division assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid division assignment to dynamic variable" );
    
    // Record whether it is a control variable
    bool isControlVar = theVariable->isControlVar();
    
    // Get a reference to the lhs value object
    const RevObject& lhs_value = theVariable->getRevObject();
    
    // Evaluate the rhs expression
    const RevPtr<Variable>& rhs = rhsExpression->evaluateContent( env );
    if ( rhs == NULL )
        throw RbException( "Invalid NULL variable returned by rhs expression in division assignment" );
    
    // Get a reference to the rhs value object
    const RevObject& rhs_value = rhs->getRevObject();
    
    // Generate result of the multiplication
    RevObject *newValue = lhs_value.divide( rhs_value );
    
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
bool SyntaxDivisionAssignment::isAssignment( void ) const
{
    return true;
}


/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxDivisionAssignment::isFunctionSafe( const Environment& env ) const
{
    // Check lhs and rhs expressions
    if ( !lhsExpression->isFunctionSafe( env ) || !rhsExpression->isFunctionSafe( env ) )
        return false;
    
    // Check whether assignment is to external variable (not function-safe)
    if ( lhsExpression->retrievesExternVar( env ) )
        return false;
    
    // All tests passed
    return true;
}

/**
 * Print info about the syntax element
 */
void SyntaxDivisionAssignment::printValue(std::ostream& o) const
{
    o << "SyntaxDivisionAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue(o);
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue(o);
    o << std::endl;
}


