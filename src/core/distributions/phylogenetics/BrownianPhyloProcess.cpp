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
    
    return recursiveLnProb(tau->getValue().getRoot());
    
}


double BrownianPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    

    double zob;
    double lnProb = 0.0;
    size_t index = from.getIndex();
    double val = (*value)[index];

    if (! from.isRoot()) {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        double upval = (*value)[upindex];
        double standDev = sigma->getValue() * from.getBranchLength();
        lnProb += log( RbStatistics::Normal::lnPdf(val, standDev, upval) );
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        lnProb += recursiveLnProb(from.getChild(i));
    }
    
    return lnProb;
    
}

void BrownianPhyloProcess::redrawValue(void) {
    simulate();
    std::cerr << "Brownian:\t\t" << *value << std::endl;
}


void BrownianPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void BrownianPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    
    if (! from.isRoot())    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        double upval = (*value)[upindex];
        double standDev = sigma->getValue() * from.getBranchLength();

        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv( upval, standDev, *rng);
     
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveSimulate(from.getChild(i));
    }
    
}


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
