#include "Mk1RateMatrixFunction.h"
#include "RateMatrix_Mk1.h"
#include "RbException.h"

using namespace RevBayesCore;

Mk1RateMatrixFunction::Mk1RateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l) : TypedFunction<RateMatrix>( new RateMatrix_Mk1(n->getValue()) ), lambda( l ) {
	
    addParameter( lambda );
	
    update();
}



Mk1RateMatrixFunction::~Mk1RateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



Mk1RateMatrixFunction* Mk1RateMatrixFunction::clone( void ) const {
    return new Mk1RateMatrixFunction( *this );
}


void Mk1RateMatrixFunction::update( void ) {
    double la = lambda->getValue();
	
    static_cast< RateMatrix_Mk1* >(value)->setLambda( la );
	
    static_cast< RateMatrix_Mk1* >(value)->updateMatrix();
    
}



void Mk1RateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



