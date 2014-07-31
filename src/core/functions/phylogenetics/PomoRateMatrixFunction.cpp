#include "PomoRateMatrixFunction.h"
#include "RateMatrix_Pomo.h"
#include "RbException.h"

using namespace RevBayesCore;

PomoRateMatrixFunction::PomoRateMatrixFunction(const TypedDagNode<int > *ps, const TypedDagNode<std::vector<double> > *mr, const TypedDagNode< std::vector<double>  > *sc) : TypedFunction<RateMatrix>( new RateMatrix_Pomo(4 + 6*(ps->getValue() - 1)) ), populationSize( ps ), mutationRates( mr ), selectionCoefficients ( sc ) {
    // add the lambda parameter as a parent
    addParameter( populationSize );
    addParameter( mutationRates );
    addParameter( selectionCoefficients );

    update();
}


PomoRateMatrixFunction::~PomoRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



PomoRateMatrixFunction* PomoRateMatrixFunction::clone( void ) const
{
    return new PomoRateMatrixFunction( *this );
}


void PomoRateMatrixFunction::update( void )
{
    //For the moment, we do not allow that the population size could be updated.
    
    // get the information from the arguments for reading the file
    addParameter( mutationRates );
    addParameter( selectionCoefficients );

    const std::vector<double>& r = mutationRates->getValue();
    const std::vector<double>& s = selectionCoefficients->getValue();
    
    
    // set the base frequencies
    static_cast< RateMatrix_Pomo* >(value)->setMutationRates( r );
    static_cast< RateMatrix_Pomo* >(value)->setSelectionCoefficients( s );
    
    value->updateMatrix();
    
}



void PomoRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == mutationRates)
    {
        mutationRates = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == selectionCoefficients)
    {
        selectionCoefficients = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    
}


