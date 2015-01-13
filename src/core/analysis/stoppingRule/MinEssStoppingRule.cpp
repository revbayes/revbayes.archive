#include "MinEssStoppingRule.h"


using namespace RevBayesCore;


MinEssStoppingRule::MinEssStoppingRule(double m, size_t f) : StoppingRule(),
    minEss( m ),
    checkFrequency( f )
{
    
}



MinEssStoppingRule::~MinEssStoppingRule()
{
    
}


/**
 * Should we check at the given iteration for convergence?
 * Only for given iteration because this is a time consuming test.
 */
bool MinEssStoppingRule::checkAtIteration(size_t g) const
{
    // test if the iteration matches the pre-specified frequency
    return (g % checkFrequency) == 0;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
MinEssStoppingRule* MinEssStoppingRule::clone( void ) const
{
    
    return new MinEssStoppingRule( *this );
}


/**
 * Is this a stopping rule? Yes!
 */
bool MinEssStoppingRule::isConvergenceRule( void ) const
{
    return true;
}


/**
 * The run just started. For this rule we do not need to do anything.
 */
void MinEssStoppingRule::runStarted( void )
{
    // nothing to do
}


/**
 * Should we stop now?
 * Yes, if the minimum ESS is larger than the provided threshold.
 */
bool MinEssStoppingRule::stop( size_t g )
{
    
    return 1000 >= minEss;
}
