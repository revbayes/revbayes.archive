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
omega0(inomega0),
kappa(NULL),
df(indf),
dim(0)  {
    
    addParameter(omega0);
    addParameter(df);

    redrawValue();
}

WishartDistribution::WishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf)  :
TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix( size_t(indim->getValue()) )),
    omega0(NULL),
    kappa(inkappa),
    df(indf),
    dim(indim)    {
    
        addParameter(dim);
        addParameter(kappa);
        addParameter(df);
        
        redrawValue();
}

WishartDistribution::WishartDistribution(const WishartDistribution& from) :
    TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(from.getValue().getDim())),
    omega0(from.omega0),
    kappa(from.kappa),
    df(from.df),
    dim(from.dim)   {

        if (omega0) {
            std::cerr << "omega0??\n";
            exit(1);
            addParameter( omega0 );
        }
        if (kappa)  {
            addParameter(kappa);
        }
        if (dim)    {
            addParameter(dim);
        }
        addParameter(df);

        redrawValue();
}

WishartDistribution* WishartDistribution::clone(void) const   {

    return new WishartDistribution(*this);
}

void WishartDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == omega0) {
            std::cerr << "omega0??\n";
            exit(1);
        omega0 = static_cast<const TypedDagNode<PrecisionMatrix>* >( newP );
    }
    if (oldP == kappa)  {
        kappa = static_cast<const TypedDagNode<double>* >(newP);
    }
    if (oldP == dim)    {
        dim = static_cast<const TypedDagNode<int>* >(newP);
    }
    if (oldP == df)    {
        df = static_cast<const TypedDagNode<int>* >(newP);
    }
}


double WishartDistribution::computeLnProbability(void)  {
    
    double ret = 0;
    
    if (omega0) {    
        ret = RbStatistics::Wishart::lnPdf(omega0->getValue(),df->getValue(),getValue());
    }
    else    {
        ret = RbStatistics::Wishart::lnPdf(kappa->getValue(),df->getValue(),getValue());        
    }

    return ret;
}

void WishartDistribution::redrawValue(void)  {

    RandomNumberGenerator* rng = GLOBAL_RNG;

    if (omega0) {
        getValue() = RbStatistics::Wishart::rv(omega0->getValue(),df->getValue(), *rng);
    }
    else    {
        getValue() = RbStatistics::Wishart::rv(kappa->getValue(),getValue().getDim(),df->getValue(), *rng);        
    }

    // this will calculate the eigenvalues and eigenvectors
    getValue().update();


}
