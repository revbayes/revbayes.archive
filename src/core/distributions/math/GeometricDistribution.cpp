#include "GeometricDistribution.h"
#include "DistributionGeometric.h"
#include "RandomNumberFactory.h"

using namespace RevBayesCore;

GeometricDistribution::GeometricDistribution(const TypedDagNode<double> *q) : TypedDistribution<int>( new int( 1 ) ), p( q ) 
{
    *value = RbStatistics::Geometric::rv(p->getValue(), *GLOBAL_RNG);
}


GeometricDistribution::~GeometricDistribution( void ) 
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


GeometricDistribution* GeometricDistribution::clone( void ) const 
{
    
    return new GeometricDistribution( *this );
}


double GeometricDistribution::computeLnProbability( void ) 
{

    return RbStatistics::Geometric::lnPdf(*value, p->getValue());
}


void GeometricDistribution::redrawValue( void ) 
{

    *value = RbStatistics::Geometric::rv(p->getValue(), *GLOBAL_RNG);

}


/** Get the parameters of the distribution */
std::set<const DagNode*> GeometricDistribution::getParameters( void ) const
{
    std::set<const DagNode*> parameters;
    
    parameters.insert( p );
    
    parameters.erase( NULL );
    return parameters;
}


/** Swap a parameter of the distribution */
void GeometricDistribution::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == p) 
    {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }

}
