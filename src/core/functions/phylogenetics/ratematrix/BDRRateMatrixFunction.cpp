/**
 * @file
 * This file contains the implementation of the BDRRateMatrixFunction class.
 * This class is derived from the function class and is used to
 * create the chromosomes rate matrix.
 *
 * @brief Implementation of the BDRRateMatrixFunction.
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 */

#include "BDRRateMatrixFunction.h"
#include "RateMatrix_BDR.h"
#include "RbException.h"

using namespace RevBayesCore;

BDRRateMatrixFunction::BDRRateMatrixFunction(const TypedDagNode<int> *n, const TypedDagNode<double> *l, const TypedDagNode<double> *d, const TypedDagNode<double> *r, const TypedDagNode<double> *m, const TypedDagNode<double> *l_l, const TypedDagNode<double> *d_l) :
    TypedFunction<RateGenerator>( new RateMatrix_BDR(n->getValue()) ),
    lambda( l ),
    delta( d ),
    rho( r ),
    mu( m ),
    lambda_l( l_l ),
    delta_l( d_l )
{

    addParameter( lambda );
    addParameter( delta );
    addParameter( rho );
    addParameter( lambda_l );
    addParameter( delta_l );
    addParameter( mu );
    
    update();
}



BDRRateMatrixFunction::~BDRRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BDRRateMatrixFunction* BDRRateMatrixFunction::clone( void ) const {
    return new BDRRateMatrixFunction( *this );
}


void BDRRateMatrixFunction::update( void ) {
    double la = lambda->getValue();
    double de = delta->getValue();
    double rh = rho->getValue();
    double la_l = lambda_l->getValue();
    double de_l = delta_l->getValue();
    double m = mu->getValue();

    static_cast< RateMatrix_BDR* >(value)->setLambda( la );
    static_cast< RateMatrix_BDR* >(value)->setDelta( de );
    static_cast< RateMatrix_BDR* >(value)->setRho( rh );
    static_cast< RateMatrix_BDR* >(value)->setMu( m );
    static_cast< RateMatrix_BDR* >(value)->setLambda_l( la_l );
    static_cast< RateMatrix_BDR* >(value)->setDelta_l( de_l );

    static_cast< RateMatrix_BDR* >(value)->update();
    
}



void BDRRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == lambda) {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == delta) {
        delta = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho) {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    } else if (oldP == mu) {
        mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == delta_l) {
        delta_l = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lambda_l) {
        lambda_l = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



