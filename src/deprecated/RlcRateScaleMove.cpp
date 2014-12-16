#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "RlcRateScaleMove.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;


/**
 * Default constructor.
 * The default constructor does nothing except allocating the object.
 *
 * \param[in]   v    The variable on which this move operates.
 * \param[in]   w    The scaling factor.
 * \param[in]   t    If the parameter should be tuned.
 * \param[in]   w    The weight how often the move will be used.
 */
RlcRateScaleMove::RlcRateScaleMove(StochasticNode< RbVector<double> > *v, double l, bool t, double w) : SimpleMove( v, w, t ),
    variable(v),
    index( 0 ),
    lambda( l ),
    storedValue( 1.0 )
{
    
}


/**
 * Accept the move and reset flags/states.
 * We only need to reset the touched element indices of the variable.
 */
void RlcRateScaleMove::acceptSimpleMove( void )   
{
    
    variable->clearTouchedElementIndices();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
RlcRateScaleMove* RlcRateScaleMove::clone( void ) const 
{
    
    return new RlcRateScaleMove( *this );
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& RlcRateScaleMove::getMoveName( void ) const 
{
    static std::string name = "RlcRateScaleMove";
    
    return name;
}



/** 
 * Perform the move.
 *
 * This move randomly selects one element of the vector and then either adds or removes the jump,
 * depending if there was or wasn't a jump before. A jump is represented by a value != 1.0.
 * If a jump is added, the new rate is drawn from the prior.
 *
 * \return The hastings ratio.
 */
double RlcRateScaleMove::performSimpleMove( void ) 
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    std::vector<double>& v = variable->getValue();
    // choose an index (do not propose a jump for the root)
    index = size_t(rng->uniform01() * v.size());
    
    // copy value
    storedValue = v[index];
    
    // flag the index that we are changing
    variable->addTouchedElementIndex(index);
    
    double lnHastingsratio = 0.0;
    
    // now we need to check if we add a jump a remove one
    if ( storedValue != 1.0 ) 
    {
        // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
        double u = rng->uniform01();
        double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
        v[index] *= scalingFactor;
    }
    else
    {
        lnHastingsratio = RbConstants::Double::neginf;
    }
    
    
    return lnHastingsratio;
}


/**
 * Reject the move and reset flags/states.
 * 
 * Since the move stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 * We additionally need to reset the touched element indices of the variable.
 */
void RlcRateScaleMove::rejectSimpleMove( void ) 
{
    
    std::vector<double>& v = variable->getValue();
    v[index] = storedValue;
    variable->clearTouchedElementIndices();
    
}


/**
 * Tune the move to achieve the target acceptance probability of 0.44.
 */
void RlcRateScaleMove::tune( void ) 
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) 
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else 
    {
        lambda /= (2.0 - rate/0.44 );
    }
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void RlcRateScaleMove::swapNode(DagNode *oldN, DagNode *newN) 
{
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
        
}
