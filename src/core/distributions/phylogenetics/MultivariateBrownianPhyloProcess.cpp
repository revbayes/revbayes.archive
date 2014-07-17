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
// MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< PrecisionMatrix >* insigma, const TypedDagNode< std::vector<double> >* inrootVal) : TypedDistribution< MultivariatePhyloProcess >( new MatrixReal(intau->getValue().getNumberOfNodes(), insigma->getValue().getDim(), 0.0 )),
MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< PrecisionMatrix >* insigma) : TypedDistribution< MultivariatePhyloProcess >( new MultivariatePhyloProcess(&intau->getValue(), insigma->getValue().getDim())),
tau( intau ),
sigma( insigma ) {
// rootVal( inrootVal ) {
    this->addParameter( tau );
    this->addParameter( sigma );
//    this->addParameter( rootVal );
    
    simulate();
}


MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &n): TypedDistribution< MultivariatePhyloProcess>( n ), tau( n.tau ), sigma( n.sigma ) /*, rootVal( n.rootVal )*/ {
    // nothing to do here since the parameters are copied automatically
    
}

MultivariateBrownianPhyloProcess* MultivariateBrownianPhyloProcess::clone(void) const {
    return new MultivariateBrownianPhyloProcess( *this );
}



double MultivariateBrownianPhyloProcess::computeLnProbability(void) {
    
    double lnProb = 0;
    if (sigma->getValue().isPositive()) {
        lnProb = recursiveLnProb(tau->getValue().getRoot());
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
        
        // x ~ normal(x_up, sigma^2 * branchLength)

        size_t upindex = from.getParent().getIndex();
        std::vector<double> upval = (*value)[upindex];
        /*
        if (from.getParent().isRoot())  {
            for (size_t j=0; j<getDim(); j++)   {
                upval[j] = 0;
            }
        }
        */
        
        const MatrixReal& om = sigma->getValue().getInverse();
        
        double s2 = 0;
        for (size_t i=0; i<getDim(); i++)   {
            double tmp = 0;
            for (size_t j=0; j<getDim(); j++)   {
                tmp += om[i][j] * (val[j] - upval[j]);
            }
            s2 += (val[i] - upval[i]) * tmp;
        }
        
        lnProb -= 0.5 * s2 / from.getBranchLength();
        lnProb -= 0.5 * (sigma->getValue().getLogDet() + sigma->getValue().getDim() * log(from.getBranchLength()));
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
}


void MultivariateBrownianPhyloProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void MultivariateBrownianPhyloProcess::recursiveSimulate(const TopologyNode& from)  {
    
    size_t index = from.getIndex();
    if (from.isRoot())    {
        
        std::vector<double>& val = (*value)[index];
        for (size_t i=0; i<getDim(); i++)   {
            val[i] = 0;
        }
    }
    
    else    {
        
        // x ~ normal(x_up, sigma^2 * branchLength)

        std::vector<double>& val = (*value)[index];
                
        sigma->getValue().drawNormalSampleCovariance((*value)[index]);

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


double MultivariateBrownianPhyloProcess::getMean(int k) {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, tau->getValue().getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return e1;
}

double MultivariateBrownianPhyloProcess::getStdev(int k) {
    
    int n = 0;
    double e1 = 0;
    double e2 = 0;
    recursiveGetStats(k, tau->getValue().getRoot(), e1, e2, n);
    e1 /= n;
    e2 /= n;
    e2 -= e1 * e1;
    return sqrt(e2);
}


void MultivariateBrownianPhyloProcess::recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n)  {

    double tmp = (*value)[from.getIndex()][k];

    n++;
    e1 += tmp;
    e2 += tmp * tmp;
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveGetStats(k,from.getChild(i),e1,e2,n);
    }
    
}


void MultivariateBrownianPhyloProcess::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<PrecisionMatrix> * >( newP );
    }
    /*
    if ( oldP == rootVal ) {
        rootVal = static_cast< const TypedDagNode< std::vector<double> > * >( newP );
    }
    */
}

