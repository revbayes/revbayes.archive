//
//  AdmixtureAdmixtureConstantBirthDeathProcess.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureConstantBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

AdmixtureConstantBirthDeathProcess::AdmixtureConstantBirthDeathProcess(const TypedDagNode<double> *d, const TypedDagNode<double> *t, unsigned int nTaxa, const std::vector<std::string> &tn) : TypedDistribution<AdmixtureTree>( new AdmixtureTree() ), diversification( d ), turnover( t ), numTaxa( nTaxa ), taxonNames( tn ) {
    addParameter( diversification );
    addParameter( turnover );
    
    double lnFact = 0.0;
    for (size_t i = 2; i < numTaxa; i++) {
        lnFact += std::log(i);
    }
    
    logTreeTopologyProb = (numTaxa - 1) * RbConstants::LN2 - 2.0 * lnFact - std::log( numTaxa ) ;
    
    simulateTree();
    
}



AdmixtureConstantBirthDeathProcess::AdmixtureConstantBirthDeathProcess(const AdmixtureConstantBirthDeathProcess &v) : TypedDistribution<AdmixtureTree>( v ), diversification( v.diversification ), turnover( v.turnover ), numTaxa( v.numTaxa ), taxonNames( v.taxonNames ), logTreeTopologyProb( v.logTreeTopologyProb ) {
    // parameters are automatically copied
}


AdmixtureConstantBirthDeathProcess::~AdmixtureConstantBirthDeathProcess() {
    
}



void AdmixtureConstantBirthDeathProcess::attachTimes(std::vector<AdmixtureNode *> &tips, int index, const std::vector<double> &times, double T) {
    
    if (index < numTaxa-1) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t tip_index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        AdmixtureNode* parent = tips.at(tip_index);
        parent->setAge( T - times[index] );
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+tip_index);
        
        // add a left child
        AdmixtureNode* leftChild = &parent->getChild(0);
        if ( !leftChild->isTip() ) {
            tips.push_back(leftChild);
        }
        
        // add a right child
        AdmixtureNode* rightChild = &parent->getChild(1);
        if ( !rightChild->isTip() ) {
            tips.push_back(rightChild);
        }
        
        // recursive call to this function
        attachTimes(tips, index+1, times, T);
    }
}


void AdmixtureConstantBirthDeathProcess::buildRandomBinaryTree(std::vector<AdmixtureNode*> &tips) {
    
    if (tips.size() < numTaxa) {
        // Get the rng
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        // randomly draw one node from the list of tips
        size_t index = static_cast<size_t>( floor(rng->uniform01()*tips.size()) );
        
        // get the node from the list
        TopologyNode* parent = tips.at(index);
        
        // remove the randomly drawn node from the list
        tips.erase(tips.begin()+index);
        
        // add a left child
        AdmixtureNode* leftChild = new AdmixtureNode(0);
        parent->addChild(leftChild);
        leftChild->setParent(parent);
        tips.push_back(leftChild);
        
        // add a right child
        AdmixtureNode* rightChild = new AdmixtureNode(0);
        parent->addChild(rightChild);
        rightChild->setParent(parent);
        tips.push_back(rightChild);
        
        // recursive call to this function
        buildRandomBinaryTree(tips);
    }
}


AdmixtureConstantBirthDeathProcess* AdmixtureConstantBirthDeathProcess::clone( void ) const {
    return new AdmixtureConstantBirthDeathProcess( *this );
}


double AdmixtureConstantBirthDeathProcess::computeLnProbability( void ) {
    
    // ... may not matter, but branch lengths are uniformly distributed while testing likelihood fn...
    return 0.0;
    
    // retrieved the speciation times
    std::vector<double> times;
    for (int i = 0; i < value->getNumberOfInteriorNodes()+1; ++i) {
        double t = value->getInteriorNode( i ).getTime();
        times.push_back(t);
    }
    // sort the vector of times in ascending order
    std::sort(times.begin(), times.end());
    
    // present time
    double T = value->getTipNode(0).getTime();
    
    int numInitialSpecies = 1;
    // check if we condition on the root or origin
    if ( times[0] == 0.0 ) {
        //        lnProbTimes = - 2 * log( pSurvival(0,T) );
        ++numInitialSpecies;
    } else {
        //        lnProbTimes = - log( pSurvival(0,T) );
    }
    
    double lnProbTimes = numInitialSpecies * ( p1(0,T) - log(pSurvival(0,T,T)) );
    
    for (int i = (numInitialSpecies-1); i < numTaxa-1; ++i) {
        if ( lnProbTimes == RbConstants::Double::nan || lnProbTimes == RbConstants::Double::inf || lnProbTimes == RbConstants::Double::neginf) {
            return RbConstants::Double::nan;
        }
        lnProbTimes += log(diversification->getValue() + turnover->getValue()) + p1(times[i],T);
    }
    
    return lnProbTimes; // + logTreeTopologyProb;
    
}


double AdmixtureConstantBirthDeathProcess::p1(double t, double T) const {
    double a = log(pSurvival(t,T,T));
    double b = diversification->getValue() * (t-T);
    
    if ( b > 0 ) {
        return RbConstants::Double::neginf;
    }
    
    //    // remove the probability that we might not have sampled this species (or any descendant)
    //    double samplingProbability = rho->getValue().evaluate(t);
    //    b -= log(samplingProbability);
    
    double p = 2.0 * a + b;
    
    return p;
    
}


double AdmixtureConstantBirthDeathProcess::pSurvival(double start, double end, double T) const {
    
    double div = diversification->getValue();
    double xxx = ( exp( div * (start-end) ) - 1.0 ) / (-div);
    double den = 1.0 + turnover->getValue() * ( xxx );// - ( rho - 1.0 ) * exp( rate(start,end,lambda,mu,rho) );
    
    return (1.0 / den);
}


void AdmixtureConstantBirthDeathProcess::redrawValue( void ) {
    simulateTree();
}



double AdmixtureConstantBirthDeathProcess::simOrigin( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // random draw
    double u = rng->uniform01();
    
    // get the parameters
    double div = diversification->getValue();
    double mu = turnover->getValue();
    double lambda = div + mu;
    
	double origin = std::log( (-lambda + mu * std::pow(u, (1.0/numTaxa) )) / (lambda * (-1 + std::pow(u,(1.0/numTaxa))))) / div;
	
    return origin;
}


double AdmixtureConstantBirthDeathProcess::simSpeciation(double origin) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
	double u = rng->uniform01();
    
    // get the parameters
    double div = diversification->getValue();
    double mu = turnover->getValue();
    double lambda = div + mu;
    
    double t = 1.0/div * log((lambda - mu * exp((-div)*origin) - mu * (1.0 - exp((-div) * origin)) * u )/(lambda - mu * exp((-div) * origin) - lambda * (1.0 - exp(( -div ) * origin)) * u ) );
	
    return t;
}


void AdmixtureConstantBirthDeathProcess::simulateTree( void ) {
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // Draw a random topology
    AdmixtureTree tau = AdmixtureTree();
    // internally we treat unrooted topologies the same as rooted
    tau.setRooted( true );
    
    AdmixtureNode* root = new AdmixtureNode();
    std::vector<AdmixtureNode* > nodes;
    nodes.push_back(root);
    // recursively build the tree
    buildRandomBinaryTree(nodes);
    
    // set tip names
    for (int i=0; i<numTaxa; i++) {
        size_t index = size_t( floor(rng->uniform01() * nodes.size()) );
        
        // get the node from the list
        TopologyNode* node = nodes.at(index);
        
        // remove the randomly drawn node from the list
        nodes.erase(nodes.begin()+index);
        
        // set name
        std::string& name = taxonNames[i];
        node->setName(name);
    }
    
    // initialize the topology by setting the root
    tau.setRoot(root);
    
    // now simulate the speciation times
    // draw a sample for the origin

    // unit tree
	double t_or = 1.0; // simOrigin();
    
    // allocate the vector for the times
    std::vector<double> t = std::vector<double>(numTaxa,0.0);
	t[0] = 0.0;
    
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 1; i < numTaxa; ++i) {
		t[i] = t_or - simSpeciation(t[i-1]);
	}
    
    std::sort(t.begin(),t.end());
    
    nodes.clear();
    nodes.push_back( root );
    attachTimes(nodes, 0, t, t_or);
    for (size_t i = 0; i < numTaxa; ++i) {
        AdmixtureNode& n = tau.getTipNode(i);
        n.setName(taxonNames[n.getIndex()]); // match taxonName and index orders
        n.setAge( 0.0 );
    }
    
    // finally store the new value
    *value = tau;
    
}



void AdmixtureConstantBirthDeathProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == diversification) {
        diversification = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == turnover) {
        turnover = static_cast<const TypedDagNode<double>* >( newP );
    }
}