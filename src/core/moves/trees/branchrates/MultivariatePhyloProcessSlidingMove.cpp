/* 
 * File:   MultivariatePhyloProcessSlidingMove.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 23:19
 */

#include "MultivariatePhyloProcessSlidingMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


MultivariatePhyloProcessSlidingMove::MultivariatePhyloProcessSlidingMove(StochasticNode<MultivariatePhyloProcess> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ) {
    
}


/** Clone object */
MultivariatePhyloProcessSlidingMove* MultivariatePhyloProcessSlidingMove::clone( void ) const {
    
    return new MultivariatePhyloProcessSlidingMove( *this );
}



const std::string& MultivariatePhyloProcessSlidingMove::getMoveName( void ) const {
    static std::string name = "MultivariatePhyloProcessSlidingMove";
    
    return name;
}


double MultivariatePhyloProcessSlidingMove::performSimpleMove( void ) {
        
    // storedValue = variable->getValue();
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    MatrixReal& v = variable->getValue();
    compindex = size_t( rng->uniform01() * v.getNumberOfColumns() );
    nodeindex = size_t( rng->uniform01() * v.getNumberOfRows() );
    
    storedValue = v[nodeindex][compindex];
    
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );

    if (! variable->getValue().isClamped(nodeindex,compindex))   {
        v[nodeindex][compindex] += slidingFactor;
    }
        
    variable->addTouchedElementIndex(nodeindex);
    const TimeTree* tau = variable->getValue().getTimeTree();
    const TopologyNode& from = tau->getNode(nodeindex);
    if (! from.isTip()) {
        size_t numChildren = from.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            variable->addTouchedElementIndex(from.getChild(i).getIndex());
        }
    }
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void MultivariatePhyloProcessSlidingMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void MultivariatePhyloProcessSlidingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void MultivariatePhyloProcessSlidingMove::rejectSimpleMove( void ) {
    
    MatrixReal& v = variable->getValue();
    v[nodeindex][compindex] = storedValue;
    variable->addTouchedElementIndex(nodeindex);
    const TimeTree* tau = variable->getValue().getTimeTree();
    const TopologyNode& from = tau->getNode(nodeindex);
    if (! from.isTip()) {
        size_t numChildren = from.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            variable->addTouchedElementIndex(from.getChild(i).getIndex());
        }
    }    
}

void MultivariatePhyloProcessSlidingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MultivariatePhyloProcess>* >( newN );
    
}


void MultivariatePhyloProcessSlidingMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
