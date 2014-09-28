#include "PomoRateMatrixFunction.h"
#include "RateMatrix_Pomo.h"
#include "RbException.h"

using namespace RevBayesCore;

PomoRateMatrixFunction::PomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RbVector<double> > *mr, const TypedDagNode< RbVector<double>  > *sc) : TypedFunction<RateMatrix>( new RateMatrix_Pomo(4 + 6*(ps->getValue() - 1), ps->getValue(), mr->getValue(), sc->getValue()) ),
    populationSize( ps ),
    mutationRates( mr ),
    selectionCoefficients ( sc )
{

    useMutationMatrix = false;
    // add the lambda parameter as a parent
    addParameter( populationSize );
    addParameter( mutationRates );
    addParameter( selectionCoefficients );

    update();
}


 //MJL 140822: caused compile error
PomoRateMatrixFunction::PomoRateMatrixFunction(const TypedDagNode< int > *ps, const TypedDagNode< RateMatrix > *mm, const TypedDagNode< RbVector<double>  > *sc) : TypedFunction<RateMatrix>( new RateMatrix_Pomo(4 + 6*(ps->getValue() - 1), ps->getValue(), mm->getValue(), sc->getValue()) ), populationSize( ps ), mutationMatrix( mm ), selectionCoefficients ( sc ) {
    useMutationMatrix = true;
    // add the lambda parameter as a parent
    addParameter( populationSize );
    addParameter( mutationMatrix );
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
    std::vector<double> r ;
    if (useMutationMatrix) {
        // get the information from the arguments for reading the file
        r = setMutationRates( mutationMatrix->getValue() );
    } else {
        // get the information from the arguments for reading the file
        r = mutationRates->getValue();

    }

    const std::vector<double>& s = selectionCoefficients->getValue();
    
    
    // set the base frequencies
    static_cast< RateMatrix_Pomo* >(value)->setMutationRates( r );
    static_cast< RateMatrix_Pomo* >(value)->setSelectionCoefficients( s );
    static_cast< RateMatrix_Pomo* >(value)->updateMatrix();
    
}



void PomoRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == mutationRates)
    {
        mutationRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == selectionCoefficients)
    {
        selectionCoefficients = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}


std::vector<double> PomoRateMatrixFunction::setMutationRates(const RateMatrix& mm) {
    std::vector<double> r;
    r.push_back( mm[0][1] );
    r.push_back( mm[0][2] );
    r.push_back( mm[0][3] );
    r.push_back( mm[1][0] );
    r.push_back( mm[1][2] );
    r.push_back( mm[1][3] );
    r.push_back( mm[2][0] );
    r.push_back( mm[2][1] );
    r.push_back( mm[2][3] );
    r.push_back( mm[3][0] );
    r.push_back( mm[3][1] );
    r.push_back( mm[3][2] );
    return r;
}

