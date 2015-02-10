//
//  SubtreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "SubtreeScale.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

SubtreeScale::SubtreeScale(StochasticNode<TimeTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
SubtreeScale* SubtreeScale::clone( void ) const {
    
    return new SubtreeScale( *this );
}



const std::string& SubtreeScale::getMoveName( void ) const {
    static std::string name = "Subtree-Scale";
    
    return name;
}


/** Perform the move */
double SubtreeScale::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neither the direct descendant of the root
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
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // lower bound
    double min_age = 0.0;
    TreeUtilities::getOldestTip(&tau, node, min_age);
        
    // draw new ages and compute the hastings ratio at the same time
    double my_new_age = min_age + (parent_age - min_age) * rng->uniform01();
    
    double scalingFactor = my_new_age / my_age;
    
    size_t nNodes = node->getNumberOfNodesInSubtree(false);
    
    // rescale the subtrees
    TreeUtilities::rescaleSubtree(&tau, node, scalingFactor );
    
    if (min_age != 0.0)
    {
        for (size_t i = 0; i < tau.getNumberOfTips(); i++)
        {
            if (tau.getNode(i).getAge() < 0.0) {
                return RbConstants::Double::neginf;
            }
        }
    }
    
    // compute the Hastings ratio
    double lnHastingsratio = (nNodes > 1 ? log( scalingFactor ) * (nNodes-1) : 0.0 );
    
    return lnHastingsratio;
}


void SubtreeScale::rejectSimpleMove( void ) {
    
    // undo the proposal
    TreeUtilities::rescaleSubtree(&variable->getValue(), storedNode, storedAge / storedNode->getAge() );
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void SubtreeScale::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

