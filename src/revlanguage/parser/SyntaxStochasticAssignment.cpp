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
SyntaxStochasticAssignment::SyntaxStochasticAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) : SyntaxAssignment(lhsExpr,rhsExpr)
{
    
}


/** Destructor deletes operands */
SyntaxStochasticAssignment::~SyntaxStochasticAssignment()
{

}


/** Type-safe clone of syntax element */
SyntaxStochasticAssignment* SyntaxStochasticAssignment::clone () const
{
    return new SyntaxStochasticAssignment( *this );
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
void SyntaxStochasticAssignment::assign(RevPtr<RevVariable> &lhs, RevPtr<RevVariable> &rhs)
{
#ifdef DEBUG_PARSER
    printf( "Evaluating tilde assignment\n" );
#endif
    
    // Get distribution, which should be the return value of the rhs function
    const RevObject& exprValue = rhs->getRevObject();
    if ( !exprValue.isType( Distribution::getClassTypeSpec() ) ) 
    {
        throw RbException( "Expression on the right-hand-side of '~' did not return a distribution object." );
    }
    const Distribution &dist = dynamic_cast<const Distribution &>( exprValue );
        
    // Create new stochastic variable
    RevObject* rv = dist.createRandomVariable();
    
    // Fill the slot with the new stochastic variable
    lhs->replaceRevObject( rv );
    
    // make sure all the implicitly created variables got a correct name
    RevBayesCore::DagNode* theNode = lhs->getRevObject().getDagNode();
    theNode->setParentNamePrefix( theNode->getName() );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
}


/** Should the rhs be evaluated dynamically? Yes, because we want the distribution. */
bool SyntaxStochasticAssignment::isDynamic( void )
{
    return true;
}

