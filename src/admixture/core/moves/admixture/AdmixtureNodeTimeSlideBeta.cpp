//
//  AdmixtureNodeTimeSlideBeta.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureNodeTimeSlideBeta.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureNodeTimeSlideBeta::AdmixtureNodeTimeSlideBeta(StochasticNode<AdmixtureTree> *v, int ni, double d, bool t, double w) : SimpleMove( v, w, t), nodeIndex(ni), variable( v ), delta(d) {
    
}



/* Clone object */
AdmixtureNodeTimeSlideBeta* AdmixtureNodeTimeSlideBeta::clone( void ) const {
    
    return new AdmixtureNodeTimeSlideBeta( *this );
}



const std::string& AdmixtureNodeTimeSlideBeta::getMoveName( void ) const {
    static std::string name = "AdmixtureNodeTimeSlideBeta";
    
    return name;
}


/** Perform the move */
double AdmixtureNodeTimeSlideBeta::performSimpleMove( void ) {
    
    //std::cout << "\nNode Age Beta Slide\n";
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    // get nodes[nodeIndex]
    AdmixtureNode* node = &tau.getNode(nodeIndex);
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
    
    // sample beta rv and compute proposal factors
    double ageRange = parent_age - child_Age;
    double unitAge = (storedAge - child_Age) / ageRange;
    
    
    //std::cout << child_Age << " "  << storedAge << " " << parent_age << " " << ageRange << " " << unitAge << "\n";

    double a = delta * unitAge + 1.0;
    double b = delta * (1.0 - unitAge) + 1.0;
    double newUnitAge = RbStatistics::Beta::rv(a, b, *rng);
    double fwdProposal = RbStatistics::Beta::lnPdf(a, b, newUnitAge);
    double my_new_age = newUnitAge * ageRange + child_Age;
    double new_a = delta * newUnitAge + 1.0;
    double new_b = delta * (1.0 - newUnitAge) + 1.0;
    double bwdProposal = RbStatistics::Beta::lnPdf(new_a, new_b, unitAge);

    // set the age
    node->setAge( my_new_age );
    
    // set age of admixture pair
    if (&node->getAdmixtureParent() != NULL)
        node->getAdmixtureParent().setAge( my_new_age );
    else if (&node->getAdmixtureChild() != NULL)
        node->getAdmixtureChild().setAge( my_new_age );
    
    //std::cout << "idx\t" << node->getIndex() << "   " << storedAge << " -> " << my_new_age << "\n";
    
    return bwdProposal - fwdProposal;
}


void AdmixtureNodeTimeSlideBeta::rejectSimpleMove( void ) {
    
    //std::cout << "Node Beta Slide Reject\n";
    
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


void AdmixtureNodeTimeSlideBeta::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}

void AdmixtureNodeTimeSlideBeta::acceptSimpleMove(void)
{
    //std::cout << "accept time slide beta\n";
}

void AdmixtureNodeTimeSlideBeta::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}
