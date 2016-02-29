#include "FreeKRateMatrixFunction.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"

using namespace RevBayesCore;

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode< RbVector<double> > *trf) : TypedFunction<RateGenerator>( new RateMatrix_FreeK( 0.5+sqrt(0.25+trf->getValue().size() ) ) ),
    transitionRates( NULL ),
    transitionRatesFlat( trf )
{
    
    // add the rate and frequency parameters as parents
    addParameter( transitionRatesFlat );
    
    update();
}

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode< RbVector<RbVector<double> > >*tr) : TypedFunction<RateGenerator>( new RateMatrix_FreeK( tr->getValue().size() ) ),
    transitionRates( tr ),
    transitionRatesFlat( NULL )
{
    
    // add the rate and frequency parameters as parents
    addParameter( transitionRates );
    
    update();
}

FreeKRateMatrixFunction::~FreeKRateMatrixFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



FreeKRateMatrixFunction* FreeKRateMatrixFunction::clone( void ) const
{
    return new FreeKRateMatrixFunction( *this );
}


void FreeKRateMatrixFunction::update( void )
{
    // get the information from the arguments for reading the file
    if (transitionRates != NULL && transitionRatesFlat == NULL) {
        const RbVector<RbVector<double> >& r = transitionRates->getValue();
        
        size_t n = r.size();
        std::vector<double> r_flat( n * (n-1) );
        size_t k = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                if (i != j) {
                    r_flat[k++] = r[i][j];
                }
            }
        }

        // set the flattened rates
        static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r_flat);
    }
    
    else if (transitionRates == NULL && transitionRatesFlat != NULL) {
        const std::vector<double>& r = transitionRatesFlat->getValue();
        
        // set the flattened rates
        static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r);
    }


    
    value->update();
}



void FreeKRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == transitionRates)
    {
        transitionRates = static_cast<const TypedDagNode< RbVector<RbVector<double> > >* >( newP );
    }
    else if (oldP == transitionRatesFlat)
    {
        transitionRatesFlat = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

