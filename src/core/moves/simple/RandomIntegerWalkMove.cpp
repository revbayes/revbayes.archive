#include "RandomIntegerWalkMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 */
RandomIntegerWalkMove::RandomIntegerWalkMove( StochasticNode<int> *n, double w ) : SimpleMove( n, w ), 
variable( n ), 
storedValue( 0 )
{
    // we need to allocate memory for the stored value
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
RandomIntegerWalkMove* RandomIntegerWalkMove::clone( void ) const 
{
    
    return new RandomIntegerWalkMove( *this );
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& RandomIntegerWalkMove::getMoveName( void ) const 
{
    static std::string name = "Random-Integer-Walk";
    
    return name;
}


/** 
 * Perform the move.
 *
 * This is a very simple move on integer number that proposes with probability p = 0.5
 * to increase the current value by 1 and with probability p = 0.5 to decrease the 
 * current value by 1. Thus, it is a random walk but guided by the acceptance ratio.
 *
 * \return The hastings ratio.
 */
double RandomIntegerWalkMove::performSimpleMove( void ) 
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    int &val = variable->getValue();
    
    // copy value
    storedValue = val;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    if ( u >= 0.5 ) 
    {
        val++;
    }
    else
    {
        val--;
    }
    
    return 0.0;
}


/**
 * Reject the move.
 *
 * Since the move stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void RandomIntegerWalkMove::rejectSimpleMove( void ) 
{
    // swap current value and stored value
    variable->getValue() = storedValue;
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void RandomIntegerWalkMove::swapNode(DagNode *oldN, DagNode *newN) 
{
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<int>* >(newN) ;
    
}


