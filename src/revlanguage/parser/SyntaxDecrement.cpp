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
 * \param[in]   v   The variable expression
 */
SyntaxDecrement::SyntaxDecrement( SyntaxVariable* v ) :
    SyntaxElement(),
    variable( v )
{
}


/** 
 * Deep copy constructor. It really should not be possible for the
 * parser to return a NULL variable statement, so we do not have
 * to check for a NULL pointer.
 */
SyntaxDecrement::SyntaxDecrement( const SyntaxDecrement& x ) :
    SyntaxElement(x)
{
    variable   = x.variable->clone();
}


/** 
 * Destructor deletes operands 
 */
SyntaxDecrement::~SyntaxDecrement() 
{
    delete variable;
}


/** 
 * Assignment operator performing deep assignment.
 */
SyntaxDecrement& SyntaxDecrement::operator=( const SyntaxDecrement& x ) 
{
    if ( this != &x )
    {
        SyntaxElement::operator=( x );

        delete variable;
        variable = x.variable->clone();
    }
    
    return ( *this );
}


/**
 * The clone function is a convenience function to create proper copies of inherited objects.
 * That is, a.clone() will create a clone of type 'b' if the 'a' instance is of derived type 'b'.
 *
 * \return A new copy of myself
 */
SyntaxDecrement* SyntaxDecrement::clone () const
{
    return new SyntaxDecrement( *this );
}


/** 
 * Evaluate the content of this syntax element. This will perform
 * a decrement assignment operation.
 */
RevPtr<Variable> SyntaxDecrement::evaluateContent( Environment& env ) 
{
#ifdef DEBUG_PARSER
    printf( "Evaluating decrement assignment\n" );
#endif
    
    // Get variable. We use standard evaluation because the variable is
    // implicitly on both sides (lhs and rhs) of this type of statement
    RevPtr<Variable> theVariable = variable->evaluateContent( env );
    if ( theVariable == NULL )
        throw RbException( "Invalid NULL variable returned by variable expression in decrement assignment" );
    
    // Make sure that the variable is constant
    if ( !theVariable->getRevObject().isConstant() )
        throw RbException( "Invalid decrement assignment to dynamic variable" );
    
    // Get a non-const reference to the lhs value object
    RevObject& lhs_value = theVariable->getRevObject();

    // Decrement the lhs value. This will not change the control variable status.
    lhs_value.decrement();
    
#ifdef DEBUG_PARSER
    env.printValue(std::cerr);
#endif

    // Return the variable for further assignment
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
    o << "expression    = '--'" << std::endl;
}

