#include "OffsetExponentialDistribution.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

OffsetExponentialDistribution::OffsetExponentialDistribution(const TypedDagNode<double> *l, const TypedDagNode<double> *o) : ContinuousDistribution( new double( 0.0 ) ), lambda( l ), offset( o ) {
    // add the lambda parameter as a parent
    addParameter( lambda );
    addParameter( offset );
    
    *value = offset->getValue() + RbStatistics::Exponential::rv(lambda->getValue(), *GLOBAL_RNG);
}


OffsetExponentialDistribution::OffsetExponentialDistribution(const OffsetExponentialDistribution &n) : ContinuousDistribution( n ), lambda( n.lambda ), offset( n.offset ) {
    // parameters are automatically copied
}


OffsetExponentialDistribution::~OffsetExponentialDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


double OffsetExponentialDistribution::cdf( void ) const {
    return RbStatistics::Exponential::cdf(lambda->getValue(), *value - offset->getValue());
}


OffsetExponentialDistribution* OffsetExponentialDistribution::clone( void ) const {
    return new OffsetExponentialDistribution( *this );
}


double OffsetExponentialDistribution::computeLnProbability( void ) {
    
    double x = -(*value - offset->getValue());
    
    if ( x <= 0 ) return RbConstants::Double::neginf;
    
    return RbStatistics::Exponential::lnPdf(lambda->getValue(), x);
}


double OffsetExponentialDistribution::getMax( void ) const {
    return RbConstants::Double::inf;
}


double OffsetExponentialDistribution::getMin( void ) const {
    return offset->getValue();
}


double OffsetExponentialDistribution::quantile(double p) const {
    return RbStatistics::Exponential::quantile(lambda->getValue(), p) + offset->getValue();
}


void OffsetExponentialDistribution::redrawValue( void ) {
    *value = RbStatistics::Exponential::rv(lambda->getValue(), *GLOBAL_RNG) + offset->getValue();
}


void OffsetExponentialDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == offset) {
        offset = static_cast<const TypedDagNode<double>* >( newP );
    }
}


