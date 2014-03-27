#include "MultivariateNormalDistribution.h"
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
    
    const PrecisionMatrix& om = precision->getValue();

    double s2 = 0;
    for (size_t i=0; i<getValue().size(); i++)   {
        double tmp = 0;
        for (size_t j=0; j<getValue().size(); j++)   {
            tmp += om[i][j] * (getValue()[j] - mean->getValue()[j]);
        }
        s2 += (getValue()[i] - mean->getValue()[i]) * tmp;
    }
    
    double lnProb = 0.5 * log(precision->getValue().getLogDet()) - 0.5 * s2;
    
    return lnProb;
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


