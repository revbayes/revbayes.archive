//
//  SubtreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "NodeTimeSlideUniform.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

NodeTimeSlideUniform::NodeTimeSlideUniform(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
NodeTimeSlideUniform* NodeTimeSlideUniform::clone( void ) const {
    
    return new NodeTimeSlideUniform( *this );
}



const std::string& NodeTimeSlideUniform::getMoveName( void ) const {
    static std::string name = "NodeTimeSlideUniform";
    
    return name;
}


/** Perform the move */
double NodeTimeSlideUniform::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
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
    double my_new_age = (parent_age-child_Age) * rng->uniform01() + child_Age;
    
    // set the age
    tau.setAge( node->getIndex(), my_new_age );
    
    return 0.0;
}


void NodeTimeSlideUniform::rejectSimpleMove( void ) {
    
    // undo the proposal
    variable->getValue().setAge( storedNode->getIndex(), storedAge );
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void NodeTimeSlideUniform::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

