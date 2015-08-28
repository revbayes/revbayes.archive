//
//  AdmixtureEdgeReversePolarity.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeReversePolarity.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeReversePolarity::AdmixtureEdgeReversePolarity(StochasticNode<AdmixtureTree> *v, int ag, double l, double w) : SimpleMove( v, w), variable( v ), lambda(l), failed(false), activeGen(ag) {
    
}



/* Clone object */
AdmixtureEdgeReversePolarity* AdmixtureEdgeReversePolarity::clone( void ) const {
    
    return new AdmixtureEdgeReversePolarity( *this );
}



const std::string& AdmixtureEdgeReversePolarity::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeReversePolarity";
    
    return name;
}


/** Perform the move */
double AdmixtureEdgeReversePolarity::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Edge Reverse\n";
    
    failed = false;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    
    // if no admixtureParent exists, the proposal fails
    if (admixtureParents.size() == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        failed = false;
        
        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedWeight = storedAdmixtureChild->getWeight();
        
        // forwards proposal
        double admixtureMaxScaler = 0.5;
        double unitWeight = 1.0 - storedWeight / admixtureMaxScaler;
        double a = lambda * unitWeight + 1.0;
        double b = lambda * (1.0 - unitWeight) + 1.0;
        
        // double fwdAlpha = beta * unitWeight / (1.0 - unitWeight);
        double newUnitWeight = RbStatistics::Beta::rv(a, b, *rng);
        double fwdLnProb = RbStatistics::Beta::lnPdf(a, b, newUnitWeight);
        double newWeight = newUnitWeight * admixtureMaxScaler;
        
        // backwards proposal
        // double bwdAlpha = beta * newUnitWeight / (1.0 - newUnitWeight);
        double new_a = lambda * newUnitWeight + 1.0;
        double new_b = lambda * (1.0 - newUnitWeight) + 1.0;
        double bwdLnProb = RbStatistics::Beta::lnPdf(new_a, new_b, unitWeight);
        
        // prior
//        double prior_a = 1.0;
//        double prior_b = 1.0;
//        bwdLnProb += RbStatistics::Beta::lnPdf(prior_a, prior_b, newUnitWeight); // bwd move and fwd prior
//        fwdLnProb += RbStatistics::Beta::lnPdf(prior_a, prior_b, unitWeight); // fwd move and bwd prior
        
        // set new edge
        storedAdmixtureParent->setAdmixtureChild(NULL);
        storedAdmixtureParent->setAdmixtureParent(storedAdmixtureChild);
        storedAdmixtureParent->setWeight(newWeight);
        
        storedAdmixtureChild->setAdmixtureParent(NULL);
        storedAdmixtureChild->setAdmixtureChild(storedAdmixtureParent);
        storedAdmixtureChild->setWeight(1.0);

        // uniform on 0,1 is symmetric
        return bwdLnProb - fwdLnProb;
    }
    
}

void AdmixtureEdgeReversePolarity::acceptSimpleMove( void ) {
    //std::cout << "accept reverse polarity\n";
}

void AdmixtureEdgeReversePolarity::rejectSimpleMove( void ) {
    
    // undo the proposal
    if (!failed)
    {
        //std::cout << "reject reverse polarity\n";
        storedAdmixtureChild->setAdmixtureChild(NULL);
        storedAdmixtureChild->setAdmixtureParent(storedAdmixtureParent);
        storedAdmixtureChild->setWeight(storedWeight);
        
        storedAdmixtureParent->setAdmixtureParent(NULL);
        storedAdmixtureParent->setAdmixtureChild(storedAdmixtureChild);
        storedAdmixtureParent->setWeight(1.0);
    }
}

bool AdmixtureEdgeReversePolarity::isActive(unsigned long g) const {
    
    return g > activeGen;
}


void AdmixtureEdgeReversePolarity::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}
