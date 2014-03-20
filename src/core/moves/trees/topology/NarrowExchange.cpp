//
//  NarrowExchange.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "NarrowExchange.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

NarrowExchange::NarrowExchange(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
NarrowExchange* NarrowExchange::clone( void ) const {
    
    return new NarrowExchange( *this );
}



const std::string& NarrowExchange::getMoveName( void ) const {
    static std::string name = "NarrowExchange";
    
    return name;
}


/** Perform the move */
double NarrowExchange::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor a direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    
    TopologyNode& parent = node->getParent();
    TopologyNode& grandparent = parent.getParent();
    TopologyNode* uncle = &grandparent.getChild( 0 );
    // check if we got the correct child
    if ( uncle == &parent ) {
        uncle = &grandparent.getChild( 1 );
    }
    
    // we need to work with the times
    double parent_age   = parent.getAge();
    double uncles_age   = uncle->getAge();
    
    if( uncles_age < parent_age ) {
        failed = false;
    
        // now we store all necessary values
        storedChoosenNode   = node;
        storedUncle         = uncle;
    
        // now exchange the two nodes
        grandparent.removeChild( uncle );
        parent.removeChild( node );
        grandparent.addChild( node );
        parent.addChild( uncle );
        node->setParent( &grandparent );
        uncle->setParent( &parent );
    
        return 0.0;
    } else {
        failed = true;
        return RbConstants::Double::neginf;
    }
}


void NarrowExchange::rejectSimpleMove( void ) {
    
    // we undo the proposal only if it didn't fail
    if ( !failed ) {
        // undo the proposal
        TopologyNode& parent = storedUncle->getParent();
        TopologyNode& grandparent = storedChoosenNode->getParent();
    
        // now exchange the two nodes
        grandparent.removeChild( storedChoosenNode );
        parent.removeChild( storedUncle );
        grandparent.addChild( storedUncle );
        parent.addChild( storedChoosenNode );
        storedUncle->setParent( &grandparent );
        storedChoosenNode->setParent( &parent );
        
    }  
    
}


void NarrowExchange::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

