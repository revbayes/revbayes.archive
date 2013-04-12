//
//  AdmixtureNearestNeighborInterchange.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureNearestNeighborInterchange.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureNearestNeighborInterchange::AdmixtureNearestNeighborInterchange(StochasticNode<AdmixtureTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
AdmixtureNearestNeighborInterchange* AdmixtureNearestNeighborInterchange::clone( void ) const {
    
    return new AdmixtureNearestNeighborInterchange( *this );
}



const std::string& AdmixtureNearestNeighborInterchange::getMoveName( void ) const {
    static std::string name = "NNI";
    
    return name;
}


/** Perform the move */
double AdmixtureNearestNeighborInterchange::performSimpleMove( void ) {
    
    
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    //std::cout << "\nNNI\n";
    //std::cout << "before NNI proposal\n";
    //tau.checkAllEdgesRecursively(&tau.getRoot());
    
    // pick a random node which is not the root, not the direct descendant of the root
    AdmixtureNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getTopologyParent().isRoot() || node->getNumberOfChildren() == 1);
    
    AdmixtureNode& parent = node->getTopologyParent();
    AdmixtureNode& grandparent = parent.getTopologyParent();
    AdmixtureNode* uncle = &grandparent.getTopologyChild( 0 );
    
    
    // check if we got the correct child
    if ( uncle == &parent ) {
        uncle = &grandparent.getTopologyChild( 1 );
    }

    // we need to work with the times
    double gparent_age  = grandparent.getAge();
    double parent_age   = parent.getAge();
    double uncles_age   = uncle->getAge();
    double my_age       = node->getAge();
    double p2age = node->getParent().getAge();
    double u2age = uncle->getParent().getAge();
    
//    if( uncles_age < parent_age ) {
    //if (u2age <= p2age)
    if (uncles_age < p2age && my_age < u2age)
    {
        
        failed = false;
        
        // now we store all necessary values
        storedChosenNode   = node;
        storedUncle         = uncle;
        storedChosenNodeParent = &node->getParent();
        storedUncleParent = &uncle->getParent();

        storedChosenAge    = my_age;
        storedUnclesAge     = uncles_age;

        // now exchange the two nodes
        storedUncleParent->removeChild( uncle ) ;
        storedUncleParent->addChild( node );
        node->setParent( storedUncleParent );

        storedChosenNodeParent->removeChild( node );
        storedChosenNodeParent->addChild( uncle );
        uncle->setParent( storedChosenNodeParent );
        
        //std::cout << "\nafter NNI proposal\n";
        //tau.checkAllEdgesRecursively(&tau.getRoot());
        
        return 0.0;
    }
    else
    {
        failed = true;
        return RbConstants::Double::neginf;
    }

}


void AdmixtureNearestNeighborInterchange::rejectSimpleMove( void ) {
    
    //std::cout << "NNI reject\n";
    if (!failed)
    {
        storedUncleParent->removeChild(storedChosenNode);
        storedUncleParent->addChild(storedUncle);
        storedUncle->setParent(storedUncleParent);
        
        storedChosenNodeParent->removeChild(storedUncle);
        storedChosenNodeParent->addChild(storedChosenNode);
        storedChosenNode->setParent(storedChosenNodeParent);
    }
    //AdmixtureTree& tau = variable->getValue();
    //std::cout << "\nafter NNI reject\n";
    //tau.checkAllEdgesRecursively(&tau.getRoot());
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedChosenAge - storedChosenNode->getAge()) > 1E-8 || fabs(storedUnclesAge - storedUncle->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting NNI proposal: Node ages were not correctly restored!");
    }
#endif
    
}

void AdmixtureNearestNeighborInterchange::acceptSimpleMove( void ) {
    //std::cout << "NNI accept\n";
}


void AdmixtureNearestNeighborInterchange::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}