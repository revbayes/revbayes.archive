//
//  AdmixtureShiftTreeRates.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 7/5/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureShiftTreeRates.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureShiftTreeRates::AdmixtureShiftTreeRates(StochasticNode<double> *v, std::vector<ContinuousStochasticNode*> br, double d, bool t, double w) : MoveOld( v, w, t), treeRate( v ), branchRates(br), changed(false), failed(false), delta(d) {
    
    for (size_t i = 0; i < branchRates.size(); i++)
        nodes.insert(branchRates[i]);
}



/* Clone object */
AdmixtureShiftTreeRates* AdmixtureShiftTreeRates::clone( void ) const {
    
    return new AdmixtureShiftTreeRates( *this );
}



const std::string& AdmixtureShiftTreeRates::getMoveName( void ) const {
    static std::string name = "AdmixtureShiftTreeRates";
    
    return name;
}


/** Perform the move */
double AdmixtureShiftTreeRates::performSimpleMove( void ) {
    
    
    //std::cout << "\nTreeRate Shift\n";
    
    // clear old rates
    storedRates.clear();
    
    // rescale tree rate
    storedTreeRate = treeRate->getValue();
    double scaler = exp(delta*(GLOBAL_RNG->uniform01() - 0.5));
    treeRate->setValue(new double(storedTreeRate * scaler));
    
    //std::cout << treeRate->getName() << " " << storedTreeRate << " -> " << storedTreeRate * scaler << "\n";
    
    // rescale branch rates
    
    // random number of branches??
    // ...
    
    for (int i = 0; i < (int)branchRates.size(); i++)
    {
        double v = branchRates[i]->getValue();
        storedRates[i] = v;
        branchRates[i]->setValue(new double(v/scaler));
        //std::cout << branchRates[i]->getName() << " " << v << " -> " << v/scaler << "\n";
    }
    
    // proposal ratio
    return log(scaler);
}


void AdmixtureShiftTreeRates::rejectSimpleMove( void ) {

    treeRate->setValue(new double(storedTreeRate));
    for (std::map<int,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    {
        int idx = it->first;
        branchRates[idx]->setValue(new double(it->second));
    }
}

void AdmixtureShiftTreeRates::acceptMove( void ) {
    // nothing to do
    changed = false;
    //std::cout << "ACCEPT treeshift\n";
    acceptSimpleMove();
}

void AdmixtureShiftTreeRates::acceptSimpleMove( void ) {
    // do nothing
    ;
}

double AdmixtureShiftTreeRates::performMove( double &probRatio ) {
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    treeRate->touch();
    
    // calculate the probability ratio for the node we just changed
    probRatio = treeRate->getLnProbabilityRatio();
    //std::cout << "n\t" << treeRate->getName() << "\t" << probRatio << "\n";
    
    //for (size_t i = 0; i < branchRates.size(); i++)
    for (std::map<int,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    {
        int idx = it->first;
        branchRates[idx]->touch();
        probRatio += branchRates[idx]->getLnProbabilityRatio();
        //std::cout << "n\t" << branchRates[idx]->getName() << " " << branchRates[idx]->getLnProbability() << " " << exp(branchRates[idx]->getLnProbability()) << " " << probRatio << "\n";
    }
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        treeRate->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
           // std::cout << "\tch\t" << theNode->getName() << "\t" << theNode->getLnProbabilityRatio() << "  " <<  probRatio << "\n";
        }
    }
    
    return hr;
}

void AdmixtureShiftTreeRates::rejectMove( void ) {
    changed = false;
    //std::cout << "REJECT treeshift\n";
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    //variable->touch();
    //for (std::map<AdmixtureNode*,double>::iterator it = storedRates.begin(); it != storedRates.end(); it++)
    //    branchRates[it->first->getIndex()]->touch();
}


void AdmixtureShiftTreeRates::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN,newN);
    if (oldN == treeRate)
        treeRate = static_cast<StochasticNode<double>* >(newN) ;
    for (size_t i = 0; i < branchRates.size(); i++)
    {
        if (oldN == branchRates[i])
        {
            branchRates[i] = static_cast<ContinuousStochasticNode*>(newN) ;
        }
    }
}

void AdmixtureShiftTreeRates::tune( void )
{
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta *= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta /= (2.0 - rate/0.234 );
    }
}
