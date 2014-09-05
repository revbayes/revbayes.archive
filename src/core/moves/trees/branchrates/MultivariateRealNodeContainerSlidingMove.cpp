/* 
 * File:   MultivariateRealNodeContainerSlidingMove.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 23:19
 */

#include "MultivariateRealNodeContainerSlidingMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


MultivariateRealNodeContainerSlidingMove::MultivariateRealNodeContainerSlidingMove(StochasticNode<MultivariateRealNodeContainer> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ) {
    
}


/** Clone object */
MultivariateRealNodeContainerSlidingMove* MultivariateRealNodeContainerSlidingMove::clone( void ) const {
    
    return new MultivariateRealNodeContainerSlidingMove( *this );
}



const std::string& MultivariateRealNodeContainerSlidingMove::getMoveName( void ) const {
    static std::string name = "MultivariateRealNodeContainerSlidingMove";
    
    return name;
}


double MultivariateRealNodeContainerSlidingMove::performSimpleMove( void ) {
        
    // storedValue = variable->getValue();
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    MatrixReal& v = variable->getValue();
    compindex = size_t( rng->uniform01() * v.getNumberOfColumns() );
    nodeindex = size_t( rng->uniform01() * v.getNumberOfRows() );
    
    storedValue = v[nodeindex][compindex];
    
    // bk = variable->getValue();
    
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );

    if (! variable->getValue().isClamped(nodeindex,compindex))   {
        v[nodeindex][compindex] += slidingFactor;
    }
        
    variable->addTouchedElementIndex(nodeindex);
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void MultivariateRealNodeContainerSlidingMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void MultivariateRealNodeContainerSlidingMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void MultivariateRealNodeContainerSlidingMove::rejectSimpleMove( void ) {
    
    MatrixReal& v = variable->getValue();
    v[nodeindex][compindex] = storedValue;
    variable->addTouchedElementIndex(nodeindex);
//    variable->setValue(bk);
}

void MultivariateRealNodeContainerSlidingMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MultivariateRealNodeContainer>* >( newN );
    
}


void MultivariateRealNodeContainerSlidingMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
