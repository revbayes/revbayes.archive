
#include "NormalDistribution.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

NormalDistribution::NormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s) : ContinuousDistribution( new double( 0.0 ) ),
    mean( m ),
    stDev( s )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mean );
    addParameter( stDev );
    
    *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
}


NormalDistribution::~NormalDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double NormalDistribution::cdf( void ) const {
    return RbStatistics::Normal::cdf( mean->getValue(), stDev->getValue(), *value);
}


NormalDistribution* NormalDistribution::clone( void ) const {
    return new NormalDistribution( *this );
}


double NormalDistribution::computeLnProbability( void ) {
    return RbStatistics::Normal::lnPdf(mean->getValue(), stDev->getValue(), *value);
}


double NormalDistribution::getMax( void ) const {
    return RbConstants::Double::inf;
}


double NormalDistribution::getMin( void ) const {
    return RbConstants::Double::neginf;
}


double NormalDistribution::quantile(double p) const {
    return RbStatistics::Normal::quantile(mean->getValue(), stDev->getValue(), p);
}


void NormalDistribution::redrawValue( void ) {
    *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void NormalDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == mean)
    {
        mean = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == stDev)
    {
        stDev = static_cast<const TypedDagNode<double>* >( newP );
    }
}
