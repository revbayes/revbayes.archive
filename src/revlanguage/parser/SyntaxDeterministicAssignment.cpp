#include "RbException.h"
#include "RbOptions.h"
#include "SyntaxDeterministicAssignment.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** Construct from operator type, variable and expression */
SyntaxDeterministicAssignment::SyntaxDeterministicAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) : SyntaxAssignment(lhsExpr,rhsExpr)
{

}


/** Destructor deletes operands */
SyntaxDeterministicAssignment::~SyntaxDeterministicAssignment()
{

}


/** Type-safe clone of syntax element */
SyntaxDeterministicAssignment* SyntaxDeterministicAssignment::clone () const
{
    return new SyntaxDeterministicAssignment( *this );
}


/** Get semantic value: insert symbol and return the rhs value of the assignment */
void SyntaxDeterministicAssignment::assign(RevPtr<Variable> &lhs, RevPtr<Variable> &rhs)
{
#ifdef DEBUG_PARSER
    printf( "Evaluating deterministic assignment\n" );
#endif

    // Check if the variable returned from the rhs expression is a named
    // variable in the environment. If so, we want to create an indirect
    // reference to it; otherwise, we want to fill the slot with a clone
    // of the variable returned by the rhs expression.
    if ( rhs->getName() != "" )
    {
        lhs->setRevObject( rhs->getRevObject().makeIndirectReference() );
    }
    else
    {    
        lhs->setRevObject( rhs->getRevObject().clone() );
        
        // make sure all the implicitly created variables got a correct name
        RevBayesCore::DagNode* theNode = lhs->getRevObject().getDagNode();
        theNode->setParentNamePrefix( theNode->getName() );
    }

#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
}


/** Should we execute the rhs dynamically? Yes, because this is a deterministic assingment. */
bool SyntaxDeterministicAssignment::isDynamic( void )
{
    return true;
}

