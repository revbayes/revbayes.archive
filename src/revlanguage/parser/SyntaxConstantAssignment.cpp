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
SyntaxConstantAssignment::SyntaxConstantAssignment( SyntaxElement* lhsExpr, SyntaxElement* rhsExpr ) : SyntaxAssignment(lhsExpr, rhsExpr)
{
    
}


/** Destructor deletes operands */
SyntaxConstantAssignment::~SyntaxConstantAssignment( void )
{
    
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
void SyntaxConstantAssignment::assign(RevPtr<Variable> &lhs, RevPtr<Variable> &rhs)
{
#ifdef DEBUG_PARSER
    printf( "Evaluating constant assignment\n" );
#endif
    
    // check first if the right-hand-side is a model variable
    if ( !rhs->getRevObject().isModelObject() )
    {
        throw RbException("You used a non-model object to create a constant node. Only model objects can be used for constant nodes.");
    }
    
    // Get a reference to the Rev object value
    const RevObject& value = rhs->getRevObject();
    
    // TODO: This needs to be cleaned up because it is not used properly anymore! (Sebastian)
    // Perform type conversion if needed, otherwise just clone the value object
    RevObject* newValue;
    if ( !value.getTypeSpec().isDerivedOf( lhs->getRevObjectTypeSpec() ) )
    {
        // We are not of a derived type (or the same type) so we need to cast
        if (value.isConvertibleTo( lhs->getRevObjectTypeSpec(), true ) )
        {
            newValue = value.convertTo( lhs->getRevObjectTypeSpec() );
        }
        else
        {
            std::ostringstream msg;
            msg << "Cannot assign variable '" << lhs->getName() << "' with value of type '" << value.getTypeSpec().getType() << "'" << std::endl;
            msg << " because the variable requires type '" << lhs->getRevObjectTypeSpec().getType() << "'" << std::endl;
            throw RbException( msg );
        }
    }
    else
    {
        newValue = value.clone();
        newValue->makeConstantValue();  // We cannot trust evaluateContent to return a constant variable
    }
    
    // Fill the slot with newValue
    lhs->setRevObject( newValue );
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
}


