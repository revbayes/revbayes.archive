#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "VariableSlot.h"
#include "SyntaxDecrement.h"
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
SyntaxDecrement::SyntaxDecrement(SyntaxVariable* v) : SyntaxElement(), 
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
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the model. 
 */
SyntaxDecrement* SyntaxDecrement::clone () const 
{
    
    return new SyntaxDecrement(*this);
}


/** 
 * Evaluate the content of this syntax element.
 * This will perform an addition assignment operation.
 */
RevPtr<Variable> SyntaxDecrement::evaluateContent( Environment& env ) 
{
    
#ifdef DEBUG_PARSER
    printf( "Evaluating Decrement expression\n" );
#endif
    
    // Get variable info from lhs
    RevPtr<Variable> theVariable = variable->createVariable( env );
    
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by lhs expression in addition assignment" );
    
    RevObject& lhs_value = theVariable->getRevObject();
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


