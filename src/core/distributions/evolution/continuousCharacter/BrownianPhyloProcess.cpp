#include "BrownianPhyloProcess.h"
#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "GtrRateMatrix.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <cmath>
#include <iostream>



using namespace RevBayesCore;



// constructor(s)
BrownianPhyloProcess::BrownianPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* d): TypedDistribution< RealNodeContainer >(new RealNodeContainer(&t->getValue())),
        tau( t ), 
        sigma( s ), 
        drift( d ) {
    
    simulate();
}


BrownianPhyloProcess::BrownianPhyloProcess(const BrownianPhyloProcess &n): TypedDistribution< RealNodeContainer >( n ), tau( n.tau ), sigma( n.sigma ), drift( n.drift ) {
    // nothing to do here since the parameters are copied automatically
    
}



BrownianPhyloProcess* BrownianPhyloProcess::clone(void) const {
    return new BrownianPhyloProcess( *this );
}


double BrownianPhyloProcess::computeLnProbability(void) {
    
    double ret = recursiveLnProb(tau->getValue().getRoot());
    return ret;
}


double BrownianPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    double val = (*value)[index];

    if (! from.isRoot()) {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        double standDev = sigma->getValue() * sqrt(from.getBranchLength());
        double mean = (*value)[upindex] + drift->getValue() * from.getBranchLength();
        lnProb += RbStatistics::Normal::lnPdf(val, standDev, mean);
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
}


void BrownianPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void BrownianPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    
    if (! from.isRoot())    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        double standDev = sigma->getValue() * sqrt(from.getBranchLength());
        double mean = (*value)[upindex] + drift->getValue() * from.getBranchLength();

        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv( mean, standDev, *rng);
     
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveSimulate(from.getChild(i));
    }
    
}


/** Get the parameters of the distribution */
std::set<const DagNode*> BrownianPhyloProcess::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( tau );
    parameters.insert( sigma );
    parameters.insert( drift );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void BrownianPhyloProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == drift ) {
        drift = static_cast< const TypedDagNode< double > * >( newP );
    }
}
