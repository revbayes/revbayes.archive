//
//  PhyloMultivariateBrownianProcess.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//


#include "PhyloMultivariateBrownianProcess.h"

#include "ConstantNode.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include "StochasticNode.h"

#include <iostream>

using namespace RevBayesCore;


// constructor(s)
PhyloMultivariateBrownianProcess::PhyloMultivariateBrownianProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< MatrixRealSymmetric >* insigma) : TypedDistribution< RbVector< RbVector<double> > >( new RbVector< RbVector<double> >(0,insigma->getValue().getDim())),
    tau( intau ),
    sigma( insigma ),
    dirtyNodes(intau->getValue().getNumberOfNodes(),true),
    nodeLogProbs(intau->getValue().getNumberOfNodes(),0)
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( tau );
    addParameter( sigma );
    
    simulate();
}


PhyloMultivariateBrownianProcess* PhyloMultivariateBrownianProcess::clone(void) const {
    return new PhyloMultivariateBrownianProcess( *this );
}



double PhyloMultivariateBrownianProcess::computeLnProbability(void) {
    
    double lnProb = 0;
    if (sigma->getValue().isPositive()) {
        lnProb = recursiveLnProb(tau->getValue().getRoot());
    }
    else{
        lnProb = RbConstants::Double::neginf;
    }

    // std::cerr << "brownian log prob  :" << lnProb - bklnProb << '\n';
    // bklnProb = lnProb;
    
    return lnProb;
}


double PhyloMultivariateBrownianProcess::recursiveLnProb( const TopologyNode& from ) {
    
    double lnProb = 0.0;
    size_t index = from.getIndex();
    std::vector<double> val = (*value)[index];
    
    if (! from.isRoot()) {
        
//        if (1)  {
        if (dirtyNodes[index])  {

            // x ~ normal(x_up, sigma^2 * branchLength)

            size_t upindex = from.getParent().getIndex();
            std::vector<double> upval = (*value)[upindex];

            const MatrixReal& om = sigma->getValue().getInverse();

            double s2 = 0;
            for (size_t i = 0; i < getDim(); i++) {
                double tmp = 0;
                for (size_t j = 0; j < getDim(); j++) {
                    tmp += om[i][j] * (val[j] - upval[j]);
                }
                s2 += (val[i] - upval[i]) * tmp;
            }

            double logprob = 0;
            logprob -= 0.5 * s2 / from.getBranchLength();
            logprob -= 0.5 * (sigma->getValue().getLogDet() + sigma->getValue().getDim() * log(from.getBranchLength()));
            nodeLogProbs[index] = logprob;
            dirtyNodes[index] = false;
        }
        lnProb += nodeLogProbs[index];
    }
    
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        lnProb += recursiveLnProb(from.getChild(i));
    }
    
    return lnProb;
    
}

void PhyloMultivariateBrownianProcess::redrawValue(void) {
    simulate();
    corruptAll();
}

void PhyloMultivariateBrownianProcess::simulate() {
    
    recursiveSimulate(tau->getValue().getRoot());
}


void PhyloMultivariateBrownianProcess::recursiveSimulate(const TopologyNode& from)  {
    
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


/** Swap a parameter of the distribution */
void PhyloMultivariateBrownianProcess::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if ( oldP == tau )
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<MatrixRealSymmetric> * >( newP );
    }
}

void PhyloMultivariateBrownianProcess::corruptAll()
{
    recursiveCorruptAll(tau->getValue().getRoot());
}

void PhyloMultivariateBrownianProcess::recursiveCorruptAll(const TopologyNode& from)
{
    
    dirtyNodes[from.getIndex()] = true;
    for (size_t i = 0; i < from.getNumberOfChildren(); ++i)
    {
        recursiveCorruptAll(from.getChild(i));
    }    
}

void PhyloMultivariateBrownianProcess::flagNodes()
{

    // the value at some of the nodes has changed
    // flag them as well as their immediate children
    // only those nodes will recompute their probability

    const std::set<size_t> &indices = dagNode->getTouchedElementIndices();

    // flag recomputation only for the nodes
    for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) {
        dirtyNodes[*it] = true;
        const TimeTree& tau = *getTimeTree();
        const TopologyNode& from = tau.getNode(*it);
        for (size_t i = 0; i < from.getNumberOfChildren(); ++i) {
            dirtyNodes[from.getChild(i).getIndex()] = true;
        }
    }
}

void PhyloMultivariateBrownianProcess::touchSpecialization( DagNode *toucher ) {
    
    if ( toucher == sigma ) {
        // should recompute the probability of all nodes
        corruptAll();
    }
    else if (toucher == tau)    {
        // if only branch lengths have changed, something could be done here
        // but for the moment, recompute the probability of all nodes
        corruptAll();        
    } 
    else {
        // the value at some of the nodes has changed
        // flag them as well as their immediate children
        flagNodes();
    }
    dagNode->clearTouchedElementIndices();    
}


void PhyloMultivariateBrownianProcess::keepSpecialization( DagNode* affecter ) {
}


void PhyloMultivariateBrownianProcess::restoreSpecialization( DagNode *restorer ) {
    if ( restorer == sigma ) {
        corruptAll();
    }
    else if (restorer == tau)    {
        // if only branch lengths have changed, something could be done here
        // but leave it for the moment
        corruptAll();        
    } 
    else {
        flagNodes();
    }
    dagNode->clearTouchedElementIndices();    
}
