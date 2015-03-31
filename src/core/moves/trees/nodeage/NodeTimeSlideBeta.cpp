//
//  SubtreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DistributionBeta.h"
#include "NodeTimeSlideBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

NodeTimeSlideBeta::NodeTimeSlideBeta(StochasticNode<TimeTree> *v, double d, double o, bool t, double w) : SimpleMove( v, w, t), variable( v ), delta( d ), offset( o ) {
    
}



/* Clone object */
NodeTimeSlideBeta* NodeTimeSlideBeta::clone( void ) const {
    
    return new NodeTimeSlideBeta( *this );
}



const std::string& NodeTimeSlideBeta::getMoveName( void ) const {
    static std::string name = "NodeTimeSlideBeta";
    
    return name;
}


/** Perform the move */
double NodeTimeSlideBeta::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->isTip() );
    
    TopologyNode& parent = node->getParent();
    
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double child_Age   = node->getChild( 0 ).getAge();
    if ( child_Age < node->getChild( 1 ).getAge()) {
        child_Age = node->getChild( 1 ).getAge();
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // draw new ages and compute the hastings ratio at the same time
    double m = (my_age-child_Age) / (parent_age-child_Age);
    double a = delta * m + offset;
    double b = delta * (1.0-m) + offset;
    double new_m = RbStatistics::Beta::rv(a, b, *rng);
    double my_new_age = (parent_age-child_Age) * new_m + child_Age;
    
    // compute the Hastings ratio
    double forward = RbStatistics::Beta::lnPdf(a, b, new_m);
    double new_a = delta * new_m + offset;
    double new_b = delta * (1.0-new_m) + offset;
    double backward = RbStatistics::Beta::lnPdf(new_a, new_b, m);
    
    // set the age
    tau.setAge( node->getIndex(), my_new_age );
    
    return backward - forward;
}


void NodeTimeSlideBeta::printParameterSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void NodeTimeSlideBeta::rejectSimpleMove( void ) {
    
    // undo the proposal
    variable->getValue().setAge( storedNode->getIndex(), storedAge );
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting NodeTimeSlideBeta proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void NodeTimeSlideBeta::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}


void NodeTimeSlideBeta::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        delta /= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        delta *= (2.0 - rate/0.44 );
    }
}

