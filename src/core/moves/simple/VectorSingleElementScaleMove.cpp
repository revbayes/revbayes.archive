#include "VectorSingleElementScaleMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;


VectorSingleElementScaleMove::VectorSingleElementScaleMove(StochasticNode<RbVector<double> > *v, double l, bool t, double w) : SimpleMove( v, w, t ),
    variable(v),
    lambda( l )
{
    
}


/** Clone object */
VectorSingleElementScaleMove* VectorSingleElementScaleMove::clone( void ) const
{
    
    return new VectorSingleElementScaleMove( *this );
}



const std::string& VectorSingleElementScaleMove::getMoveName( void ) const
{
    static std::string name = "VectorSingleElementMove";
    
    return name;
}


double VectorSingleElementScaleMove::performSimpleMove( void )
{
        
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    std::vector<double>& v = variable->getValue();
    // choose an index
    index = size_t(rng->uniform01() * v.size());
    
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

void VectorSingleElementScaleMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void VectorSingleElementScaleMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void VectorSingleElementScaleMove::rejectSimpleMove( void ) {
    
    std::vector<double>& v = variable->getValue();
    v[index] = storedValue;
    variable->clearTouchedElementIndices();
    
}

void VectorSingleElementScaleMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode< RbVector<double> >* >( newN );
    
}


void VectorSingleElementScaleMove::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
