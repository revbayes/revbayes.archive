#include "DirichletDistribution.h"
#include "DistributionDirichlet.h"
#include "RandomNumberFactory.h"

using namespace RevBayesCore;

DirichletDistribution::DirichletDistribution(const TypedDagNode< std::vector<double> > *a) : TypedDistribution< std::vector<double> >( new std::vector<double>() ), alpha( a ) {
    // add the lambda parameter as a parent
    addParameter( a );
    
    *value = RbStatistics::Dirichlet::rv(alpha->getValue(), *GLOBAL_RNG);
}


DirichletDistribution::DirichletDistribution(const DirichletDistribution &n) : TypedDistribution<std::vector<double> >( n ), alpha( n.alpha ) {
    // parameters are automatically copied
}


DirichletDistribution::~DirichletDistribution( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



DirichletDistribution* DirichletDistribution::clone( void ) const {
    return new DirichletDistribution( *this );
}


double DirichletDistribution::computeLnProbability( void ) {
    return RbStatistics::Dirichlet::lnPdf(alpha->getValue(), *value);
}


void DirichletDistribution::redrawValue( void ) {
    *value = RbStatistics::Dirichlet::rv(alpha->getValue(), *GLOBAL_RNG);
}


void DirichletDistribution::swapParameter(const DagNode *oldP, const DagNode *newP) {
    if (oldP == alpha) {
        alpha = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}


