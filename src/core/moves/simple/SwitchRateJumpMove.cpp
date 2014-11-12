#include "BranchRateJumpProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "SwitchRateJumpMove.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;


/**
 * Default constructor.
 * The default constructor does nothing except allocating the object.
 *
 * \param[in]   v    The variable on which this move operates.
 * \param[in]   w    The weight how often the move will be used.
 */
SwitchRateJumpMove::SwitchRateJumpMove(StochasticNode<RbVector<double> > *v, double w) : SimpleMove( v, w, false ),
    variable(v),
    index( 0 ),
    storedValue( 1.0 ),
    valueDistribution( NULL )
{
    
    // we need to get the distribution object
    valueDistribution = static_cast<BranchRateJumpProcess&>( variable->getDistribution() ).getValueDistribution();

}


/**
 * Accept the move and reset flags/states.
 * We only need to reset the touched element indices of the variable.
 */
void SwitchRateJumpMove::acceptSimpleMove( void )   
{
    
    variable->clearTouchedElementIndices();

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
SwitchRateJumpMove* SwitchRateJumpMove::clone( void ) const 
{
    
    return new SwitchRateJumpMove( *this );
}


/**
 * Get moves' name of object 
 *
 * \return The moves' name.
 */
const std::string& SwitchRateJumpMove::getMoveName( void ) const 
{
    static std::string name = "SwitchRateJumpMove";
    
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
double SwitchRateJumpMove::performSimpleMove( void ) 
{
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    std::vector<double>& v = variable->getValue();
    // choose an index (do not propose a jump for the root)
    index = size_t( rng->uniform01() * v.size() );

    // copy value
    storedValue = v[index];
    
    // flag the index that we are changing
    variable->addTouchedElementIndex(index);
    
    double lnHastingsratio = 0.0;
    
    // now we need to check if we add a jump a remove one
    if ( storedValue == 1.0 ) 
    {
        // add a jump
        
        // draw a new value from the value distribution
        valueDistribution->redrawValue();
        
        // store the proposal ratio
        lnHastingsratio = - valueDistribution->computeLnProbability();
        
        // get the new value
        double newVal = valueDistribution->getValue();
        
        // set the value
        v[index] = newVal;
    }
    else
    {
        // remove the jump
        
        // we need to set the value of the value distribution so that we can compute the log probability density
        valueDistribution->setValue( storedValue );
        
        // store the proposal ratio
        lnHastingsratio = valueDistribution->computeLnProbability();
        
        // set the value
        v[index] = 1.0;
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
void SwitchRateJumpMove::rejectSimpleMove( void ) 
{
    
    std::vector<double>& v = variable->getValue();
    v[index] = storedValue;
    variable->clearTouchedElementIndices();
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new variable.
 */
void SwitchRateJumpMove::swapNode(DagNode *oldN, DagNode *newN) 
{
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
    
    // we need to get the distribution object
    valueDistribution = static_cast<BranchRateJumpProcess&>( variable->getDistribution() ).getValueDistribution();
    
}
