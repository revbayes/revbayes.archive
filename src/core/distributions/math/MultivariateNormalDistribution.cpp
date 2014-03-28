#include "MultivariateNormalDistribution.h"
#include "DistributionMultivariateNormal.h"

#include "RandomNumberFactory.h"

using namespace RevBayesCore;

MultivariateNormalDistribution::MultivariateNormalDistribution(const TypedDagNode< std::vector<double> > *inmean, const TypedDagNode<PrecisionMatrix>* inprec) : TypedDistribution< std::vector<double> >( new std::vector<double>() ), mean( inmean ), precision(inprec) {
    // add the lambda parameter as a parent
    addParameter( mean );
    addParameter( precision );
    
    redrawValue();
}


MultivariateNormalDistribution::MultivariateNormalDistribution(const MultivariateNormalDistribution &n) : TypedDistribution<std::vector<double> >( n ), mean( n.mean ), precision( n.precision) {

}


MultivariateNormalDistribution::~MultivariateNormalDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MultivariateNormalDistribution* MultivariateNormalDistribution::clone( void ) const {
    return new MultivariateNormalDistribution( *this );
}


double MultivariateNormalDistribution::computeLnProbability( void ) {
    
    return RbStatistics::MultivariateNormal::lnPdf(mean->getValue(), precision->getValue(), *value);
}


void MultivariateNormalDistribution::redrawValue( void ) {
    
    precision->getValue().drawNormalSample(getValue());
}


void MultivariateNormalDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == mean) {
        mean = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    if (oldP == precision) {
        precision = static_cast<const TypedDagNode<PrecisionMatrix >* >( newP );
    }
}


