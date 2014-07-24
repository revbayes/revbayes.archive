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
MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const TypedDagNode< TimeTree > *intau, const TypedDagNode< PrecisionMatrix >* insigma) : TypedDistribution< MultivariatePhyloProcess >( new MultivariatePhyloProcess(&intau->getValue(), insigma->getValue().getDim())),
    tau( intau ),
    sigma( insigma ),
    dirtyNodes(intau->getValue().getNumberOfNodes(),true),
    nodeLogProbs(intau->getValue().getNumberOfNodes(),0)
{
    
    simulate();
}


MultivariateBrownianPhyloProcess::MultivariateBrownianPhyloProcess(const MultivariateBrownianPhyloProcess &n): TypedDistribution< MultivariatePhyloProcess>( n ), 
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

void MultivariateBrownianPhyloProcess::corruptAll() {
    recursiveCorruptNodes(tau->getValue().getRoot());
}

void MultivariateBrownianPhyloProcess::recursiveCorruptNodes(const TopologyNode& from)    {
    
    dirtyNodes[from.getIndex()] = true;
    // propagate forward
    size_t numChildren = from.getNumberOfChildren();
    
    for (size_t i = 0; i < numChildren; ++i) {
        recursiveCorruptNodes(from.getChild(i));
    }    
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
    }
    
    if ( oldP == sigma ) {
        sigma = static_cast< const TypedDagNode<PrecisionMatrix> * >( newP );
    }
}


void MultivariateBrownianPhyloProcess::getAffected(std::set<DagNode *> &affected, DagNode* affecter) {
    if ( affecter == sigma )    {
        //this->dagNode->getAffectedNodes( affected );
        corruptAll();
    }
    else    {
        std::cerr << "affected by sth else\n";
        exit(1);
    }
}

void MultivariateBrownianPhyloProcess::touchSpecialization( DagNode *toucher ) {
    // if the root rate has changed, then we need to change the corresponding value in our vector and downpass the touch
    if ( toucher == sigma ) {
        
        corruptAll();
        // delegate a touch to our children
        this->dagNode->touchAffected();
    }
    else    {
        
        const std::set<size_t> &indices = this->dagNode->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 ) 
        {
            // just delegate the call
            // ???
        } 
        else 
        {
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) 
            {
                dirtyNodes[*it] = true;
            } 
        }
    }
}


void MultivariateBrownianPhyloProcess::keepSpecialization( DagNode* affecter ) {
    // only do this when the toucher was the root rate
    /*
    if ( affecter == sigma ) {
        
        corruptAll();
        this->dagNode->keepAffected();
    }
    else {
//        std::cerr << "keep spec by sth else\n";
//        exit(1);
    }
    */
}


void MultivariateBrownianPhyloProcess::restoreSpecialization( DagNode *restorer ) {
    // only do this when the toucher was our parameters
    if ( restorer == sigma ) {
        
        corruptAll();
        // delegate a restore to our children
        // this->dagNode->restoreAffected();
    }
    else {
//        std::cerr << "restore spec by sth else\n";
//        exit(1);
        const std::set<size_t> &indices = this->dagNode->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 ) 
        {
            // just delegate the call
            // ???
        } 
        else 
        {
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) 
            {
                dirtyNodes[*it] = true;
            } 
        }
    }
}

