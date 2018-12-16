#include "BivariatePoissonDistribution.h"
#include "DistributionBivariatePoisson.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"
#include "RbVector.h"

#include <cmath>
#include <vector>

using namespace RevBayesCore;

BivariatePoissonDistribution::BivariatePoissonDistribution(const TypedDagNode<double> *lambda0, const TypedDagNode<double> *lambda1, const TypedDagNode<double> *lambda2 ) :
  TypedDistribution< RbVector<int> >( new RbVector<int>() ),
  theta0( lambda0 ),
  theta1( lambda1 ),
  theta2( lambda2 )
{

    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( theta0 );
    addParameter( theta1 );
    addParameter( theta2 );


    *value = RbStatistics::BivariatePoisson::rv(theta0->getValue(), theta1->getValue(), theta2->getValue(), *GLOBAL_RNG);
}


BivariatePoissonDistribution::~BivariatePoissonDistribution( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BivariatePoissonDistribution* BivariatePoissonDistribution::clone( void ) const
{
  return new BivariatePoissonDistribution( *this );
}





double BivariatePoissonDistribution::computeLnProbability( void )
{
  return RbStatistics::BivariatePoisson::pdf(*value, theta0->getValue(), theta1->getValue(), theta2->getValue(), true);
}




void BivariatePoissonDistribution::redrawValue( void )
{
  *value = RbStatistics::BivariatePoisson::rv( theta0->getValue(), theta1->getValue(), theta2->getValue(), *GLOBAL_RNG );
}

/** Swap a parameter of the distribution */
void BivariatePoissonDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == theta0)
      {
        theta0 = static_cast<const TypedDagNode< double >* >( newP );
      }
    if (oldP == theta1)
      {
        theta1 = static_cast<const TypedDagNode< double >* >( newP );
      }
    if (oldP == theta2)
      {
        theta2 = static_cast<const TypedDagNode< double >* >( newP );
      }

}
