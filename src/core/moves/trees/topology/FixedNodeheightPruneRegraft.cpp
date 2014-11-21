//
//  NearestNeighborInterchange.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "FixedNodeheightPruneRegraft.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

FixedNodeheightPruneRegraft::FixedNodeheightPruneRegraft(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
FixedNodeheightPruneRegraft* FixedNodeheightPruneRegraft::clone( void ) const {
    
    return new FixedNodeheightPruneRegraft( *this );
}


void FixedNodeheightPruneRegraft::findNewBrothers(std::vector<TopologyNode *> &b, TopologyNode &p, TopologyNode *n) {
    // security check that I'm not a tip
    if (!n->isTip() && &p != n) {
        // check the first child
        TopologyNode* child = &n->getChild( 0 );
        if ( child->getAge() < p.getAge() ) {
            b.push_back( child );
        } else {
            findNewBrothers(b, p, child);
        }
        
        // check the second child
        child = &n->getChild( 1 );
        if ( child->getAge() < p.getAge() ) {
            b.push_back( child );
        } else {
            findNewBrothers(b, p, child);
        }
    }
}



const std::string& FixedNodeheightPruneRegraft::getMoveName( void ) const {
    static std::string name = "FNPR";
    
    return name;
}


/** Perform the move */
double FixedNodeheightPruneRegraft::performSimpleMove( void ) {
    
    failed = false;
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    
    TopologyNode* parent        = &node->getParent(); 
    TopologyNode& grandparent   = parent->getParent();
    TopologyNode& brother       = parent->getChild( 0 );
    // check if we got the correct child
    if ( &brother == node ) {
        brother = parent->getChild( 1 );
    }
    
    // collect the possible reattachement points
    std::vector<TopologyNode*> new_brothers;
    findNewBrothers(new_brothers, *parent, &tau.getRoot());
    
    if ( new_brothers.size() < 1) {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    size_t index = size_t(rng->uniform01() * new_brothers.size());
    TopologyNode* newBro = new_brothers[index];
    
    
    // now we store all necessary values
    storedBrother       = &brother;
    storedNewBrother    = newBro;
    
    // prune
    grandparent.removeChild( parent );
    parent->removeChild( &brother );
    grandparent.addChild( &brother );
    brother.setParent( &grandparent );
    
    // regraft
    TopologyNode* newGrandParent = &newBro->getParent();
    newGrandParent->removeChild( newBro );
    newGrandParent->addChild( parent );
    parent->setParent( newGrandParent );
    parent->addChild( newBro );
    newBro->setParent( parent );
    
    return 0.0;
}


void FixedNodeheightPruneRegraft::rejectSimpleMove( void ) {
    
    
    // we undo the proposal only if it didn't fail
    if ( !failed ) {
        // undo the proposal
        TopologyNode& parent = storedNewBrother->getParent();
        TopologyNode& newGrandparent = parent.getParent();
        TopologyNode& grandparent = storedBrother->getParent();
        
        // prune
        newGrandparent.removeChild( &parent );
        parent.removeChild( storedNewBrother );
        newGrandparent.addChild( storedNewBrother );
        storedNewBrother->setParent( &newGrandparent );
        
        
        // regraft
        grandparent.removeChild( storedBrother );
        parent.addChild( storedBrother );
        storedBrother->setParent( &parent );
        grandparent.addChild( &parent );
        parent.setParent( &grandparent );
    }
    
    
}


void FixedNodeheightPruneRegraft::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

