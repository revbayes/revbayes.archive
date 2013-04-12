#include "ExponentialDistribution.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

ExponentialDistribution::ExponentialDistribution(const TypedDagNode<double> *l) : ContinuousDistribution( new double( 0.0 ) ), lambda( l ) {
    // add the lambda parameter as a parent
    addParameter( l );
    
    *value = RbStatistics::Exponential::rv(lambda->getValue(), *GLOBAL_RNG);
}


ExponentialDistribution::ExponentialDistribution(const ExponentialDistribution &n) : ContinuousDistribution( n ), lambda( n.lambda ) {
    // parameters are automatically copied
}


ExponentialDistribution::~ExponentialDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double ExponentialDistribution::cdf( void ) const {
    return RbStatistics::Exponential::cdf(lambda->getValue(), *value);
}


ExponentialDistribution* ExponentialDistribution::clone( void ) const {
    return new ExponentialDistribution( *this );
}


double ExponentialDistribution::computeLnProbability( void ) {
    return RbStatistics::Exponential::lnPdf(lambda->getValue(), *value);
}


double ExponentialDistribution::getMax( void ) const {
    return RbConstants::Double::inf;
}


double ExponentialDistribution::getMin( void ) const {
    return 0.0;
}


double ExponentialDistribution::quantile(double p) const {
    return RbStatistics::Exponential::quantile(lambda->getValue(), p);
}


void ExponentialDistribution::redrawValue( void ) {
    *value = RbStatistics::Exponential::rv(lambda->getValue(), *GLOBAL_RNG);
}


void ExponentialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
}


