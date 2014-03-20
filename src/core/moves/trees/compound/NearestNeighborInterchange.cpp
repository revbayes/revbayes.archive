//
//  NearestNeighborInterchange.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "NearestNeighborInterchange.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

NearestNeighborInterchange::NearestNeighborInterchange(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
NearestNeighborInterchange* NearestNeighborInterchange::clone( void ) const {
    
    return new NearestNeighborInterchange( *this );
}



const std::string& NearestNeighborInterchange::getMoveName( void ) const {
    static std::string name = "NNI";
    
    return name;
}


/** Perform the move */
double NearestNeighborInterchange::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
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
    
    // we need to work with the times
    double gparent_age  = grandparent.getAge();
    double parent_age   = parent.getAge();
    double uncles_age   = uncle->getAge();
    double my_age       = node->getAge();
    
    // now we store all necessary values
    storedChoosenNode   = node;
    storedUncle         = uncle;
    storedChoosenAge    = my_age;
    storedUnclesAge     = uncles_age;
    
    // now exchange the two nodes
    grandparent.removeChild( uncle );
    parent.removeChild( node );
    grandparent.addChild( node );
    parent.addChild( uncle );
    node->setParent( &grandparent );
    uncle->setParent( &parent );
    
    // draw new ages and compute the hastings ratio at the same time
    // Note: the Hastings ratio needs to be there because one of the nodes might be a tip and hence not scaled!
    double my_new_age;
    double lnHastingsratio;
    if ( node->isTip() ) {
        my_new_age = my_age;
        lnHastingsratio = 0.0;
    } else {
        my_new_age = gparent_age * rng->uniform01();
        lnHastingsratio = log( gparent_age / parent_age);
    }
    double uncles_new_age;
    if ( uncle->isTip() ) {
        uncles_new_age = uncles_age;
    } else {
        uncles_new_age = parent_age * rng->uniform01();
        lnHastingsratio += log( parent_age / gparent_age);
    }
    
    // rescale the subtrees
    TreeUtilities::rescaleSubtree(&tau, node, my_new_age / my_age );
    TreeUtilities::rescaleSubtree(&tau, uncle, uncles_new_age / uncles_age );
    
    return lnHastingsratio;
}


void NearestNeighborInterchange::rejectSimpleMove( void ) {
        
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
    
    // rescale to old ages
    TreeUtilities::rescaleSubtree(&variable->getValue(), storedChoosenNode, storedChoosenAge / storedChoosenNode->getAge() );
    TreeUtilities::rescaleSubtree(&variable->getValue(), storedUncle, storedUnclesAge / storedUncle->getAge() );
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedChoosenAge - storedChoosenNode->getAge()) > 1E-8 || fabs(storedUnclesAge - storedUncle->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting NNI proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void NearestNeighborInterchange::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

