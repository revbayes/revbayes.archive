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
SyntaxAdditionAssignment::SyntaxAdditionAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) : SyntaxAssignment(lhsExpr,rhsExpr)
{
    
}


/** 
 * Destructor deletes operands 
 */
SyntaxAdditionAssignment::~SyntaxAdditionAssignment() 
{

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
void SyntaxAdditionAssignment::assign(RevPtr<Variable> &lhs, RevPtr<Variable> &rhs)
{
#ifdef DEBUG_PARSER
    printf( "Evaluating addition assignment\n" );
#endif
    
    // Get variable from lhs. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    if ( lhs == NULL )
    {
        throw RbException( "Invalid NULL variable returned by lhs expression in addition assignment" );
    }
    
    // Make sure that the variable is constant
    if ( !lhs->getRevObject().isConstant() )
    {
        throw RbException( "Invalid addition assignment to dynamic variable" );
    }
    
    // Record whether it is a workspace (control) variable
    bool isWorkspaceVar = lhs->isWorkspaceVariable();
    
    // Get a reference to the lhs value object
    const RevObject& lhs_value = lhs->getRevObject();
    
    // Evaluate the rhs expression
    if ( rhs == NULL )
    {
        throw RbException( "Invalid NULL variable returned by rhs expression in addition assignment" );
    }
    
    // Get a reference to the rhs value object
    const RevObject& rhs_value = rhs->getRevObject();
    
    // Generate result of the multiplication
    RevObject *newValue = lhs_value.add( rhs_value );
    
    // Fill the slot with the new variable
    lhs->setRevObject( newValue );

    // Reset it as workspace (control) variable, if it was a workspace (control) variable before the assignment.
    // When we fill the slot, the workspace (control) variable property is reset to false by default.
    if ( isWorkspaceVar )
    {
        lhs->setWorkspaceVariableState( true );
    }
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
}

