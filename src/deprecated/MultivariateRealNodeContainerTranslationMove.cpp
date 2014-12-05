/* 
 * File:   MultivariateRealNodeContainerTranslationMove.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:32
 */

#include "MultivariateRealNodeContainerTranslationMove.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMathVector.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;


MultivariateRealNodeContainerTranslationMove::MultivariateRealNodeContainerTranslationMove(StochasticNode<MultivariateRealNodeContainer> *v, double l, bool t, double w) : SimpleMove( v, w, t ), variable(v), lambda( l ), storedValue(v->getValue()) {
    
}


/** Clone object */
MultivariateRealNodeContainerTranslationMove* MultivariateRealNodeContainerTranslationMove::clone( void ) const {
    
    return new MultivariateRealNodeContainerTranslationMove( *this );
}



const std::string& MultivariateRealNodeContainerTranslationMove::getMoveName( void ) const {
    static std::string name = "MultivariateRealNodeContainerTranslationMove";
    
    return name;
}


double MultivariateRealNodeContainerTranslationMove::performSimpleMove( void ) {
        
    storedValue = variable->getValue();
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    MatrixReal& v = variable->getValue();
    // const TypedDagNode< TimeTree >* tau = variable->getValue().getTree();
    const TimeTree* tau = variable->getValue().getTimeTree();
    
    // choose an index
    size_t component = size_t( rng->uniform01() * v.getNumberOfColumns() );
    
    double u = rng->uniform01();
    double slidingFactor = lambda * ( u - 0.5 );
    recursiveTranslate(tau->getRoot(),int(component),slidingFactor);
    
    double lnHastingsratio = 0;
    
    return lnHastingsratio;
}

void MultivariateRealNodeContainerTranslationMove::recursiveTranslate(const TopologyNode& from, int component, double slide)   {


    MatrixReal& value = variable->getValue();
    size_t index = from.getIndex();
    if (! variable->getValue().isClamped(index,component))   {
        value[index][component] += slide;
    }
    
    variable->addTouchedElementIndex(index);

    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveTranslate(from.getChild(i), component, slide);
    }
    
}

void MultivariateRealNodeContainerTranslationMove::recursiveTouch(const TopologyNode& from)  {
    
    variable->addTouchedElementIndex(from.getIndex());

    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveTouch(from.getChild(i));
    }
    
}

void MultivariateRealNodeContainerTranslationMove::acceptSimpleMove()   {
    variable->clearTouchedElementIndices();
}

void MultivariateRealNodeContainerTranslationMove::printParameterSummary(std::ostream &o) const {
    o << "lambda = " << lambda;
}


void MultivariateRealNodeContainerTranslationMove::rejectSimpleMove( void ) {
    
    variable->getValue() = storedValue;
    recursiveTouch(variable->getValue().getTimeTree()->getRoot());
}

void MultivariateRealNodeContainerTranslationMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    
    SimpleMove::swapNode(oldN, newN);
    variable = static_cast<StochasticNode<MultivariateRealNodeContainer>* >( newN );
    
}


void MultivariateRealNodeContainerTranslationMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        lambda /= (2.0 - rate/0.44 );
    }
}
