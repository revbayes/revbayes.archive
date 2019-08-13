#include "ExperimentalCodonModelRateMatrixVectorFunction.h"
#include "RbException.h"

using namespace RevBayesCore;


ExperimentalCodonModelRateMatrixVectorFunction::ExperimentalCodonModelRateMatrixVectorFunction(const TypedDagNode<double> *b, const TypedDagNode<double> *o, const TypedDagNode<double> *k, const TypedDagNode< MatrixReal > *aap, const TypedDagNode< Simplex > *bf, const TypedDagNode< RbVector<long> > *s) : TypedFunction< RbVector<RateGenerator> >( new RbVector<RateGenerator>(aap->getValue().size(), RateMatrix_ExperimentalCodonModel()) ),
amino_acid_preferences( aap ),
base_frequencies( bf ),
beta( b ),
kappa( k ),
omega( o ),
site_assignments( s )
{
    // add the lambda parameter as a parent
    addParameter( amino_acid_preferences );
    addParameter( base_frequencies );
    addParameter( beta );
    addParameter( kappa );
    addParameter( omega );
    addParameter( site_assignments );

    update();
}


ExperimentalCodonModelRateMatrixVectorFunction::~ExperimentalCodonModelRateMatrixVectorFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



ExperimentalCodonModelRateMatrixVectorFunction* ExperimentalCodonModelRateMatrixVectorFunction::clone( void ) const
{

    return new ExperimentalCodonModelRateMatrixVectorFunction( *this );
}


void ExperimentalCodonModelRateMatrixVectorFunction::update( void )
{
    // get the information from the arguments for reading the file
    double b = beta->getValue();
    double o = omega->getValue();
    double k = kappa->getValue();
    const std::vector<double>& f = base_frequencies->getValue();
    const std::vector<long>& s = site_assignments->getValue();

    // std::vector<double> site_rates;
    double total_rate = 0.0;

    size_t num_matrices = amino_acid_preferences->getValue().size();

    for (size_t i=0; i<num_matrices; ++i)
    {
      const std::vector<double>& p = amino_acid_preferences->getValue()[i];
      // set the base frequencies
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->setAminoAcidPreferences( p );
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->setNucleotideFrequencies( f );
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->setBeta( b );
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->setKappa( k );
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->setOmega( o );

      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->update();

      int num_sites_with_matrix;
      for (size_t j=0; j<s.size(); ++j)
      {
        if (s[j])
        {
          ++num_sites_with_matrix;
        }
      }

      // site_rates.push_back((double)num_sites_with_matrix * static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->averageRate());
      total_rate += (double)num_sites_with_matrix * static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->averageRate();
    }

    for (size_t i=0; i<num_matrices; ++i)
    {
      static_cast< RateMatrix_ExperimentalCodonModel* >(&(*value)[i])->rescaleToOne(total_rate);
    }

}



void ExperimentalCodonModelRateMatrixVectorFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == amino_acid_preferences)
    {
        amino_acid_preferences = static_cast<const TypedDagNode< MatrixReal >* >( newP );
    }

    if (oldP == base_frequencies)
    {
        base_frequencies = static_cast<const TypedDagNode< Simplex >* >( newP );
    }

    if (oldP == beta)
    {
        beta = static_cast<const TypedDagNode< double >* >( newP );
    }

    if (oldP == kappa)
    {
        kappa = static_cast<const TypedDagNode< double >* >( newP );
    }

    if (oldP == omega)
    {
        omega = static_cast<const TypedDagNode< double >* >( newP );
    }

    if (oldP == site_assignments)
    {
        site_assignments = static_cast<const TypedDagNode< RbVector<long> >* >( newP );
    }

}
