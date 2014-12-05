/* 
 * File:   RealNodeContainerSlidingMove.cpp
 * Author: nl
 * 
 * Created on 26 juillet 2014, 13:27
 */

#include "RealNodeContainerSlidingMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


RealNodeContainerSlidingMove::RealNodeContainerSlidingMove(StochasticNode<RealNodeContainer> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ) {
    
}


/** Clone object */
RealNodeContainerSlidingMove* RealNodeContainerSlidingMove::clone( void ) const {
    
    return new RealNodeContainerSlidingMove( *this );
}



const std::string& RealNodeContainerSlidingMove::getMoveName( void ) const {
    static std::string name = "RealNodeContainerSlidingMove";
    
    return name;
}


double RealNodeContainerSlidingMove::performSimpleMove( void ) {
        
    // storedValue = variable->getValue();
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    std::vector<double>& v = variable->getValue();
    nodeindex = size_t( rng->uniform01() * v.size() );
    
    storedValue = v[nodeindex];
    
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );

    if (! variable->getValue().isClamped(nodeindex))   {
        v[nodeindex] += slidingFactor;
    }
        
    variable->addTouchedElementIndex(nodeindex);
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void RealNodeContainerSlidingMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void RealNodeContainerSlidingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void RealNodeContainerSlidingMove::rejectSimpleMove( void ) {
    
    std::vector<double>& v = variable->getValue();
    v[nodeindex] = storedValue;
    variable->addTouchedElementIndex(nodeindex);
}

void RealNodeContainerSlidingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<RealNodeContainer>* >( newN );
    
}


void RealNodeContainerSlidingMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
