
#include "FiellerHinkleyDistribution.h"
#include "DistributionFiellerHinkley.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

FiellerHinkleyDistribution::FiellerHinkleyDistribution(const TypedDagNode<double> *m1, const TypedDagNode<double> *s1, const TypedDagNode<double> *m2, const TypedDagNode<double> *s2, const TypedDagNode<double> *r) : ContinuousDistribution( new double( 0.0 ) ),
    mu1( m1 ),
    sd1( s1 ),
    mu2( m2 ),
    sd2( s2 ),
    rho( r )

{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mu1 );
    addParameter( sd1 );
    addParameter( mu2 );
    addParameter( sd2 );
    addParameter( rho );
    
    *value = RbStatistics::FiellerHinkley::rv(mu1->getValue(), sd1->getValue(), mu2->getValue(), sd2->getValue(), rho->getValue(), *GLOBAL_RNG);
}


FiellerHinkleyDistribution::~FiellerHinkleyDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}

double FiellerHinkleyDistribution::cdf( void ) const {
    
    return RbStatistics::FiellerHinkley::cdf(mu1->getValue(), sd1->getValue(), mu2->getValue(), sd2->getValue(), rho->getValue(), *value);
}

double FiellerHinkleyDistribution::quantile( double p ) const {
    
    return RbStatistics::FiellerHinkley::quantile(mu1->getValue(), sd1->getValue(), mu2->getValue(), sd2->getValue(), rho->getValue(), *value);
}

double FiellerHinkleyDistribution::getMax( void ) const {
    
    return RbConstants::Double::inf;
}

double FiellerHinkleyDistribution::getMin( void ) const {
    
    return RbConstants::Double::neginf;
}

FiellerHinkleyDistribution* FiellerHinkleyDistribution::clone( void ) const {
    return new FiellerHinkleyDistribution( *this );
}


double FiellerHinkleyDistribution::computeLnProbability( void ) {
    
    return RbStatistics::FiellerHinkley::lnPdf(mu1->getValue(), sd1->getValue(), mu2->getValue(), sd2->getValue(), rho->getValue(), *value);
}


void FiellerHinkleyDistribution::redrawValue( void ) {
    *value = RbStatistics::FiellerHinkley::rv(mu1->getValue(), sd1->getValue(), mu2->getValue(), sd2->getValue(), rho->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void FiellerHinkleyDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == mu1)
    {
        mu1 = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == sd1)
    {
        sd1 = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == mu2)
    {
        mu2 = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == sd2)
    {
        sd2 = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == rho)
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }

}
