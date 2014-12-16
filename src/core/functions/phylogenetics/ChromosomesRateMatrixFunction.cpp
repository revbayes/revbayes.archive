/**
 * @file
 * This file contains the implementation of the ChromosomesRateMatrixFunction class.
 * This class is derived from the function class and is used to
 * create the chromosomes rate matrix.
 *
 * @brief Implementation of the ChromosomesRateMatrixFunction.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */

#include "ChromosomesRateMatrixFunction.h"
#include "RateMatrix_Chromosomes.h"
#include "RbException.h"

using namespace RevBayesCore;

ChromosomesRateMatrixFunction::ChromosomesRateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l, const TypedDagNode<double> *d, const TypedDagNode<double> *r) : TypedFunction<RateMatrix>( new RateMatrix_Chromosomes(n->getValue()) ), lambda( l ), delta( d ), rho( r ) {

    addParameter( lambda );
    addParameter( delta );
    addParameter( rho );

    update();
}



ChromosomesRateMatrixFunction::~ChromosomesRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



ChromosomesRateMatrixFunction* ChromosomesRateMatrixFunction::clone( void ) const {
    return new ChromosomesRateMatrixFunction( *this );
}


void ChromosomesRateMatrixFunction::update( void ) {
    double la = lambda->getValue();
    double de = delta->getValue();
    double rh = rho->getValue();

    static_cast< RateMatrix_Chromosomes* >(value)->setLambda( la );
    static_cast< RateMatrix_Chromosomes* >(value)->setDelta( de );
    static_cast< RateMatrix_Chromosomes* >(value)->setRho( rh );

    static_cast< RateMatrix_Chromosomes* >(value)->updateMatrix();
    
}



void ChromosomesRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == delta) {
        delta = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho) {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



