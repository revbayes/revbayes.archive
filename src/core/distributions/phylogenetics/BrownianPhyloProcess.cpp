#include "BrownianPhyloProcess.h"
#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>



using namespace RevBayesCore;



// constructor(s)
BrownianPhyloProcess::BrownianPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* rv): TypedDistribution< std::vector< double > >( new std::vector< double >(t->getValue().getNumberOfNodes(), 0.0 ) ),
        tau( t ), 
        sigma( s ), 
        rootVal( rv ) {
    this->addParameter( tau );
    this->addParameter( sigma );
    this->addParameter( rootVal );
    
    simulate();
}


BrownianPhyloProcess::BrownianPhyloProcess(const BrownianPhyloProcess &n): TypedDistribution< std::vector< double > >( n ), tau( n.tau ), sigma( n.sigma ), rootVal( n.rootVal ) {
    // nothing to do here since the parameters are copied automatically
    
}



BrownianPhyloProcess* BrownianPhyloProcess::clone(void) const {
    return new BrownianPhyloProcess( *this );
}


double BrownianPhyloProcess::computeLnProbability(void) {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    size_t rootIndex= root.getIndex();
    
    double lnProb = 0.0;
    
    double parentVal = rootVal->getValue();
    // check if the root val matches the parameter value for the root val
    // the move could have changed this
    if ( (*value)[rootIndex] != parentVal ) {
        lnProb = RbConstants::Double::neginf;
    } else {
        // we need to take the log of the root val because we expect the parent val to be the mean of the normal distribution
    
        size_t numChildren = root.getNumberOfChildren();
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = root.getChild(i);
            lnProb += recursiveLnProb(child);
            
            // compute the variance
            double standDev = sigma->getValue() * child.getBranchLength();
            
            size_t childIndex = child.getIndex();
            double childVal = (*value)[childIndex];
            lnProb += log( RbStatistics::Normal::lnPdf(parentVal, standDev, childVal) );
            
        } 
    }
    
    return lnProb;
}

/*
void BrownianPhyloProcess::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    // only delegate when the toucher was the root val
    if ( affecter == rootVal )
        this->dagNode->getAffectedNodes( affected );
}


void BrownianPhyloProcess::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was the root val
    if ( affecter == rootVal )
        this->dagNode->keepAffected();
}
*/


double BrownianPhyloProcess::recursiveLnProb( const TopologyNode& n ) {
    
    // get the index
    size_t nodeIndex = n.getIndex();
    
    double lnProb = 0.0;
    size_t numChildren = n.getNumberOfChildren();
    
    if ( numChildren > 0 ) {
        double parentVal = (*value)[nodeIndex];
        
        for (size_t i = 0; i < numChildren; ++i) {
            const TopologyNode& child = n.getChild(i);
            lnProb += recursiveLnProb(child);
            
            // compute the variance
            double standDev = sigma->getValue() * child.getBranchLength();
            
            size_t childIndex = child.getIndex();
            double childVal = (*value)[childIndex];
            lnProb += log( RbStatistics::Normal::lnPdf(parentVal, standDev, childVal) );
                
        } 
    }
    
    return lnProb;
    
}


void BrownianPhyloProcess::redrawValue(void) {
    simulate();
    std::cerr << "Brownian:\t\t" << *value << std::endl;
}

/*
void BrownianPhyloProcess::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
    if ( restorer == rootVal ) {
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootVal->getValue();
        
        // delegate a restore to our children
        this->dagNode->restoreAffected();
    }
}
*/

void BrownianPhyloProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == rootVal ) {
        rootVal = static_cast< const TypedDagNode< double > * >( newP );
    }
}


void BrownianPhyloProcess::simulate() {
        
    // get the initial val
    double parentVal = rootVal->getValue();
        
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    
    // get the index of the root
    size_t rootIndex = root.getIndex();
    
    // for savety reasons we store this val here again
    (*value)[rootIndex] = parentVal;
    
    size_t numChildren = root.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = root.getChild(i);
        recursiveSimulate(child,parentVal);
    }
    
}


void BrownianPhyloProcess::recursiveSimulate(const TopologyNode& node, double parentVal) {
    
    // get the index
    size_t nodeIndex = node.getIndex();
    
    // compute the variance along the branch
    double standDev = sigma->getValue() * node.getBranchLength();
    
    // simulate a new Val
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double nodeVal = RbStatistics::Normal::rv( parentVal, standDev, *rng);
    
    // we store this val here
    (*value)[nodeIndex] = nodeVal;
    
    
    // simulate the val for each child (if any)
    size_t numChildren = node.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = node.getChild(i);
        recursiveSimulate(child,nodeVal);
    }
}


/*
void BrownianPhyloProcess::touchSpecialization( DagNode *toucher ) {
    // if the root val has changed, then we need to change the corresponding value in our vector and downpass the touch
    if ( rootVal == toucher ) {
        
        // get the index of the root
        size_t rootIndex = tau->getValue().getRoot().getIndex();
        (*value)[rootIndex] = rootVal->getValue();
        
        // we notify our DAG node which element has changed
        dagNode->addTouchedElementIndex( rootIndex );
        
        // delegate a touch to our children
        this->dagNode->touchAffected();
    }
}
*/
