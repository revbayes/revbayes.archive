#include "DoubletEpistaticGTRRateMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;


DoubletEpistaticGTRRateMatrixFunction::DoubletEpistaticGTRRateMatrixFunction(const TypedDagNode<double> *d, const TypedDagNode< Simplex > *er, const TypedDagNode< Simplex > *df) : TypedFunction<RateGenerator>( new RateMatrix_DoubletEpistaticGTR() ),
doublet_frequencies( df ),
exchange_rates( er ),
epistasis_d( d )
{
    // add the lambda parameter as a parent
    addParameter( doublet_frequencies );
    addParameter( exchange_rates );
    addParameter( epistasis_d );

    update();
}


DoubletEpistaticGTRRateMatrixFunction::~DoubletEpistaticGTRRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



DoubletEpistaticGTRRateMatrixFunction* DoubletEpistaticGTRRateMatrixFunction::clone( void ) const
{

    return new DoubletEpistaticGTRRateMatrixFunction( *this );
}


void DoubletEpistaticGTRRateMatrixFunction::update( void )
{
    // get the information from the arguments for reading the file
    double d = epistasis_d->getValue();
    const std::vector<double>& e = exchange_rates->getValue();
    const std::vector<double>& f = doublet_frequencies->getValue();


    // set the base frequencies
    static_cast< RateMatrix_DoubletEpistaticGTR* >(value)->setDoubletFrequencies( f );
    static_cast< RateMatrix_DoubletEpistaticGTR* >(value)->setNucleotideExchangeRates( e );
    static_cast< RateMatrix_DoubletEpistaticGTR* >(value)->setD( d );

    value->update();
}



void DoubletEpistaticGTRRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == doublet_frequencies)
    {
        doublet_frequencies = static_cast<const TypedDagNode< Simplex >* >( newP );
    }

    if (oldP == exchange_rates)
    {
        exchange_rates = static_cast<const TypedDagNode< Simplex >* >( newP );
    }

    if (oldP == epistasis_d)
    {
        epistasis_d = static_cast<const TypedDagNode< double >* >( newP );
    }

}
