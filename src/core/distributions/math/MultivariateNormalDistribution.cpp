#include "MultivariateNormalDistribution.h"
#include "DistributionMultivariateNormal.h"
#include "RandomNumberFactory.h"
#include "RbConstants.h"
#include "RbException.h"
#include "StochasticNode.h"

#include <cmath>

using namespace RevBayesCore;

MultivariateNormalDistribution::MultivariateNormalDistribution(const TypedDagNode< RbVector<double> > *m, const TypedDagNode<MatrixReal>* cov, const TypedDagNode<MatrixReal>* prec, const TypedDagNode<double>* sc) :
    TypedDistribution< RbVector<double> >( new RbVector<double>() ),
    mean( m ),
    covariance( cov ),
    precision( prec),
    scale( sc ),
    observed(mean->getValue().size(),false),
    observations(mean->getValue().size(),0.0),
    active_matrix(0),
    inverse_covariance_matrices( std::vector<MatrixReal>( 2, MatrixReal(mean->getValue().size()) ) )
{
    // make sure that only either the covariance or the precision matrix are set
    if ( covariance == NULL && precision == NULL )
    {
        throw RbException("You need to provide a covariance matrix OR a precision matrix");
    }
    else if ( covariance != NULL && precision != NULL )
    {
        throw RbException("You can only provide a covariance matrix OR a precision matrix");
    }
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( mean );
    addParameter( covariance );
    addParameter( precision );
    addParameter( scale );
    
    // if we're using a covariance matrix, initialize the
    // inverse of the covariance matrix
    if ( covariance != NULL )
    {
        inverse_covariance_matrices[0] = covariance->getValue().computeInverse();
        inverse_covariance_matrices[0].setCholesky(true);
        inverse_covariance_matrices[1] = covariance->getValue().computeInverse();
        inverse_covariance_matrices[1].setCholesky(true);
    }
    
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


void MultivariateNormalDistribution::clampAt(size_t i, double v)
{
    (*value)[i] = v;
    observations[i] = v;
    observed[i] = true;
    
    // set the flag the this node is clamped
    this->dag_node->setClamped( true );
}


MatrixReal MultivariateNormalDistribution::computeContrasts( void )
{
    size_t dim = value->size();
    std::vector<double> tmp = std::vector<double>(dim, 0.0);
    
    const RbVector<double> &m = mean->getValue();
    for (size_t i = 0; i < dim; ++i)
    {
        tmp[i] = ((*value)[i] - m[i]) / sqrt(scale->getValue());
    }
    
    MatrixReal c = MatrixReal( dim );
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            c[i][j] = tmp[i] * tmp[j];
        }
    }
    
    return c;
}


double MultivariateNormalDistribution::computeLnProbability( void )
{
    size_t dim = observed.size();
    bool matchesObservations = true;
    for (size_t i = 0; i < dim; ++i)
    {
        matchesObservations &= ( !observed[i] || (*value)[i] == observations[i]);
    }
    if ( matchesObservations == false )
    {
        return RbConstants::Double::neginf;
    }
    
    if ( covariance != NULL )
    {
        return RbStatistics::MultivariateNormal::lnPdfPrecision(mean->getValue(), inverse_covariance_matrices[active_matrix], *value, scale->getValue() );
    }
    else
    {
        return RbStatistics::MultivariateNormal::lnPdfPrecision(mean->getValue(), precision->getValue(), *value, scale->getValue() );
    }
}


RevLanguage::RevPtr<RevLanguage::RevVariable> MultivariateNormalDistribution::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
{
    
    if (name == "clampAt")
    {
        found = true;
        
        int    index = (int)static_cast<const TypedDagNode<long>   *>( args[0] )->getValue() - 1;
        double v     = static_cast<const TypedDagNode<double>*>( args[1] )->getValue();
        
        clampAt(index, v);

        return NULL;
    }
    
    return TypedDistribution<RbVector<double> >::executeProcedure( name, args, found );

    
}


void MultivariateNormalDistribution::redrawValue( void )
{
    
    RandomNumberGenerator *rng = GLOBAL_RNG;

    if ( covariance != NULL )
    {
        *value = RbStatistics::MultivariateNormal::rvPrecision( mean->getValue(), inverse_covariance_matrices[active_matrix], *rng, scale->getValue() );
    }
    else
    {
        *value = RbStatistics::MultivariateNormal::rvPrecision( mean->getValue(), precision->getValue(), *rng, scale->getValue() );
    }
    
    size_t dim = observed.size();
    for (size_t i = 0; i < dim; ++i)
    {
        if (observed[i] == true )
        {
            (*value)[i] = observations[i];
        }
    }
    
}

void MultivariateNormalDistribution::restoreSpecialization(DagNode *affecter)
{
    
    // reset the precision matrix if necessary
    if ( affecter == covariance )
    {
        active_matrix = (active_matrix == 0 ? 1 : 0);
    }

}

void MultivariateNormalDistribution::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == covariance )
    {
        // compute the inverse variance-covariance matrix (the precision matrix)
        active_matrix = (active_matrix == 0 ? 1 : 0);
        inverse_covariance_matrices[active_matrix] = covariance->getValue().computeInverse();
        inverse_covariance_matrices[active_matrix].setCholesky(true);
    }
    
}


/** Swap a parameter of the distribution */
void MultivariateNormalDistribution::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == mean)
    {
        mean = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    if (oldP == covariance)
    {
        covariance = static_cast<const TypedDagNode<MatrixReal >* >( newP );
        active_matrix = 0;
        inverse_covariance_matrices[0] = covariance->getValue().computeInverse();
        inverse_covariance_matrices[0].setCholesky(true);
        inverse_covariance_matrices[1] = covariance->getValue().computeInverse();
        inverse_covariance_matrices[1].setCholesky(true);
    }
    if (oldP == precision)
    {
        precision = static_cast<const TypedDagNode<MatrixReal >* >( newP );
    }
    if (oldP == scale)
    {
        scale = static_cast<const TypedDagNode< double >* >( newP );
    }
    
}


