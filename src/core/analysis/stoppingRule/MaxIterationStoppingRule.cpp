#include "MaxIterationStoppingRule.h"


using namespace RevBayesCore;


MaxIterationStoppingRule::MaxIterationStoppingRule(size_t g) : StoppingRule(),
    maxGenerations( g )
{
    
}



MaxIterationStoppingRule::~MaxIterationStoppingRule()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
MaxIterationStoppingRule* MaxIterationStoppingRule::clone( void ) const
{
    
    return new MaxIterationStoppingRule( *this );
}



bool MaxIterationStoppingRule::stop()
{
    return true;
}
