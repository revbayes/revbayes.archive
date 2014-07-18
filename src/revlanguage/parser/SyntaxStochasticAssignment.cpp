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

/**
 * Basic constructor from lhs expression and
 * rhs expression.
 */
SyntaxStochasticAssignment::SyntaxStochasticAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) :
    SyntaxElement(),
    lhsExpression( lhsExpr ),
    rhsExpression( rhsExpr )
{
}


/** Deep copy constructor */
SyntaxStochasticAssignment::SyntaxStochasticAssignment( const SyntaxStochasticAssignment& x ) :
    SyntaxElement( x ),
    lhsExpression( x.lhsExpression->clone() ),
    rhsExpression( x.rhsExpression->clone() )
{
}


/** Destructor deletes operands */
SyntaxStochasticAssignment::~SyntaxStochasticAssignment()
{
    delete lhsExpression;
    delete rhsExpression;
}


/** Assignment operator */
SyntaxStochasticAssignment& SyntaxStochasticAssignment::operator=( const SyntaxStochasticAssignment& x )
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


/** Type-safe clone of syntax element */
SyntaxStochasticAssignment* SyntaxStochasticAssignment::clone () const
{
    return new SyntaxStochasticAssignment( *this );
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
RevPtr<Variable> SyntaxStochasticAssignment::evaluateContent( Environment& env )
{
#ifdef DEBUG_PARSER
    printf( "Evaluating tilde assignment\n" );
#endif
    
    // Evaluate the rhs expression and wrap it into a dynamic variable
    RevPtr<Variable> theVariable = rhsExpression->evaluateDynamicContent(env);
        
    // Get distribution, which should be the return value of the rhs function
    const RevObject& exprValue = theVariable->getRevObject();
    if ( !exprValue.isTypeSpec( Distribution::getClassTypeSpec() ) ) 
    {
        throw RbException( "Expression on the right-hand-side of '~' did not return a distribution object." );
    }
    const Distribution &dist = dynamic_cast<const Distribution &>( exprValue );
        
    // Create new stochastic variable
    RevObject* rv = dist.createRandomVariable();
        
    // Get variable slot from lhs
    RevPtr<Variable> theSlot = lhsExpression->evaluateLHSContent( env, rv->getType() );
    
    // Fill the slot with the new stochastic variable
    theSlot->setRevObject( rv );
        
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif    
    
    // Return the rhs variable for further assignment
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
    o << "SyntaxStochasticAssignExpr:" << std::endl;
    o << "lhsExpression = ";
    lhsExpression->printValue(o);
    o << std::endl;
    o << "rhsExpression = ";
    rhsExpression->printValue(o);
    o << std::endl;
}

