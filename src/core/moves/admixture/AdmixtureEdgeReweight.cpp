//
//  AdmixtureEdgeReweight.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeReweight.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeReweight::AdmixtureEdgeReweight(StochasticNode<AdmixtureTree> *v, int ag, double l, double w) : SimpleMove( v, w), variable( v ), lambda( l ), activeGen(ag) {
    
}



/* Clone object */
AdmixtureEdgeReweight* AdmixtureEdgeReweight::clone( void ) const {
    
    return new AdmixtureEdgeReweight( *this );
}



const std::string& AdmixtureEdgeReweight::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeReweight";
    
    return name;
}


/** Perform the move */
double AdmixtureEdgeReweight::performSimpleMove( void ) {
    
    //std::cout << "\nAdmix Edge Reweight\n";
    
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
        storedAdmixtureChild = &admixtureParents[index]->getAdmixtureChild();
        
        // get weight for admixture event
        storedWeight = storedAdmixtureChild->getWeight();
        
        // forwards proposal
        double admixtureMaxScaler = 0.5;
        double unitWeight = storedWeight / admixtureMaxScaler;
        double a = lambda * unitWeight + 1.0;
        double b = lambda * (1.0 - unitWeight) + 1.0;
        
        // double fwdAlpha = beta * unitWeight / (1.0 - unitWeight);
        double newUnitWeight = RbStatistics::Beta::rv(a, b, *rng);
        double fwdLnProb = RbStatistics::Beta::lnPdf(a, b, newUnitWeight);
        double newWeight = newUnitWeight * admixtureMaxScaler;
        /*
        if (newUnitWeight < 10e-10 || (1.0 - newUnitWeight < 10e-10))
        {
            failed = true;
            return RbConstants::Double::neginf;
        }
         */
        
        // backwards proposal
        // double bwdAlpha = beta * newUnitWeight / (1.0 - newUnitWeight);
        double new_a = lambda * newUnitWeight + 1.0;
        double new_b = lambda * (1.0 - newUnitWeight) + 1.0;
        double bwdLnProb = RbStatistics::Beta::lnPdf(new_a, new_b, unitWeight);

        
        // prior
        double prior_a = 1.0;
        double prior_b = 1.0;
        bwdLnProb += RbStatistics::Beta::lnPdf(prior_a, prior_b, newUnitWeight); // bwd move and fwd prior
        fwdLnProb += RbStatistics::Beta::lnPdf(prior_a, prior_b, unitWeight); // fwd move and bwd prior
        /*
         // now we store all necessary values
         storedNode = node;
         storedAge = my_age;
         
         // draw new ages and compute the hastings ratio at the same time
         double m = (my_age-child_Age) / (parent_age-child_Age);
         double a = delta * m + 1.0;
         double b = delta * (1.0-m) + 1.0;
         double new_m = RbStatistics::Beta::rv(a, b, *rng);
         double my_new_age = (parent_age-child_Age) * new_m + child_Age;
         
         // compute the Hastings ratio
         double forward = RbStatistics::Beta::lnPdf(a, b, new_m);
         double new_a = delta * new_m + 1.0;
         double new_b = delta * (1.0-new_m) + 1.0;
         double backward = RbStatistics::Beta::lnPdf(new_a, new_b, m);
         
         // set the age
         node->setAge( my_new_age );
         
         return backward - forward;
         */
        
        // set the weight
        storedAdmixtureChild->setWeight( newWeight );
        
        //std::cout  << "reweight\t" << bwdLnProb << " - " << fwdLnProb << " = " << bwdLnProb - fwdLnProb << ";\t" << unitWeight << " -> " << newUnitWeight << ";\t" << storedWeight << " -> " << newWeight << "\n";
        
        // uniform on 0,1 is symmetric
        return bwdLnProb - fwdLnProb;
    }
   
}

bool AdmixtureEdgeReweight::isActive(unsigned long g) const {
    
    return g > activeGen;
}


void AdmixtureEdgeReweight::rejectSimpleMove( void ) {
    
    // undo the proposal
    if (!failed)
    {
        storedAdmixtureChild->setWeight( storedWeight );
    }
}


void AdmixtureEdgeReweight::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
}
