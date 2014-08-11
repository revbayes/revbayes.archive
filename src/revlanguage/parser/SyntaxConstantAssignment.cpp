#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxConstantAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Basic constructor from lef-hand side and right-hand side expressions */
SyntaxConstantAssignment::SyntaxConstantAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/** Deep copy constructor */
SyntaxConstantAssignment::SyntaxConstantAssignment( const SyntaxConstantAssignment& x ) :
    SyntaxElement( x ),
    lhsExpression( x.lhsExpression->clone() ),
    rhsExpression( x.rhsExpression->clone() )
{
}


/** Destructor deletes operands */
SyntaxConstantAssignment::~SyntaxConstantAssignment( void )
{
    delete lhsExpression;
    delete rhsExpression;
}


/** Assignment operator */
SyntaxConstantAssignment& SyntaxConstantAssignment::operator=( const SyntaxConstantAssignment& x )
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
SyntaxConstantAssignment* SyntaxConstantAssignment::clone () const
{
    return new SyntaxConstantAssignment( *this );
}


/**
 * Get semantic value. The semantic value of a constant assignment is the same
 * independent of grammatical context, so we only need one evaluate function.
 * We first evaluate the lhs content of the lhs expression. If it is a variable,
 * and the variable does not exist in the workspace, the variable will be created
 * and inserted into the workspace (the environment) before being returned as the
 * content of the lhs expression. Other expressions will typically return temporary
 * variables that will be lost after the assignment, but it could be used in some
 * contexts. For instance, it might be used in a chain assignment or in passing a
 * variable to a function.
 */
RevPtr<Variable> SyntaxConstantAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating constant assignment\n" );
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
        if (value.isConvertibleTo( theSlot->getRevObjectTypeSpec() ) )
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

    // Fill the slot with newValue
    theSlot->setRevObject( newValue );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    // We return the rhs variable itself as the semantic value of the
    // assignment statement. It can be used in further assignments.
    return theVariable;
}


/** This is an assignment, return true. */
bool SyntaxConstantAssignment::isAssignment( void ) const
{
    return true;
}



/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxConstantAssignment::isFunctionSafe( const Environment& env ) const
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
void SyntaxConstantAssignment::printValue( std::ostream& o ) const
{
    o << "SyntaxConstantAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue( o );
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue( o );
    o << std::endl;
}


