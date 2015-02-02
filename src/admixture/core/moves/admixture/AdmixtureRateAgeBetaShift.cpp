//
//  AdmixtureRateAgeBetaShift.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 7/15/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureRateAgeBetaShift.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "AdmixtureRateAgeBetaShift.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureRateAgeBetaShift::AdmixtureRateAgeBetaShift(std::vector<DagNode*> v, double d, bool t, double w) : CompoundMove( v, w, t), delta( d ) {
    
    tree = static_cast<StochasticNode<AdmixtureTree>* > (v[0]);
    for (size_t i = 1; i < v.size(); i++)
        rates.push_back( static_cast<StochasticNode<double>* >(v[i]) );
    
}



/* Clone object */
AdmixtureRateAgeBetaShift* AdmixtureRateAgeBetaShift::clone( void ) const {
    
    return new AdmixtureRateAgeBetaShift( *this );
}



const std::string& AdmixtureRateAgeBetaShift::getMoveName( void ) const {
    static std::string name = "AdmixtureRateAgeBetaShift";
    
    return name;
}


/** Perform the move */
double AdmixtureRateAgeBetaShift::performCompoundMove( void ) {
    
    // clear rates map
    storedRates.clear();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = tree->getValue();
    
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
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    int nodeIdx = (int)node->getIndex();
    storedRates[nodeIdx] = rates[nodeIdx]->getValue();
    for (size_t i = 0; i < node->getNumberOfChildren(); i++)
    {
        int childIdx = (int)node->getChild(i).getIndex();
        storedRates[childIdx] = rates[childIdx]->getValue();
    }
    
    
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
    tau.setAge( node->getIndex(), my_new_age );
    
    // set the rates
    rates[nodeIdx]->setValue( new double((node->getParent().getAge() - my_age) * storedRates[nodeIdx] / (node->getParent().getAge() - my_new_age)));
    for (size_t i = 0; i < node->getNumberOfChildren(); i++)
    {
        int childIdx = (int)node->getChild(i).getIndex();
        rates[childIdx]->setValue( new double((my_age - node->getChild(i).getAge()) * storedRates[childIdx] / (my_new_age - node->getChild(i).getAge())));
    }
    
    return backward - forward;
}


void AdmixtureRateAgeBetaShift::printParameterSummary(std::ostream &o) const {
    o << "delta = " << delta;
}


void AdmixtureRateAgeBetaShift::rejectCompoundMove( void ) {
    
    // undo the proposal
    tree->getValue().setAge( storedNode->getIndex(), storedAge );
    
    // undo the rates
    int nodeIdx = (int)storedNode->getIndex();
    rates[nodeIdx]->setValue(new double(storedRates[nodeIdx]));
    for (size_t i = 0; i < storedNode->getNumberOfChildren(); i++)
    {
        int childIdx = (int)storedNode->getChild(i).getIndex();
        rates[childIdx]->setValue(new double(storedRates[childIdx]));
    }
    
    
#ifdef ASSERTIONS_TREE
    if ( fabs(storedAge - storedNode->getAge()) > 1E-8 ) {
        throw RbException("Error while rejecting AdmixtureRateAgeBetaShift proposal: Node ages were not correctly restored!");
    }
#endif
    
}


void AdmixtureRateAgeBetaShift::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    CompoundMove::swapNode(oldN, newN);
    
    if (oldN == tree)
        tree = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    else
    {
        for (size_t i = 0; i < rates.size(); i++)
        {
            if (oldN == rates[i])
            {
                rates[i] = static_cast<StochasticNode<double>* >(newN);
                break;
            }
        }
    }
}


void AdmixtureRateAgeBetaShift::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 ) {
        delta /= (1.0 + ((rate-0.44)/0.56) );
    }
    else {
        delta *= (2.0 - rate/0.44 );
    }
}


