#include "MultivariateNormalDistribution.h"
#include "DistributionMultivariateNormal.h"

#include "RandomNumberFactory.h"

using namespace RevBayesCore;

MultivariateNormalDistribution::MultivariateNormalDistribution(const TypedDagNode< RbVector<double> > *inmean, const TypedDagNode<MatrixRealSymmetric>* inprec) :
    TypedDistribution< RbVector<double> >( new RbVector<double>() ),
    mean( inmean ),
    precision(inprec)
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mean );
    addParameter( precision );
    
    redrawValue();
}


MultivariateNormalDistribution::~MultivariateNormalDistribution( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



MultivariateNormalDistribution* MultivariateNormalDistribution::clone( void ) const
{
    return new MultivariateNormalDistribution( *this );
}


double MultivariateNormalDistribution::computeLnProbability( void )
{
    
    return RbStatistics::MultivariateNormal::lnPdfPrecision(mean->getValue(), precision->getValue(), *value);
}


void MultivariateNormalDistribution::redrawValue( void )
{
    
    precision->getValue().drawNormalSamplePrecision(getValue());
}

/** Swap a parameter of the distribution */
void MultivariateNormalDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == mean)
    {
        mean = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    if (oldP == precision)
    {
        precision = static_cast<const TypedDagNode<MatrixRealSymmetric >* >( newP );
    }
}


