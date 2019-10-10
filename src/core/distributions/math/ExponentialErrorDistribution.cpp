/*
 * File:   ExponentialErrorDistribution.cpp
 * Author: David Cerny
 *
 * Created on October 8, 2019
 */

#include "ExponentialErrorDistribution.h"
#include "DistributionExponentialError.h"
#include "RandomNumberFactory.h"
#include "Cloneable.h"
#include "RbException.h"
#include "TypedFunction.h"
#include "TypedDagNode.h"

namespace RevBayesCore { class DagNode; } // deals with nodes of DAGs
namespace RevBayesCore { class RandomNumberGenerator; }

using namespace RevBayesCore;

ExponentialErrorDistribution::ExponentialErrorDistribution(const TypedDagNode<DistanceMatrix> *adm, const TypedDagNode<double>* l)  :
    TypedDistribution<DistanceMatrix>( new DistanceMatrix(adm->getValue().getSize()) ),
    avgDistMat( adm ),
    lambda( l )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( adm );
    addParameter( l );
        
    redrawValue();
}

ExponentialErrorDistribution* ExponentialErrorDistribution::clone(void) const   {

    return new ExponentialErrorDistribution(*this);
}

void ExponentialErrorDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == avgDistMat)
    {
        avgDistMat = static_cast<const TypedDagNode<DistanceMatrix>* >( newP );
    }
    
    if (oldP == lambda)
    {
        lambda = static_cast<const TypedDagNode<double>* >(newP);
    }
    
}

double ExponentialErrorDistribution::computeLnProbability(void)   {
    
    return (RbStatistics::ExponentialError::lnPdf(avgDistMat->getValue(), lambda->getValue(), getValue()));
}

void ExponentialErrorDistribution::redrawValue(void)   {

    RandomNumberGenerator* rng = GLOBAL_RNG;
    setValue( RbStatistics::ExponentialError::rv(avgDistMat->getValue(), lambda->getValue(), *rng).clone() );
}

