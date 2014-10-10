#include "DirichletDistribution.h"
#include "DistributionDirichlet.h"
#include "RandomNumberFactory.h"

using namespace RevBayesCore;

DirichletDistribution::DirichletDistribution(const TypedDagNode< RbVector<double> > *a) : TypedDistribution< RbVector<double> >( new RbVector<double>() ),
    alpha( a )
{
    
    *value = RbStatistics::Dirichlet::rv(alpha->getValue(), *GLOBAL_RNG);
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


/** Get the parameters of the distribution */
std::set<const DagNode*> DirichletDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( alpha );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void DirichletDistribution::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == alpha)
    {
        alpha = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}


