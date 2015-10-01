#include "GtrRateMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

GtrRateMatrixFunction::GtrRateMatrixFunction(const TypedDagNode< RbVector<double> > *er, const TypedDagNode< RbVector<double> > *bf) : TypedFunction<RateGenerator>( new RateMatrix_GTR(bf->getValue().size()) ),
    exchangeabilityRates( er ),
    baseFrequencies( bf )
{
    // add the lambda parameter as a parent
    addParameter( baseFrequencies );
    addParameter( exchangeabilityRates );
    
    update();
}

GtrRateMatrixFunction::~GtrRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GtrRateMatrixFunction* GtrRateMatrixFunction::clone( void ) const
{
    return new GtrRateMatrixFunction( *this );
}


void GtrRateMatrixFunction::update( void )
{

    // get the information from the arguments for reading the file
    const std::vector<double>& r = exchangeabilityRates->getValue();
    const std::vector<double>& f = baseFrequencies->getValue();
    
    
    // set the base frequencies
    static_cast< RateMatrix_GTR* >(value)->setStationaryFrequencies( f );
    static_cast< RateMatrix_GTR* >(value)->setExchangeabilityRates( r );
    
    value->update();
    
}



void GtrRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == baseFrequencies)
    {
        baseFrequencies = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == exchangeabilityRates)
    {
        exchangeabilityRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }

}


