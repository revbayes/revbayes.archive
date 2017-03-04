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

BDRRateMatrixFunction::BDRRateMatrixFunction( const TypedDagNode<int> *n, const TypedDagNode<double> *a, const TypedDagNode<double> *b, const TypedDagNode<double> *l_a, const TypedDagNode<double> *m_a, const TypedDagNode<double> *l_i, const TypedDagNode<double> *m_i, const TypedDagNode<double> *l_ai, const TypedDagNode<double> *l_ia, const TypedDagNode<double> *d_i, const TypedDagNode<double> *d_a ) :
    TypedFunction<RateGenerator>( new RateMatrix_BDR(n->getValue()) ),

    alpha( a ),
    beta( b ),

    lambda_a( l_a ),
    mu_a( m_a ),

    lambda_i( l_i ),
    mu_i( m_i ),

    lambda_ai( l_ai ),
    lambda_ia( l_ia ),

    denovo_i( d_i ),
    denovo_a( d_a )


{

    
    addParameter( alpha );
    addParameter( beta );
    addParameter( lambda_a );
    addParameter( mu_a );
    addParameter( lambda_i );
    addParameter( mu_i );
    addParameter( lambda_ai );
    addParameter( lambda_ia );
    addParameter( denovo_i );
    addParameter( denovo_a );

    update();
}



BDRRateMatrixFunction::~BDRRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BDRRateMatrixFunction* BDRRateMatrixFunction::clone( void ) const {
    return new BDRRateMatrixFunction( *this );
}


void BDRRateMatrixFunction::update( void ) {
    
    double a = alpha->getValue();
    double b = beta->getValue();

    double l_i = lambda_i->getValue();
    double m_i = mu_i->getValue();

    double l_a = lambda_a->getValue();
    double m_a = mu_a->getValue();
    
    double l_ai = lambda_ai->getValue();
    double l_ia = lambda_ia->getValue();
    
    double d_i = denovo_i->getValue();
    double d_a = denovo_a->getValue();
    

    static_cast< RateMatrix_BDR* >(value)->setAlpha( a );
    static_cast< RateMatrix_BDR* >(value)->setBeta( b );
    static_cast< RateMatrix_BDR* >(value)->setLambdaA( l_a );
    static_cast< RateMatrix_BDR* >(value)->setMuA( m_a );
    static_cast< RateMatrix_BDR* >(value)->setLambdaI( l_i );
    static_cast< RateMatrix_BDR* >(value)->setMuI( m_i );
    static_cast< RateMatrix_BDR* >(value)->setLambdaAI( l_ai );
    static_cast< RateMatrix_BDR* >(value)->setLambdaIA( l_ia );
    static_cast< RateMatrix_BDR* >(value)->setDenovoI( d_i );
    static_cast< RateMatrix_BDR* >(value)->setDenovoA( d_a );
    
    static_cast< RateMatrix_BDR* >(value)->update();
    
}



void BDRRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == alpha) {
        alpha = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == beta) {
        beta = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lambda_a) {
        lambda_a = static_cast<const TypedDagNode<double>* >( newP );
    } else if (oldP == mu_a) {
        mu_a = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lambda_i) {
        lambda_i = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == mu_i) {
        mu_i = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lambda_ai) {
        lambda_ai = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == lambda_ia) {
        lambda_ia = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == denovo_i) {
        denovo_i = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == denovo_a) {
        denovo_a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



