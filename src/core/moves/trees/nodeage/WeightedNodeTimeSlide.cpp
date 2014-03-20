//
//  SubtreeScale.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 7/12/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "WeightedNodeTimeSlide.h"

#include <cmath>

using namespace RevBayesCore;

WeightedNodeTimeSlide::WeightedNodeTimeSlide(StochasticNode<TimeTree> *v, int b, double w) : SimpleMove( v, w), variable( v ), blocks( b ) {
    
    for (size_t i = 1; i <= blocks; ++i) {
        double x = i / (1.0 + blocks);
        double q = RbStatistics::Beta::quantile( 0.25, 0.25, x);
        interval.push_back( q );
    }
}



/* Clone object */
WeightedNodeTimeSlide* WeightedNodeTimeSlide::clone( void ) const {
    
    return new WeightedNodeTimeSlide( *this );
}



const std::string& WeightedNodeTimeSlide::getMoveName( void ) const {
    static std::string name = "WeightedNodeTimeSlide";
    
    return name;
}


/** Perform the move */
double WeightedNodeTimeSlide::performSimpleMove( void ) {
    
    // Get random number generator    
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = variable->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = std::floor(tau.getNumberOfNodes() * u);
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->isTip() );
    
    TopologyNode& parent = node->getParent();
    
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double child_Age   = node->getChild( 0 ).getAge();
    if ( child_Age < node->getChild( 1 ).getAge()) {
        child_Age = node->getChild( 1 ).getAge();
    }
    
    // we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // approximate the likelihood curve for this node
    std::vector<double> lnl(1,0.0);
    // get the affected dag nodes for the posterior computation
    std::set<DagNode*> affected;
    variable->getAffectedNodes( affected );
    double f = (parent_age - child_Age);
    double marginal = 0.0;
    double prev_x = 0.0;
    double pre_lnl = 0.0;
    for (size_t i = 0; i < blocks; ++i) {
        double newAge = interval[i] * f + child_Age;
        tau.setAge( node->getIndex(), newAge );
        
        double lnLikelihood = variable->getLnProbability();
        for (std::set<DagNode*>::iterator it = affected.begin(); it != affected.end(); ++it) {
            lnLikelihood += (*it)->getLnProbability();
        }
        lnl.push_back( lnLikelihood );
        
        // compute the integral (marginal likelihood)
        marginal += (pre_lnl+lnLikelihood)/2.0 * (interval[i] - prev_x);
        prev_x = interval[i];
        pre_lnl = lnLikelihood;
    }
    // add the final piece of the marginal likelihood
    lnl.push_back( 0.0 );
    marginal += pre_lnl/2.0 * (1.0 - prev_x);
    // normalize the likelihoods
    for (size_t i = 0; i < (blocks+2); ++i) {
        lnl[i] /= marginal;
    }
    
    // randomly draw a new age (using the cdf of the weight function)
    double u = rng->uniform01();
    double proposedAge = 0.0;
    size_t index = 1;
    while ( u > 0 ) {
        double block = (lnl[index]+lnl[index-1])/2.0 * (interval[index] - interval[index-1]);
        if ( u < block ) {
            double slope = (lnl[index-1]-lnl[index]) / (interval[index] - interval[index-1]);
            double tmp = sqrt( lnl[index-1]*lnl[index-1] + 2.0*u*slope);
            proposedAge = interval[index-1] + (tmp-lnl[index-1])/slope;
            if ( proposedAge < interval[index-1] || proposedAge > interval[index] ) {
                proposedAge = interval[index-1] + (-tmp-lnl[index-1])/slope;
            }
        }
        u -= block;
        index++;
    }
    
    // set the age
    tau.setAge( node->getIndex(), proposedAge );
    
    // compute Hastings ratio (ratio of the weights)
    double weight_old, weight_new = 1.0;
    prev_x = 0.0;
    pre_lnl = 0.0;
    bool foundForward = false, foundBackward = false;
    double proposed_x = (proposedAge-child_Age)/f;
    double old_x = (my_age-child_Age)/f;
    for (size_t i = 0; i < blocks; ++i) {
        if ( !foundForward && interval[i] > proposed_x) {
            foundForward = true;
            weight_new = pre_lnl + (proposed_x-prev_x)/(interval[i]-prev_x)*(lnl[i+1]-pre_lnl);
        }
        if ( !foundBackward && interval[i] > old_x) {
            foundForward = true;
            weight_new = pre_lnl + (old_x-prev_x)/(interval[i]-prev_x)*(lnl[i+1]-pre_lnl);
        }
        if ( foundForward && foundBackward ) {
            break;
        }
        prev_x = interval[i];
        pre_lnl = lnl[i+1];
    }
    
    // where is weight_old from??
    return log( weight_old / weight_new );
}


void WeightedNodeTimeSlide::rejectSimpleMove( void ) {
    
    // undo the proposal
    variable->getValue().setAge( storedNode->getIndex(), storedAge );
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting SubtreeScale proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void WeightedNodeTimeSlide::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<TimeTree>* >(newN) ;
}

