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
MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< MatrixRealSymmetric >* insigma) : TypedDistribution< MultivariateRealNodeContainer >( new MultivariateRealNodeContainer(&intau->getValue(), insigma->getValue().getDim())),
    tau( intau ),
    sigma( insigma ),
    dirtyNodes(intau->getValue().getNumberOfNodes(),true),
    nodeLogProbs(intau->getValue().getNumberOfNodes(),0)
{
    
    simulate();
}


MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &n): TypedDistribution< MultivariateRealNodeContainer>( n ), 
    tau( n.tau ),
    sigma( n.sigma ),
    /*, rootVal( n.rootVal )*/ 
    dirtyNodes(n.tau->getValue().getNumberOfNodes(),true),
    nodeLogProbs(n.tau->getValue().getNumberOfNodes(),0)
{
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

    // std::cerr << "brownian log prob  :" << lnProb - bklnProb << '\n';
    // bklnProb = lnProb;
    
    return lnProb;
}


double MultivariateBrownianPhyloProcess::recursiveLnProb( const TopologyNode& from ) {
    
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

void MultivariateBrownianPhyloProcess::redrawValue(void) {
    simulate();
    corruptAll();
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


/** Get the parameters of the distribution */
std::set<const DagNode*> MultivariateBrownianPhyloProcess::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( tau );
    parameters.insert( sigma );
    
    return parameters;
}


/** Swap a parameter of the distribution */
void MultivariateBrownianPhyloProcess::swapParameter( const DagNode *oldP, const DagNode *newP )
{
    if ( oldP == tau )
    {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
        getValue().setTimeTree(&tau->getValue());
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<MatrixRealSymmetric> * >( newP );
    }
}

void MultivariateBrownianPhyloProcess::corruptAll() {
    recursiveCorruptAll(tau->getValue().getRoot());
}

void MultivariateBrownianPhyloProcess::recursiveCorruptAll(const TopologyNode& from)    {
    
    dirtyNodes[from.getIndex()] = true;
    for (size_t i = 0; i < from.getNumberOfChildren(); ++i) {
        recursiveCorruptAll(from.getChild(i));
    }    
}

void MultivariateBrownianPhyloProcess::flagNodes() {

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

void MultivariateBrownianPhyloProcess::touchSpecialization( DagNode *toucher ) {
    
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


void MultivariateBrownianPhyloProcess::keepSpecialization( DagNode* affecter ) {
}


void MultivariateBrownianPhyloProcess::restoreSpecialization( DagNode *restorer ) {
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
