//
//  AdmixtureNodeTimeSlideUniform.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureNodeTimeSlideUniform.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureNodeTimeSlideUniform::AdmixtureNodeTimeSlideUniform(StochasticNode<AdmixtureTree> *v, double w) : SimpleMove( v, w), variable( v ) {
    
}



/* Clone object */
AdmixtureNodeTimeSlideUniform* AdmixtureNodeTimeSlideUniform::clone( void ) const {
    
    return new AdmixtureNodeTimeSlideUniform( *this );
}



const std::string& AdmixtureNodeTimeSlideUniform::getMoveName( void ) const {
    static std::string name = "AdmixtureNodeTimeSlideUniform";
    
    return name;
}


/** Perform the move */
double AdmixtureNodeTimeSlideUniform::performSimpleMove( void ) {
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    AdmixtureNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->isTip() );
    
    AdmixtureNode& parent = node->getParent();
    
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double child_Age   = node->getChild( 0 ).getAge();
    if ( node->getNumberOfChildren() > 1 && child_Age < node->getChild( 1 ).getAge()) {
        child_Age = node->getChild( 1 ).getAge();
    }
    
    // admixture node ages are further constrained by their partner's time
    if (&node->getAdmixtureChild() != NULL)
    {
        AdmixtureNode& admixtureChild = node->getAdmixtureChild();
        double other_parent_age = admixtureChild.getParent().getAge();
        if (parent_age > other_parent_age)
            parent_age = other_parent_age;
        double other_child_age = admixtureChild.getChild(0).getAge();
        if (child_Age < other_child_age)
            child_Age = other_child_age;
    }
    else if (&node->getAdmixtureParent() != NULL)
    {
        AdmixtureNode& admixtureParent = node->getAdmixtureParent();
        double other_parent_age = admixtureParent.getParent().getAge();
        if (parent_age > other_parent_age)
            parent_age = other_parent_age;
        double other_child_age = admixtureParent.getChild(0).getAge();
        if (child_Age < other_child_age)
            child_Age = other_child_age;
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // draw new ages and compute the hastings ratio at the same time
    double my_new_age = (parent_age-child_Age) * rng->uniform01() + child_Age;
    
    // set the age
    node->setAge( my_new_age );
    
    // set age of admixture pair
    if (&node->getAdmixtureParent() != NULL)
        node->getAdmixtureParent().setAge( my_new_age );
    else if (&node->getAdmixtureChild() != NULL)
        node->getAdmixtureChild().setAge( my_new_age );
    
    return 0.0;
}


void AdmixtureNodeTimeSlideUniform::rejectSimpleMove( void ) {
    
    // undo the proposal
    storedNode->setAge( storedAge );
    
    if (&storedNode->getAdmixtureParent() != NULL)
        storedNode->getAdmixtureParent().setAge( storedAge );
    else if (&storedNode->getAdmixtureChild() != NULL)
        storedNode->getAdmixtureChild().setAge( storedAge );

    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void AdmixtureNodeTimeSlideUniform::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}
