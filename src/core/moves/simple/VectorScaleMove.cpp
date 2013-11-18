#include "VectorScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Simple constructor only setting the interval members from the given parameters.
 *
 * \param[in]    n    The stochastic node holding the vector of doubles.
 * \param[in]    l    The scaling parameter (lambda).
 * \param[in]    t    Should this move be tuned.
 * \param[in]    w    Weight of the proposal.
 */
VectorScaleMove::VectorScaleMove( StochasticNode< std::vector<double> > *n, double l, bool t, double w ) : SimpleMove( n, w, t ), variable( n ), storedValue( 0.0 ), lambda( l ) {
    // we need to allocate memory for the stored value
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the move. 
 */
VectorScaleMove* VectorScaleMove::clone( void ) const {
    
    return new VectorScaleMove( *this );
}


/**
 * The function returns a string containing the name of the move. This name can be used for output purposes.
 *
 * \return The move's name. 
 */
const std::string& VectorScaleMove::getMoveName( void ) const {
    static std::string name = "VectorScale";
    
    return name;
}


/**
 * Perform the actual move, i.e., propose a new value.
 * First, we uniformly pick a new index, which is then stored for a potential reject call.
 * Additionally, we store the value of the chose element too in case of a reject call.
 * The, we draw randumly u ~ Uniform(0,1) and compute the scaling factor by
 * sf = exp( lambda * ( u - 0.5 ) )
 *
 * \return The Hastings ratio. 
 */
double VectorScaleMove::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    std::vector<double> &val = variable->getValue();
        
    // copy value
    storedValue = val;
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    for (size_t index=0; index<val.size(); ++index) 
    {
        val[index] *= scalingFactor;
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = val.size() * log( scalingFactor );
    
    return lnHastingsratio;
}


void VectorScaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void VectorScaleMove::rejectSimpleMove( void ) {
    
    std::vector<double> &val = variable->getValue();
	
    // swap current value and stored value
    val = storedValue;
	
}


void VectorScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode< std::vector<double> >* >(newN) ;
}


void VectorScaleMove::tune( void ) {
    
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

