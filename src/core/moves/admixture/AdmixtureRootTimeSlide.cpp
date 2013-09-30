//
//  AdmixtureRootTimeSlide.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureRootTimeSlide.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureRootTimeSlide::AdmixtureRootTimeSlide(StochasticNode<AdmixtureTree> *v, double d, bool t, double w) : SimpleMove( v, w, t), variable( v ), delta( d ) {
    
}



/* Clone object */
AdmixtureRootTimeSlide* AdmixtureRootTimeSlide::clone( void ) const {
    
    return new AdmixtureRootTimeSlide( *this );
}

void AdmixtureRootTimeSlide::rescaleSubtree(AdmixtureNode *n, double factor) {

    // we only rescale internal nodes
    if ( !n->isTip() ) {
        // rescale the age of the node
        double newAge = n->getAge() * factor;
        n->setAge(newAge);
        
        // rescale both children
        rescaleSubtree( &n->getChild(0), factor);
        rescaleSubtree( &n->getChild(1), factor);
    }
    
    // just realized what a mess this is... requires rescaling the admixture events, which will upset their time consistency
}

const std::string& AdmixtureRootTimeSlide::getMoveName( void ) const {
    static std::string name = "AdmixtureRootTimeSlide";
    
    return name;
}


/** Perform the move */
double AdmixtureRootTimeSlide::performSimpleMove( void ) {
    
    // Get random number generator
    
    /*
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode& node = tau.getRoot();
    
    
    // get the sum of branch lengths attached to the root node
    double t_0 = node.getAge() - node.getChild(0).getAge();
    double t_1 = node.getAge() - node.getChild(1).getAge();
    double t_m = t_0 + t_1;
    
    // draw new position along root's children's branches
    double u = rng->uniform01() * t_m;
    double m = t_0 / t_m;
    
    // forwards proposal
    double a = delta * m + 1.0;
    double b = delta * (1.0 - m) + 1.0;
    
    // double fwdAlpha = beta * unitWeight / (1.0 - unitWeight);
    double new_m = RbStatistics::Beta::rv(a, b, *rng);
    double fwdLnProb = RbStatistics::Beta::lnPdf(a, b, new_m);
    double new_pos = new_m * t_m;
    
    // backwards proposal
    double new_a = delta * new_m + 1.0;
    double new_b = delta * (1.0 - new_m) + 1.0;
    double bwdLnProb = RbStatistics::Beta::lnPdf(new_a, new_b, m);
    
    // rescale the subtrees
    storedRescale_0 = m / new_m;
    storedRescale_1 = 1.0 - storedRescale_0;
  
    // NOT IMPLEMENTED -- rescaling subtrees requires special consideration for the admixture events s.t. they retain time-consistency
//    rescaleSubtree(&node.getChild(0), storedRescale_0 );
    */
    
    return 0.0;
}


void AdmixtureRootTimeSlide::printParameterSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void AdmixtureRootTimeSlide::rejectSimpleMove( void ) {
    
    //AdmixtureTree& tau = variable->getValue();
    
    //AdmixtureNode& node = tau.getRoot();
    
    // undo the proposal
    // ... rescale subtrees
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - node.getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void AdmixtureRootTimeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}


void AdmixtureRootTimeSlide::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}
