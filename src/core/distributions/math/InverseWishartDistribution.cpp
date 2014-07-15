/* 
 * File:   InverseWishartDistribution.cpp
 * Author: nl
 * 
 * Created on 15 juillet 2014, 10:08
 */

#include "InverseWishartDistribution.h"

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"
#include "DistributionInverseWishart.h"

using namespace RevBayesCore;

InverseWishartDistribution::InverseWishartDistribution(const TypedDagNode<PrecisionMatrix> *insigma0, const TypedDagNode<int>* indf)  :
TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(insigma0->getValue().getDim())),
sigma0(insigma0),
kappa(NULL),
df(indf),
dim(0)  {
    
    addParameter(sigma0);
    addParameter(df);

    redrawValue();
}

InverseWishartDistribution::InverseWishartDistribution(const TypedDagNode<int>* indim, const TypedDagNode<double> *inkappa, const TypedDagNode<int>* indf)  :
TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix( size_t(indim->getValue()) )),
    sigma0(NULL),
    kappa(inkappa),
    df(indf),
    dim(indim)    {
    
        addParameter(dim);
        addParameter(kappa);
        addParameter(df);
        
        redrawValue();
}

InverseWishartDistribution::InverseWishartDistribution(const InverseWishartDistribution& from) :
    TypedDistribution<RevBayesCore::PrecisionMatrix>(new PrecisionMatrix(from.getValue().getDim())),
    sigma0(from.sigma0),
    kappa(from.kappa),
    df(from.df),
    dim(from.dim)   {

        if (sigma0) {
            std::cerr << "sigma0??\n";
            exit(1);
            addParameter( sigma0 );
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

InverseWishartDistribution* InverseWishartDistribution::clone(void) const   {

    return new InverseWishartDistribution(*this);
}

void InverseWishartDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == sigma0) {
            std::cerr << "sigma0??\n";
            exit(1);
        sigma0 = static_cast<const TypedDagNode<PrecisionMatrix>* >( newP );
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


double InverseWishartDistribution::computeLnProbability(void)  {
    
    double ret = 0;
    
    if (sigma0) {    
        ret = RbStatistics::InverseWishart::lnPdf(sigma0->getValue(),df->getValue(),getValue());
    }
    else    {
        ret = RbStatistics::InverseWishart::lnPdf(kappa->getValue(),df->getValue(),getValue());        
    }

    return ret;
}

void InverseWishartDistribution::redrawValue(void)  {

    RandomNumberGenerator* rng = GLOBAL_RNG;

    if (sigma0) {
        getValue() = RbStatistics::InverseWishart::rv(sigma0->getValue(),df->getValue(), *rng);
    }
    else    {
        getValue() = RbStatistics::InverseWishart::rv(kappa->getValue(),getValue().getDim(),df->getValue(), *rng);        
    }

    // this will calculate the eigenvalues and eigenvectors
    getValue().update();


}
