#include "GtrRateMatrixFunction.h"
#include "GtrRateMatrix.h"
#include "RbException.h"

using namespace RevBayesCore;

GtrRateMatrixFunction::GtrRateMatrixFunction(const TypedDagNode<std::vector<double> > *er, const TypedDagNode<std::vector<double> > *bf) : TypedFunction<RateMatrix>( new RateMatrix_GTR(bf->getValue().size()) ), exchangeabilityRates( er ), baseFrequencies( bf ) {
    // add the lambda parameter as a parent
    addParameter( baseFrequencies );
    addParameter( exchangeabilityRates );
    
    update();
}


GtrRateMatrixFunction::GtrRateMatrixFunction(const GtrRateMatrixFunction &n) : TypedFunction<RateMatrix>( n ), baseFrequencies( n.baseFrequencies ), exchangeabilityRates( n.exchangeabilityRates ) {
    // no need to add parameters, happens automatically
}


GtrRateMatrixFunction::~GtrRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GtrRateMatrixFunction* GtrRateMatrixFunction::clone( void ) const {
    return new GtrRateMatrixFunction( *this );
}


void GtrRateMatrixFunction::update( void ) {
    // get the information from the arguments for reading the file
    const std::vector<double>& r = exchangeabilityRates->getValue();
    const std::vector<double>& f = baseFrequencies->getValue();
    
    // delegate the call to our internal function
    RbPhylogenetics::Gtr::computeRateMatrix(r, f, static_cast< RateMatrix_GTR* >( value ) );
}



void GtrRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == baseFrequencies) {
        baseFrequencies = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == exchangeabilityRates) {
        exchangeabilityRates = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}


