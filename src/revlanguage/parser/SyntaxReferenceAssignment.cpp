#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxReferenceAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Basic constructor from lef-hand side and right-hand side expressions */
SyntaxReferenceAssignment::SyntaxReferenceAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/** Deep copy constructor */
SyntaxReferenceAssignment::SyntaxReferenceAssignment( const SyntaxReferenceAssignment& x ) :
    SyntaxElement( x ),
    lhsExpression( x.lhsExpression->clone() ),
    rhsExpression( x.rhsExpression->clone() )
{
}


/** Destructor deletes operands */
SyntaxReferenceAssignment::~SyntaxReferenceAssignment( void )
{
    delete lhsExpression;
    delete rhsExpression;
}


/** Assignment operator */
SyntaxReferenceAssignment& SyntaxReferenceAssignment::operator=( const SyntaxReferenceAssignment& x )
{
    if ( this != &x ) {
        
        delete lhsExpression;
        delete rhsExpression;
        
        lhsExpression = x.lhsExpression->clone();
        rhsExpression = x.rhsExpression->clone();
    }
    
    return ( *this );
}


/** Type-safe clone of the syntax element */
SyntaxReferenceAssignment* SyntaxReferenceAssignment::clone () const
{
    return new SyntaxReferenceAssignment( *this );
}


/**
 * Get semantic value. When evaluating the semantic value of a reference assignment,
 * we first evaluate the rhs expression as if it were a constant expression. Among
 * other things, this makes it possible for us to create references to control
 * variables, which would be impossible if the rhs expression was evaluated as a
 * dynamic expression. We are interested in creating a reference to the variable
 * that results from evaluation of the rhs expression now.
 *
 * Note that the return variable is variable returned by the rhs expression.
 * We need not clone it.
 */
RevPtr<Variable> SyntaxReferenceAssignment::evaluateContent( Environment& env, bool dynamic )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating reference assignment\n" );
#endif
    
    // Declare variable storing the return value of the assignment expression
    RevPtr<Variable> theVariable;
    
    // Get the rhs expression wrapped and executed into a variable.
    theVariable = rhsExpression->evaluateContent( env );
    
    // Get variable slot from lhs
    RevPtr<Variable> theSlot;
    theSlot = lhsExpression->evaluateLHSContent( env, theVariable->getRevObject().getType() );
    
    // Make the slot a reference to the rhs expression variable.
    theSlot->makeReference( theVariable );

#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    // Return variable
    return theVariable;
}


/** This is an assignment, return true. */
bool SyntaxReferenceAssignment::isAssignment( void ) const
{
    return true;
}


/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxReferenceAssignment::isFunctionSafe( const Environment& env, std::set<std::string>& localVars ) const
{
    // Check lhs and rhs expressions
    if ( !lhsExpression->isFunctionSafe( env, localVars ) || !rhsExpression->isFunctionSafe( env, localVars ) )
        return false;
    
    // Check whether assignment is to external variable (not function-safe)
    if ( lhsExpression->retrievesExternVar( env, localVars, true ) )
        return false;
    
    // All tests passed
    return true;
}


/** Print info about the syntax element */
void SyntaxReferenceAssignment::printValue( std::ostream& o ) const
{
    o << "SyntaxReferenceAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue( o );
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue( o );
    o << std::endl;
}


