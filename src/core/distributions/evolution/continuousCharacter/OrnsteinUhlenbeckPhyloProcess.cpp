/* 
 * File:   OrnsteinUhlenbeckPhyloProcess.cpp
 * Author: nl
 * 
 * Created on 26 juillet 2014, 15:03
 */

#include "OrnsteinUhlenbeckPhyloProcess.h"
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
OrnsteinUhlenbeckPhyloProcess::OrnsteinUhlenbeckPhyloProcess(const TypedDagNode< TimeTree > *t, const TypedDagNode< double >* s, const TypedDagNode< double >* m, const TypedDagNode< double >* p): TypedDistribution< RealNodeContainer >(new RealNodeContainer(&t->getValue())),
    tau( t ),
    sigma( s ),
    mean( m ),
    phi( p )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( sigma );
    addParameter( mean );
    addParameter( phi );
    
    simulate();
}



OrnsteinUhlenbeckPhyloProcess* OrnsteinUhlenbeckPhyloProcess::clone(void) const {
    return new OrnsteinUhlenbeckPhyloProcess( *this );
}


double OrnsteinUhlenbeckPhyloProcess::computeLnProbability(void) {
    
    double ret = recursiveLnProb(tau->getValue().getRoot());
    return ret;
}


double OrnsteinUhlenbeckPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    double val = (*value)[index];

    if (from.isRoot()) {

        double m = mean->getValue();
        double standDev = sigma->getValue() / sqrt(2 * phi->getValue());
        
        lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);
    }
    else    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        double upval = (*value)[from.getParent().getIndex()];
        double t = from.getBranchLength();
        double e = exp(-phi->getValue() * t);
        double e2 = exp(-2 * phi->getValue() * t);
        double m = e * upval + (1 - e) * mean->getValue();
        double standDev = sigma->getValue() * sqrt((1 - e2) / 2 / phi->getValue());

        lnProb += RbStatistics::Normal::lnPdf(val, standDev, m);
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        lnProb += recursiveLnProb(from.getChild(i));
    }
    
    return lnProb;
    
}

void OrnsteinUhlenbeckPhyloProcess::redrawValue(void) {
    simulate();
}


void OrnsteinUhlenbeckPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void OrnsteinUhlenbeckPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    
    if (from.isRoot())    {
        
        // x ~ normal(mean, sigma^2 / 2 phi)

        double m = mean->getValue();
        double standDev = sigma->getValue() / sqrt(2 * phi->getValue());
        
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv(m, standDev, *rng);
    }
    else    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)

        double upval = (*value)[from.getParent().getIndex()];
        double t = from.getBranchLength();
        double e = exp(-phi->getValue() * t);
        double e2 = exp(-2 * phi->getValue() * t);
        double m = e * upval + (1 - e) * mean->getValue();
        double standDev = sigma->getValue() * sqrt((1 - e2) / 2 / phi->getValue());

        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        (*value)[index] = RbStatistics::Normal::rv(m, standDev, *rng);
     
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveSimulate(from.getChild(i));
    }
    
}


/** Swap a parameter of the distribution */
void OrnsteinUhlenbeckPhyloProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<double> * >( newP );
    }
    
    if ( oldP == mean ) {
        mean = static_cast< const TypedDagNode< double > * >( newP );
    }
    
    if ( oldP == phi ) {
        phi = static_cast< const TypedDagNode< double > * >( newP );
    }
}

