#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxControlAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Basic constructor from lef-hand side and right-hand side expressions */
SyntaxControlAssignment::SyntaxControlAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/** Deep copy constructor */
SyntaxControlAssignment::SyntaxControlAssignment( const SyntaxControlAssignment& x ) :
    SyntaxElement( x ),
    lhsExpression( x.lhsExpression->clone() ),
    rhsExpression( x.rhsExpression->clone() )
{
}


/** Destructor deletes operands */
SyntaxControlAssignment::~SyntaxControlAssignment( void )
{
    delete lhsExpression;
    delete rhsExpression;
}


/** Assignment operator */
SyntaxControlAssignment& SyntaxControlAssignment::operator=( const SyntaxControlAssignment& x )
{
    if ( this != &x ) {
        
        delete lhsExpression;
        delete rhsExpression;
        
        lhsExpression = x.lhsExpression->clone();
        rhsExpression = x.rhsExpression->clone();
    }
    
    return (*this);
}


/** Type-safe clone of the syntax element */
SyntaxControlAssignment* SyntaxControlAssignment::clone () const
{
    return new SyntaxControlAssignment( *this );
}


/**
 * Get semantic value. The semantic value of a control assignment is identical to
 * that of a constant assignment, the only difference being that the control
 * variable flag of the resulting variable will be set to true.
 *
 * Note that the return variable is not set to a control variable, in case it is
 * used in further assignments of other types.
 */
RevPtr<Variable> SyntaxControlAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating control assignment\n" );
#endif
    
    // Declare variable storing the return value of the assignment expression
    RevPtr<Variable> theVariable;
    
    // Get the rhs expression wrapped and executed into a variable.
    theVariable = rhsExpression->evaluateContent( env );
    
    // Get variable slot from lhs
    RevPtr<Variable> theSlot;
    theSlot = lhsExpression->evaluateLHSContent( env, theVariable->getRevObject().getType() );
    
    // Get a reference to the Rev object value
    const RevObject& value = theVariable->getRevObject();
    
    // Perform type conversion if needed, otherwise just clone the value object
    RevObject* newValue;
    if ( !value.getTypeSpec().isDerivedOf( theSlot->getRevObjectTypeSpec() ) )
    {
        // We are not of a derived type (or the same type) so we need to cast
        // This is a one-time type conversion, so we set the once flag to true
        if (value.isConvertibleTo( theSlot->getRevObjectTypeSpec(), true ) )
        {
            newValue = value.convertTo( theSlot->getRevObjectTypeSpec() );
        }
        else
        {
            std::ostringstream msg;
            msg << "Cannot assign variable '" << theSlot->getName() << "' with value of type '" << value.getTypeSpec().getType() << "'" << std::endl;
            msg << " because the variable requires type '" << theSlot->getRevObjectTypeSpec().getType() << "'" << std::endl;
            throw RbException( msg );
        }
    }
    else
    {
        newValue = value.clone();
        newValue->makeConstantValue();  // We cannot trust evaluateContent to return a constant variable
    }
    
    // Fill the slot with newValue. The variable itself will be
    // passed on as the semantic value of the statement and can
    // be used in further assignments.
    theSlot->setRevObject( newValue );
    theSlot->setControlVarState( true );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    // Return variable
    return theVariable;
}


/** This is an assignment, return true. */
bool SyntaxControlAssignment::isAssignment( void ) const
{
    return true;
}



/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxControlAssignment::isFunctionSafe( const Environment& env ) const
{
    // Check lhs and rhs expressions
    if ( !lhsExpression->isFunctionSafe( env ) || !rhsExpression->isFunctionSafe( env ) )
        return false;

    // Check whether assignment is to external variable (not function safe)
    if ( lhsExpression->retrievesExternVar( env ) )
        return false;

    // All tests passed
    return true;
}


/** Print info about the syntax element */
void SyntaxControlAssignment::printValue( std::ostream& o ) const
{
    o << "SyntaxControlAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue( o );
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue( o );
    o << std::endl;
}


