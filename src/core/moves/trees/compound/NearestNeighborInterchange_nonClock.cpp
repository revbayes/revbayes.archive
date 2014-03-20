//
//  NearestNeighborInterchange.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "NearestNeighborInterchange_nonClock.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

NearestNeighborInterchange_nonClock::NearestNeighborInterchange_nonClock(StochasticNode<Topology> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
NearestNeighborInterchange_nonClock* NearestNeighborInterchange_nonClock::clone( void ) const {
    
    return new NearestNeighborInterchange_nonClock( *this );
}



const std::string& NearestNeighborInterchange_nonClock::getMoveName( void ) const {
    static std::string name = "NNI";
    
    return name;
}


/** Perform the move */
double NearestNeighborInterchange_nonClock::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Topology& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
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
}


void NearestNeighborInterchange_nonClock::rejectSimpleMove( void ) {
    
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


void NearestNeighborInterchange_nonClock::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<Topology>* >(newN) ;
}

