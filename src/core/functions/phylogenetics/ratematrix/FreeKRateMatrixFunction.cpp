#include "FreeKRateMatrixFunction.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"

using namespace RevBayesCore;

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode< RbVector<double> > *tr) : TypedFunction<RateGenerator>( new RateMatrix_FreeK( 0.5+sqrt(0.25+tr->getValue().size() ) ) ),
    transitionRates( tr )
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
    const std::vector<double>& r = transitionRates->getValue();

    // set the base frequencies
    static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r);
    
    value->update();
}



void FreeKRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == transitionRates)
    {
        transitionRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}

