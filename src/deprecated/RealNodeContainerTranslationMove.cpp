/* 
 * File:   RealNodeContainerTranslationMove.cpp
 * Author: nl
 * 
 * Created on 2 août 2014, 21:32
 */

#include "RealNodeContainerTranslationMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


RealNodeContainerTranslationMove::RealNodeContainerTranslationMove(StochasticNode<RealNodeContainer> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ) {
    
}


/** Clone object */
RealNodeContainerTranslationMove* RealNodeContainerTranslationMove::clone( void ) const {
    
    return new RealNodeContainerTranslationMove( *this );
}



const std::string& RealNodeContainerTranslationMove::getMoveName( void ) const {
    static std::string name = "RealNodeContainerTranslationMove";
    
    return name;
}


double RealNodeContainerTranslationMove::performSimpleMove( void ) {
        
    storedValue = variable->getValue();
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    std::vector<double>& v = variable->getValue();
        
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );

    size_t n = v.size();
    for (size_t i=0; i<n; i++)  {
        if (!variable->getValue().isClamped(i)) {
            v[i] += slidingFactor;
        }
        variable->addTouchedElementIndex(i);
    }
    
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void RealNodeContainerTranslationMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void RealNodeContainerTranslationMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void RealNodeContainerTranslationMove::rejectSimpleMove( void ) {

    variable->setValue(storedValue);
    std::vector<double>& v = variable->getValue();
    size_t n = v.size();
    for (size_t i=0; i<n; i++)  {
        variable->addTouchedElementIndex(i);
    }
}

void RealNodeContainerTranslationMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<RealNodeContainer>* >( newN );
    
}


void RealNodeContainerTranslationMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
