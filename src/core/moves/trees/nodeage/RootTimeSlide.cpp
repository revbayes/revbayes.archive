//
//  TreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RootTimeSlide.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

RootTimeSlide::RootTimeSlide(StochasticNode<TimeTree> *v, double d, bool t, double w) : SimpleMove( v, w, t), variable( v ), delta( d ) {
    
}



/* Clone object */
RootTimeSlide* RootTimeSlide::clone( void ) const {
    
    return new RootTimeSlide( *this );
}



const std::string& RootTimeSlide::getMoveName( void ) const {
    static std::string name = "RootTimeSlide";
    
    return name;
}


/** Perform the move */
double RootTimeSlide::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // we need to work with the times
    double my_age      = node.getAge();
    
    // now we store all necessary values
    storedAge = my_age;
    
    // get the minimum age
    double child_Age   = node.getChild( 0 ).getAge();
    if ( child_Age < node.getChild( 1 ).getAge()) {
        child_Age = node.getChild( 1 ).getAge();
    }
    
    
    // draw new ages and compute the hastings ratio at the same time
    // Note: the Hastings ratio needs to be there because one of the nodes might be a tip and hence not scaled!
    double u = rng->uniform01();
    double my_new_age = my_age + ( delta * ( u - 0.5 ) );
    
    // check if the new age is lower than the minimum
    if ( my_new_age < child_Age ) {
        my_new_age = child_Age - (my_new_age - child_Age);
    }
    
    // rescale the subtrees
    tau.setAge( node.getIndex(), my_new_age );
    
    return 0.0;
}


void RootTimeSlide::printParameterSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void RootTimeSlide::rejectSimpleMove( void ) {
    
    TimeTree& tau = variable->getValue();
    
    TopologyNode& node = tau.getRoot();
    
    // undo the proposal
    tau.setAge( node.getIndex(), storedAge );
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - node.getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void RootTimeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}


void RootTimeSlide::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}

