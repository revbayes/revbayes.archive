
#include "NormalDistribution.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

NormalDistribution::NormalDistribution(const TypedDagNode<double> *m, const TypedDagNode<double> *s) : ContinuousDistribution( new double( 0.0 ) ), mean( m ), stDev( s ) {
    // add the parameters to the parents list
    addParameter( mean );
    addParameter( stDev );
    
    *value = RbStatistics::Normal::rv(mean->getValue(), stDev->getValue(), *GLOBAL_RNG);
}


NormalDistribution::NormalDistribution(const NormalDistribution &n) : ContinuousDistribution( n ), mean( n.mean ), stDev( n.stDev ) {
    // parameters are automatically copied
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


void NormalDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == mean) {
        mean = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == stDev) {
        stDev = static_cast<const TypedDagNode<double>* >( newP );
    }
}
