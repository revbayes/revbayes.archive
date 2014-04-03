//
//  WishartDistribution.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-26.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "WishartDistribution.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"
#include "DistributionWishart.h"


using namespace RevBayesCore;

WishartDistribution::WishartDistribution(const TypedDagNode<PrecisionMatrix> *inomega0, const TypedDagNode<int>* indf)  :
TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(inomega0->getValue().getDim())),
dim(0),
omega0(inomega0),
kappa(NULL),
df(indf)    {
    
    addParameter( omega0 );
    addParameter(df);

    redrawValue();
}

WishartDistribution::WishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf)  :
TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(indim->getValue())),
    dim(indim),
    omega0(NULL),
    kappa(inkappa),
    df(indf)    {
    
        addParameter( kappa );
        addParameter(df);
        
        redrawValue();
}

WishartDistribution::WishartDistribution(const WishartDistribution& from) :
    TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(from.getValue().getDim())),
    dim(from.dim),
    omega0(from.omega0),
    kappa(from.kappa),
    df(from.df)    {

        if (omega0) {
            addParameter( omega0 );
        }
        if (kappa)  {
            addParameter(kappa);
        }
        addParameter(df);

        redrawValue();
}

WishartDistribution* WishartDistribution::clone(void) const   {

    return new WishartDistribution(*this);
}

void WishartDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == omega0) {
        omega0 = static_cast<const TypedDagNode<PrecisionMatrix>* >( newP );
    }
    if (oldP == kappa)  {
        kappa = static_cast<const TypedDagNode<double>* >(newP);
    }
}


double WishartDistribution::computeLnProbability(void)  {
    
    size_t dim = getValue().getDim();
    
    double ret = 0;
    ret -= 0.5 * getDF() * omega0->getValue().getLogDet();
    ret += 0.5 * (getDF() - dim - 1) * getValue().getLogDet();

    double trace = 0;
 
    if (omega0) {
        const MatrixReal& inv = omega0->getValue().getInverse();
        
        for (size_t i=0; i<dim; i++)   {
            for (size_t j=0; j<dim; j++)   {
                trace += inv[i][j] * getValue()[j][i];
            }
        }
    }
    else{
    
        for (size_t i=0; i<dim; i++)   {
            trace += getValue()[i][i];
        }
    }

    ret -= 0.5 * trace;
    
    return ret;
}

void WishartDistribution::drawNormalSample(std::vector<double>& tmp)    {

    size_t dim = getValue().getDim();

    if (omega0) {
        omega0->getValue().drawNormalSampleFromInverse(tmp);
    }
    else{
        // simulate the new Val
        RandomNumberGenerator* rng = GLOBAL_RNG;
        
        for (int i=0; i<dim; i++)  {
            tmp[i] = RbStatistics::Normal::rv(0, 1.0 / sqrt(kappa->getValue()), *rng);
        }
    }
}

void WishartDistribution::redrawValue(void)  {

    size_t dim = getValue().getDim();

    for (size_t i=0; i<dim; i++)   {
        for (size_t j=0; j<dim; j++)   {
            getValue()[i][j] = 0;
        }
    }
    
    std::vector<double> tmp(dim);
    for (size_t k=0; k<dim; k++)   {
        drawNormalSample(tmp);
        for (size_t i=0; i<dim; i++)   {
            for (size_t j=0; j<dim; j++)   {
                getValue()[i][j] += tmp[i] * tmp[j];
            }
        }
    }
}
