#include "OneOverXDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>

using namespace RevBayesCore;

OneOverXDistribution::OneOverXDistribution(const TypedDagNode<double> *mi, const TypedDagNode<double> *ma) : ContinuousDistribution( new double( 1.0 ) ), min( mi ), max( ma ) {
    // add the parameters to the parents set
    addParameter( min );
    addParameter( max );
    
    double u = GLOBAL_RNG->uniform01();
    double ln_a = log( min->getValue() );
    *value = exp( u*(log( max->getValue() ) - ln_a) + ln_a);
}


OneOverXDistribution::OneOverXDistribution(const OneOverXDistribution &n) : ContinuousDistribution( n ), min( n.min ), max( n.max ) {
    // parameters are automatically copied
}


OneOverXDistribution::~OneOverXDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double OneOverXDistribution::cdf( void ) const {
    return 0.0;
}


OneOverXDistribution* OneOverXDistribution::clone( void ) const {
    return new OneOverXDistribution( *this );
}


double OneOverXDistribution::computeLnProbability( void ) 
{
    
    double v = *value; 
    double mi = min->getValue();
    double ma = max->getValue();
    if ( v >= mi && v <= ma ) 
    {
       return 1.0 / ( (log(max->getValue()) - log( min->getValue() )) * (*value) ); 
    } 
    else 
    {
        return RbConstants::Double::neginf;
    }
    
    
}


double OneOverXDistribution::getMax( void ) const 
{

    return max->getValue();
}


double OneOverXDistribution::getMin( void ) const 
{

    return min->getValue();
}


double OneOverXDistribution::quantile(double p) const 
{

    return 0.0;
}


void OneOverXDistribution::redrawValue( void ) 
{

    double u = GLOBAL_RNG->uniform01();
    double ln_a = log( min->getValue() );
    *value = exp( u*(log( max->getValue() ) - ln_a) + ln_a);

}


void OneOverXDistribution::swapParameter(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == min) 
    {
        min = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == max) 
    {
        max = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}
