#include "ExperimentalCodonModelRateMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;


ExperimentalCodonModelRateMatrixFunction::ExperimentalCodonModelRateMatrixFunction(const TypedDagNode<double> *b, const TypedDagNode<double> *o, const TypedDagNode<double> *k, const TypedDagNode< Simplex > *aap, const TypedDagNode< Simplex > *bf) : TypedFunction<RateGenerator>( new RateMatrix_ExperimentalCodonModel() ),
amino_acid_preferences( aap ),
base_frequencies( bf ),
beta( b ),
kappa( k ),
omega( o )
{
    // add the lambda parameter as a parent
    addParameter( amino_acid_preferences );
    addParameter( base_frequencies );
    addParameter( beta );
    addParameter( kappa );
    addParameter( omega );

    update();
}


ExperimentalCodonModelRateMatrixFunction::~ExperimentalCodonModelRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



ExperimentalCodonModelRateMatrixFunction* ExperimentalCodonModelRateMatrixFunction::clone( void ) const
{

    return new ExperimentalCodonModelRateMatrixFunction( *this );
}


void ExperimentalCodonModelRateMatrixFunction::update( void )
{
    // get the information from the arguments for reading the file
    double b = beta->getValue();
    double o = omega->getValue();
    double k = kappa->getValue();
    const std::vector<double>& p = amino_acid_preferences->getValue();
    const std::vector<double>& f = base_frequencies->getValue();


    // set the base frequencies
    static_cast< RateMatrix_ExperimentalCodonModel* >(value)->setAminoAcidPreferences( p );
    static_cast< RateMatrix_ExperimentalCodonModel* >(value)->setNucleotideFrequencies( f );
    static_cast< RateMatrix_ExperimentalCodonModel* >(value)->setBeta( b );
    static_cast< RateMatrix_ExperimentalCodonModel* >(value)->setKappa( k );
    static_cast< RateMatrix_ExperimentalCodonModel* >(value)->setOmega( o );

    value->update();
}



void ExperimentalCodonModelRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == amino_acid_preferences)
    {
        amino_acid_preferences = static_cast<const TypedDagNode< Simplex >* >( newP );
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

}
