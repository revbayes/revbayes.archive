#include "ChisqDistribution.h"
#include "DistributionChisq.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;


ChisqDistribution::ChisqDistribution(const TypedDagNode<int> *df) : ContinuousDistribution( new double( 0.0 ) ),
    degrees( df )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( degrees );
    
    *value = RbStatistics::ChiSquare::rv(degrees->getValue(), *GLOBAL_RNG);
}


ChisqDistribution::~ChisqDistribution( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double ChisqDistribution::cdf( void ) const
{
    return RbStatistics::ChiSquare::cdf(degrees->getValue(), *value );
}


ChisqDistribution* ChisqDistribution::clone( void ) const
{
    return new ChisqDistribution( *this );
}


double ChisqDistribution::computeLnProbability( void )
{
    double v = *value;
    
    // check that the value is inside the boundaries
    if ( v < 0.0 )
    {
        return RbConstants::Double::neginf;
    }
    
    return RbStatistics::ChiSquare::lnPdf(degrees->getValue(), v);
}


double ChisqDistribution::getMax( void ) const
{
    return RbConstants::Double::inf;
}


double ChisqDistribution::getMin( void ) const
{
    return 0.0;
}


double ChisqDistribution::quantile(double p) const
{
    return RbStatistics::ChiSquare::quantile(p, degrees->getValue());
}


void ChisqDistribution::redrawValue( void )
{
    *value = RbStatistics::ChiSquare::rv(degrees->getValue(), *GLOBAL_RNG);
}


/** Swap a parameter of the distribution */
void ChisqDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == degrees)
    {
        degrees = static_cast<const TypedDagNode<int>* >( newP );
    }
    
}


