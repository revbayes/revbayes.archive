#include "RbException.h"
#include "RbOptions.h"
#include "SyntaxDeterministicAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/** Deep copy constructor */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment( const SyntaxDeterministicAssignment& x ) :
    SyntaxElement(x),
    lhsExpression( x. lhsExpression ),
    rhsExpression( x. rhsExpression )
{
}


/** Destructor deletes operands */
SyntaxDeterministicAssignment::~SyntaxDeterministicAssignment()
{
    delete lhsExpression;
    delete rhsExpression;
}


/** Assignment operator */
SyntaxDeterministicAssignment& SyntaxDeterministicAssignment::operator=( const SyntaxDeterministicAssignment& x )
{
    if ( this != &x ) {
        
        SyntaxElement::operator=( x );
        
        delete lhsExpression;
        delete rhsExpression;
        
        lhsExpression = x.lhsExpression->clone();
        rhsExpression = x.rhsExpression->clone();
    }
    
    return (*this);
}


/** Type-safe clone of syntax element */
SyntaxDeterministicAssignment* SyntaxDeterministicAssignment::clone () const
{
    return new SyntaxDeterministicAssignment( *this );
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RevPtr<Variable> SyntaxDeterministicAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating deterministic assignment\n" );
#endif
    
    // Get the rhs expression wrapped and executed into a variable.
    // We need to call evaluateDynamicContent to get some elements
    // to evaluate their semantic content properly
    RevPtr<Variable> theVariable = rhsExpression->evaluateDynamicContent(env);
    
    // Get variable slot from lhs using the evaluateLHSContent to get the
    // appropriate behavior in the variable syntax element class.
    RevPtr<Variable> theSlot = lhsExpression->evaluateLHSContent( env, theVariable->getRevObject().getType() );

    // Check if the variable returned from the rhs expression is a named
    // variable in the environment. If so, we want to create an indirect
    // reference to it; otherwise, we want to fill the slot with a clone
    // of the variable returned by the rhs expression.
    if ( theVariable->getName() != "" )
        theSlot->setRevObject( theVariable->getRevObject().makeIndirectReference() );
    else
        theSlot->setRevObject( theVariable->getRevObject().clone() );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    // We return the rhs variable itself as the semantic value of the
    // assignment statement. It can be used in further assignments.
    return theVariable;
}


/** This is an assignment statement, so return true. */
bool SyntaxDeterministicAssignment::isAssignment( void ) const
{
    return true;
}


/**
 * Is the syntax element safe for use in a function (as
 * opposed to a procedure)? The assignment is safe
 * if its lhs and rhs expressions are safe, and the
 * assignment is not to an external variable.
 */
bool SyntaxDeterministicAssignment::isFunctionSafe( const Environment& env ) const
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


/** Print info about the syntax element */
void SyntaxDeterministicAssignment::printValue(std::ostream& o) const
{
    o << "SyntaxDeterministicAssignment:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue(o);
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue(o);
    o << std::endl;
}


