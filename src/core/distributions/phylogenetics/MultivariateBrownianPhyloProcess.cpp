//
//  MultivariateBrownianPhyloProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//


#include "MultivariateBrownianPhyloProcess.h"

#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "StochasticNode.h"

#include <iostream>

using namespace RevBayesCore;



// constructor(s)
MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< CovarianceMatrix >* insigma, const TypedDagNode< std::vector<double> >* inrootVal) : TypedDistribution< MatrixReal >( new MatrixReal(intau->getValue().getNumberOfNodes(), insigma->getValue().getDim(), 0.0 )),
tau( intau ),
sigma( insigma ),
rootVal( inrootVal ) {
    this->addParameter( tau );
    this->addParameter( sigma );
    this->addParameter( rootVal );
    
    simulate();
}


MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &n): TypedDistribution< MatrixReal>( n ), tau( n.tau ), sigma( n.sigma ), rootVal( n.rootVal ) {
    // nothing to do here since the parameters are copied automatically
    
}

MultivariateBrownianPhyloProcess* MultivariateBrownianPhyloProcess::clone(void) const {
    return new MultivariateBrownianPhyloProcess( *this );
}



double MultivariateBrownianPhyloProcess::computeLnProbability(void) {
    
    return recursiveLnProb(tau->getValue().getRoot());
    
}


double MultivariateBrownianPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    std::vector<double> val = (*value)[index];
    
    if (! from.isRoot()) {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        std::vector<double> upval = (*value)[upindex];
//        double standDev = sigma->getValue() * from.getBranchLength();
//        lnProb += log( RbStatistics::Normal::lnPdf(val, standDev, upval) );
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        lnProb += recursiveLnProb(from.getChild(i));
    }
    
    return lnProb;
    
}

void MultivariateBrownianPhyloProcess::redrawValue(void) {
    simulate();
    std::cerr << "Brownian:\t\t" << *value << std::endl;
}


void MultivariateBrownianPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void MultivariateBrownianPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    
    if (! from.isRoot())    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        std::vector<double> upval = (*value)[upindex];
//        double standDev = sigma->getValue() * from.getBranchLength();
        
        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
  //      (*value)[index] = RbStatistics::Normal::rv( upval, standDev, *rng);
        
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveSimulate(from.getChild(i));
    }
    
}


void MultivariateBrownianPhyloProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<CovarianceMatrix> * >( newP );
    }
    
    if ( oldP == rootVal ) {
        rootVal = static_cast< const TypedDagNode< std::vector<double> > * >( newP );
    }
}

