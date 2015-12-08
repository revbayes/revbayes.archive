
#include "TruncatedNormalDistribution.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

TruncatedNormalDistribution::TruncatedNormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s, const TypedDagNode<double> *mi, const TypedDagNode<double> *ma) : ContinuousDistribution( new double( 0.0 ) ),
    mean( m ),
    stDev( s ),
    min( mi ),
    max( ma )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mean );
    addParameter( stDev );
    addParameter( min );
    addParameter( max );
    
    if ( min->getValue() >= max->getValue() )
    {
        throw RbException("The minimum value must be smaller then the maximum value for the truncated normal distribution.");
    }
    
    do {
        *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
    } while ( *value < min->getValue() && *value > max->getValue() );
    
}


TruncatedNormalDistribution::~TruncatedNormalDistribution( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double TruncatedNormalDistribution::cdf( void ) const
{
    return RbStatistics::Normal::cdf( mean->getValue(), stDev->getValue(), *value);
}


TruncatedNormalDistribution* TruncatedNormalDistribution::clone( void ) const
{
    return new TruncatedNormalDistribution( *this );
}


double TruncatedNormalDistribution::computeLnProbability( void )
{
    
    if ( min->getValue() > *value )
    {
        return RbConstants::Double::neginf;
    }
    
    if ( max->getValue() < *value )
    {
        return RbConstants::Double::neginf;
    }
    
    return RbStatistics::Normal::lnPdf(mean->getValue(), stDev->getValue(), *value);
}


double TruncatedNormalDistribution::getMax( void ) const
{
    return max->getValue();
}


double TruncatedNormalDistribution::getMin( void ) const
{
    return min->getValue();
}


double TruncatedNormalDistribution::quantile(double p) const
{
    return RbStatistics::Normal::quantile(mean->getValue(), stDev->getValue(), p);
}


void TruncatedNormalDistribution::redrawValue( void )
{
    
    if ( min->getValue() >= max->getValue() )
    {
        throw RbException("The minimum value must be smaller then the maximum value for the truncated normal distribution.");
    }
    
    do {
        *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
    } while ( *value < min->getValue() && *value > max->getValue() );
    
}


/** Swap a parameter of the distribution */
void TruncatedNormalDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == mean)
    {
        mean = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == stDev)
    {
        stDev = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == min)
    {
        min = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == max)
    {
        max = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}
