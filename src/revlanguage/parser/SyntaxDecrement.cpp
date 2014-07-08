#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "SyntaxDecrement.h"
#include "Workspace.h"

#include <iostream>
#include <list>
#include <sstream>

using namespace RevLanguage;


/** 
 * Construct from variable expression.
 *
 * \param[in]   v   The variable expression on the left-hand side.
 */
SyntaxDecrement::SyntaxDecrement(SyntaxVariable* v) :
    SyntaxElement(),
    variable( v )
{
}


/** 
 * Deep copy constructor.
 */
SyntaxDecrement::SyntaxDecrement(const SyntaxDecrement& x) : SyntaxElement(x) 
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
SyntaxDecrement::~SyntaxDecrement() 
{
    delete variable;
}


/** 
 * Assignment operator performing a deep assignment.
 */
SyntaxDecrement& SyntaxDecrement::operator=(const SyntaxDecrement& x) 
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
SyntaxDecrement* SyntaxDecrement::clone () const 
{
    return new SyntaxDecrement(*this);
}


/** 
 * Evaluate the content of this syntax element.
 * This will perform a decrement assignment operation.
 */
RevPtr<Variable> SyntaxDecrement::evaluateContent( Environment& env ) 
{
#ifdef DEBUG_PARSER
    printf( "Evaluating decrement assignment\n" );
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
    
    // Decrement the lhs value
    lhs_value.decrement();
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif
    
    return theVariable;
}



/** 
 * Print info about the syntax element 
 */
void SyntaxDecrement::printValue(std::ostream& o) const 
{
    o << "SyntaxDecrement:" << std::endl;
    o << "variable      = ";
    variable->printValue(o);
    o << std::endl;
    o << "expression    = '++'" << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the element on our right-hand-side and also to the variable itself (lhs).
 */
void SyntaxDecrement::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    variable->replaceVariableWithConstant(name, c);
}


