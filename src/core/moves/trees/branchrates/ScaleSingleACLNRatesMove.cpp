#include "ScaleSingleACLNRatesMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;


ScaleSingleACLNRatesMove::ScaleSingleACLNRatesMove(StochasticNode<RevBayesCore::RbVector<double> >* v, double l, bool t, double w) : SimpleMove( v, w, t ),
    variable(v),
    lambda( l )
{

}


/** Clone object */
ScaleSingleACLNRatesMove* ScaleSingleACLNRatesMove::clone( void ) const {
    
    return new ScaleSingleACLNRatesMove( *this );
}



const std::string& ScaleSingleACLNRatesMove::getMoveName( void ) const
{
    static std::string name = "ScaleSingleBranchRateACLNMove";
    
    return name;
}


double ScaleSingleACLNRatesMove::performSimpleMove( void ) {
	
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
	
    RevBayesCore::RbVector<double>& v = variable->getValue();

	index = int(rng->uniform01() * v.size());
	
    // copy value
    storedValue = v[index];
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    double u = rng->uniform01();
    double scalingFactor = std::exp( lambda * ( u - 0.5 ) );
    v[index] *= scalingFactor;
    
    variable->addTouchedElementIndex(index);
	
    // compute the Hastings ratio
    double lnHastingsratio = log( scalingFactor );
    
    return lnHastingsratio;
}

void ScaleSingleACLNRatesMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void ScaleSingleACLNRatesMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void ScaleSingleACLNRatesMove::rejectSimpleMove( void ) {
    
    std::vector<double>& v = variable->getValue();
    v[index] = storedValue;
    variable->clearTouchedElementIndices();
    
}

void ScaleSingleACLNRatesMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
    
}


void ScaleSingleACLNRatesMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
