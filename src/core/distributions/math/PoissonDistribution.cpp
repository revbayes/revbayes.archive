#include "PoissonDistribution.h"
#include "DistributionPoisson.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"

using namespace RevBayesCore;

PoissonDistribution::PoissonDistribution(const TypedDagNode<double> *l) : TypedDistribution<int>( new int( 1 ) ), lambda( l ) {
    // add the parameters to the parents set
    addParameter( lambda );
    
    *value = RbStatistics::Poisson::rv(lambda->getValue(), *GLOBAL_RNG);
}


PoissonDistribution::~PoissonDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


PoissonDistribution* PoissonDistribution::clone( void ) const {
    return new PoissonDistribution( *this );
}


double PoissonDistribution::computeLnProbability( void ) {
    return RbStatistics::Poisson::lnPdf(lambda->getValue(), *value);
}


void PoissonDistribution::redrawValue( void ) {
    *value = RbStatistics::Poisson::rv(lambda->getValue(), *GLOBAL_RNG);
}


void PoissonDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
}
