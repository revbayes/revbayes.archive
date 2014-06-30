//
//  TreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "OriginTimeSlide.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

OriginTimeSlide::OriginTimeSlide( StochasticNode<double> *ot, StochasticNode<TimeTree> *tre, double d, bool t, double w) : SimpleMove( ot, w, t), variable( ot ), tree( tre ), delta( d ) {
    
}



/* Clone object */
OriginTimeSlide* OriginTimeSlide::clone( void ) const {
    
    return new OriginTimeSlide( *this );
}



const std::string& OriginTimeSlide::getMoveName( void ) const {
    static std::string name = "OriginTimeSlide";
    
    return name;
}


/** Perform the move */
double OriginTimeSlide::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = tree->getValue();
	
	
    
    TopologyNode& root = tau.getRoot();
    
    // we need to work with the times
    double my_age      = variable->getValue();
    
    // now we store all necessary values
    storedAge = my_age;
    
    // get the minimum age
    double child_Age   = root.getAge();
    
    
    // draw new ages and compute the hastings ratio at the same time
    // Note: the Hastings ratio needs to be there because one of the nodes might be a tip and hence not scaled!
    double u = rng->uniform01();
    double my_new_age = my_age + ( delta * ( u - 0.5 ) );
    
    // check if the new age is lower than the minimum
    if ( my_new_age < child_Age ) {
        my_new_age = child_Age - (my_new_age - child_Age);
    }
    
    variable->setValue( new double(my_new_age) );
    
    return 0.0;
}


void OriginTimeSlide::printParameterSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void OriginTimeSlide::rejectSimpleMove( void ) {
    
    
    // undo the proposal
    variable->setValue( new double(storedAge) );
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - node.getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting OriginTimeSlide proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void OriginTimeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<double>* >(newN) ;
}


void OriginTimeSlide::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}

