//
//  NearestNeighborInterchange.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SubtreePruneRegraft.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

SubtreePruneRegraft::SubtreePruneRegraft(StochasticNode<Topology> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
SubtreePruneRegraft* SubtreePruneRegraft::clone( void ) const {
    
    return new SubtreePruneRegraft( *this );
}



const std::string& SubtreePruneRegraft::getMoveName( void ) const {
    static std::string name = "SPR";
    
    return name;
}


bool SubtreePruneRegraft::isDescendant(const TopologyNode &n, const TopologyNode &p) {
    
    if ( n.isRoot() ) {
        return false;
    }
    
    if ( &n == &p ) {
        return true;
    }
    
    return isDescendant(n.getParent(), p);
}


/** Perform the move */
double SubtreePruneRegraft::performSimpleMove( void ) {
    
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
    
    // now we store all necessary values
    storedChoosenNode   = node;
    TopologyNode &parent = node->getParent();
    TopologyNode &grandparent = parent.getParent();
    storedBrother = &parent.getChild( 0 );
    
    // check if we got the correct child
    if ( node == storedBrother ) {
        storedBrother = &parent.getChild( 1 );
    }
    
    // pick a random new parent node
    TopologyNode* newBrother;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        newBrother = &tau.getNode(index);
    } while ( newBrother->isRoot() || isDescendant(*newBrother,parent));
    
    TopologyNode &newGrandparent = newBrother->getParent();
    
    // now prune
    grandparent.removeChild( &parent );
    parent.removeChild( storedBrother );
    grandparent.addChild( storedBrother );
    storedBrother->setParent( &grandparent );
    
    // re-attach
    newGrandparent.removeChild( newBrother );
    parent.addChild( newBrother );
    newGrandparent.addChild( &parent );
    parent.setParent( &newGrandparent );
    newBrother->setParent( &parent );
    
    return 0.0;
}


void SubtreePruneRegraft::rejectSimpleMove( void ) {
    
    // undo the proposal
    TopologyNode &parent = storedChoosenNode->getParent();
    TopologyNode &grandparent = parent.getParent();
    TopologyNode* oldBrother = &parent.getChild( 0 );
    TopologyNode &newGrandparent = storedBrother->getParent();
    
    // check if we got the correct child
    if ( storedChoosenNode == oldBrother ) {
        oldBrother = &parent.getChild( 1 );
    }
    
    // now prune
    grandparent.removeChild( &parent );
    parent.removeChild( oldBrother );
    grandparent.addChild( oldBrother );
    oldBrother->setParent( &grandparent );
    
    // re-attach
    newGrandparent.removeChild( storedBrother );
    parent.addChild( storedBrother );
    newGrandparent.addChild( &parent );
    parent.setParent( &newGrandparent );
    storedBrother->setParent( &parent );
    
}


void SubtreePruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<Topology>* >(newN) ;
}

