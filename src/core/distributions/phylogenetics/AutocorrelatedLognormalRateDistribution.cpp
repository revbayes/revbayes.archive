#include "AutocorrelatedLognormalRateDistribution.h"
#include "ConstantNode.h"
#include "DistributionLognormal.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>



using namespace RevBayesCore;



// constructor(s)
AutocorrelatedLognormalRateDistribution::AutocorrelatedLognormalRateDistribution(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rr): TypedDistribution< std::vector< double > >( new std::vector< double >(t->getValue().getNumberOfNodes(), 0.0 ) ), 
        tau( t ), 
        sigma( s ), 
        rootRate( rr ) {
    this->addParameter( tau );
    this->addParameter( sigma );
    this->addParameter( rootRate );
    
    simulate();
}


AutocorrelatedLognormalRateDistribution::AutocorrelatedLognormalRateDistribution(const AutocorrelatedLognormalRateDistribution &n): TypedDistribution< std::vector< double > >( n ), tau( n.tau ), sigma( n.sigma ), rootRate( n.rootRate ) {
    // nothing to do here since the parameters are copied automatically
    
}



AutocorrelatedLognormalRateDistribution* AutocorrelatedLognormalRateDistribution::clone(void) const {
    return new AutocorrelatedLognormalRateDistribution( *this );
}


double AutocorrelatedLognormalRateDistribution::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex= root.getIndex();
    
    double lnProb = 0.0;
    
    double parentRate = rootRate->getValue();
    // check if the root rate matches the parameter value for the root rate
    // the move could have changed this
    if ( (*value)[rootIndex] != parentRate ) {
        lnProb = RbConstants::Double::neginf;
    } else {
        // we need to take the log of the root rate because we expect the parent rate to be the mean of the normal distribution
        parentRate = log( parentRate );
        size_t numChildren = root.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = root.getChild(i);
            lnProb += recursiveLnProb(child);
            
            // compute the variance
            double standDev = sigma->getValue() * child.getBranchLength();
            
            size_t childIndex = child.getIndex();
            double childRate = (*value)[childIndex];
            lnProb += log( RbStatistics::Lognormal::lnPdf(parentRate, standDev, childRate) );
            
        } 
    }
    
    return lnProb;
}


void AutocorrelatedLognormalRateDistribution::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->getAffectedNodes( affected );
}


void AutocorrelatedLognormalRateDistribution::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was the root rate
    if ( affecter == rootRate )
        this->dagNode->keepAffected();
}



double AutocorrelatedLognormalRateDistribution::recursiveLnProb( const TopologyNode& n ) {
    
    // get the index
    size_t nodeIndex = n.getIndex();
    
    double lnProb = 0.0;
    size_t numChildren = n.getNumberOfChildren();
    
    if ( numChildren > 0 ) {
        double parentRate = log( (*value)[nodeIndex] );
        
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = n.getChild(i);
            lnProb += recursiveLnProb(child);
            
            // compute the variance
            double standDev = sigma->getValue() * child.getBranchLength();
            
            size_t childIndex = child.getIndex();
            double childRate = (*value)[childIndex];
            lnProb += log( RbStatistics::Lognormal::lnPdf(parentRate, standDev, childRate) );
                
        } 
    }
    
    return lnProb;
    
}


void AutocorrelatedLognormalRateDistribution::redrawValue(void) {
    simulate();
    std::cerr << "ACLN:\t\t" << *value << std::endl;
}


void AutocorrelatedLognormalRateDistribution::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
    if ( restorer == rootRate ) {
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootRate->getValue();
        
        // delegate a restore to our children
        this->dagNode->restoreAffected();
    }
}


void AutocorrelatedLognormalRateDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == rootRate ) {
        rootRate = static_cast< const TypedDagNode< double > * >( newP );
    }
}


void AutocorrelatedLognormalRateDistribution::simulate() {
        
    // get the initial rate
    double parentRate = rootRate->getValue();
        
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    // get the index of the root
    size_t rootIndex = root.getIndex();
    
    // for savety reasons we store this rate here again
    (*value)[rootIndex] = parentRate;
    
    size_t numChildren = root.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        recursiveSimulate(child,parentRate);
    }
    
}




void AutocorrelatedLognormalRateDistribution::recursiveSimulate(const TopologyNode& node, double parentRate) {
    
    // get the index
    size_t nodeIndex = node.getIndex();
    
    // compute the variance along the branch
    double standDev = sigma->getValue() * node.getBranchLength();
    
    // simulate a new rate
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double nodeRate = RbStatistics::Lognormal::rv( log(parentRate), standDev, *rng);
    
    // we store this rate here
    (*value)[nodeIndex] = nodeRate;
    
    
    // simulate the rate for each child (if any)
    size_t numChildren = node.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = node.getChild(i);
        recursiveSimulate(child,nodeRate);
    }
}



void AutocorrelatedLognormalRateDistribution::touchSpecialization( DagNode *toucher ) {
    // if the root rate has changed, then we need to change the corresponding value in our vector and downpass the touch
    if ( rootRate == toucher ) {
        
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootRate->getValue();
        
        // we notify our DAG node which element has changed
        dagNode->addTouchedElementIndex( rootIndex );
        
        // delegate a touch to our children
        this->dagNode->touchAffected();
    }
}

