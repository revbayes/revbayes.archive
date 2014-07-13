#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxIncrement.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;

/** 
 * Construct from expression. 
 *
 * \param[in]   e           The expression on the right hand side.
 */
SyntaxIncrement::SyntaxIncrement(SyntaxVariable* v) :
    SyntaxElement(),
    variable( v ) 
{
}


/** 
 * Deep copy constructor.
 */
SyntaxIncrement::SyntaxIncrement(const SyntaxIncrement& x) :
    SyntaxElement(x)
{
    if ( x.variable != NULL )
    {
        variable   = x.variable->clone();
    }
    else
    {
        variable = NULL;
    }
}


/** 
 * Destructor deletes operands 
 */
SyntaxIncrement::~SyntaxIncrement() 
{
    delete variable;
}


/** 
 * Assignment operator performing a deep assignment.
 */
SyntaxIncrement& SyntaxIncrement::operator=(const SyntaxIncrement& x) 
{
    if ( this != &x )
    {
        
        delete variable;
        
        variable = x.variable->clone();
    }
    
    return (*this);
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of myself 
 */
SyntaxIncrement* SyntaxIncrement::clone () const 
{
    return new SyntaxIncrement(*this);
}


/** 
 * Evaluate the content of this syntax element.
 * This will perform an increment assignment operation.
 */
RevPtr<Variable> SyntaxIncrement::evaluateContent( Environment& env ) 
{
#ifdef DEBUG_PARSER
    printf( "Evaluating increment assignment\n" );
#endif
    
    // Get variable from lhs. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<Variable> theVariable = variable->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in subtraction assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid subtraction assignment to dynamic variable" );
    
    // Get a nonconst reference to the lhs value object
    RevObject& lhs_value = theVariable->getRevObject();
    
    // Increment the lhs value
    lhs_value.increment();
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    return theVariable;
}


/** 
 * Print info about the syntax element 
 */
void SyntaxIncrement::printValue(std::ostream& o) const 
{
    o << "SyntaxIncrement:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = '++'" << std::endl;
}

