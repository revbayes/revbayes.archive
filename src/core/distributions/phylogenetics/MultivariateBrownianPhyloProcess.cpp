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
MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< PrecisionMatrix >* inomega, const TypedDagNode< std::vector<double> >* inrootVal) : TypedDistribution< MatrixReal >( new MatrixReal(intau->getValue().getNumberOfNodes(), inomega->getValue().getDim(), 0.0 )),
tau( intau ),
omega( inomega ),
rootVal( inrootVal ) {
    this->addParameter( tau );
    this->addParameter( omega );
    this->addParameter( rootVal );
    
    simulate();
}


MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &n): TypedDistribution< MatrixReal>( n ), tau( n.tau ), omega( n.omega ), rootVal( n.rootVal ) {
    // nothing to do here since the parameters are copied automatically
    
}

MultivariateBrownianPhyloProcess* MultivariateBrownianPhyloProcess::clone(void) const {
    return new MultivariateBrownianPhyloProcess( *this );
}



double MultivariateBrownianPhyloProcess::computeLnProbability(void) {
    
    double lnProb = 0;
    if (omega->getValue().isPositive()) {
        lnProb = 0.5 * log(omega->getValue().getLogDet()) + recursiveLnProb(tau->getValue().getRoot());
    }
    else{
        lnProb = RbConstants::Double::neginf;
    }
    return lnProb;
}


double MultivariateBrownianPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    std::vector<double> val = (*value)[index];
    
    if (! from.isRoot()) {
        
        // x ~ normal(x_up, omega^2 * branchLength)
        
        size_t upindex = from.getParent().getIndex();
        std::vector<double> upval = (*value)[upindex];
        
        const PrecisionMatrix& om = omega->getValue();
        
        double s2 = 0;
        for (size_t i=0; i<getDim(); i++)   {
            double tmp = 0;
            for (size_t j=0; j<getDim(); j++)   {
                tmp += om[i][j] * (val[j] - upval[j]);
            }
            s2 += (val[i] - upval[i]) * tmp;
        }
        
        lnProb -= 0.5 * s2;
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
        
        // x ~ normal(x_up, omega^2 * branchLength)

        std::vector<double>& val = (*value)[index];
        
        omega->getValue().drawNormalSample((*value)[index]);

        size_t upindex = from.getParent().getIndex();
        std::vector<double>& upval = (*value)[upindex];

        for (size_t i=0; i<getDim(); i++)   {
            val[i] += upval[i];
        }
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
    
    if ( oldP == omega ) {
        omega = static_cast< const TypedDagNode<PrecisionMatrix> * >( newP );
    }
    
    if ( oldP == rootVal ) {
        rootVal = static_cast< const TypedDagNode< std::vector<double> > * >( newP );
    }
}

