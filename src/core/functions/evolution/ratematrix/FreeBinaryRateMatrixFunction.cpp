//
//  FreeBinaryRateMatrixFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "FreeBinaryRateMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

FreeBinaryRateMatrixFunction::FreeBinaryRateMatrixFunction(const TypedDagNode<RbVector<double> > *tr) : TypedFunction<RateMatrix>( new RateMatrix_FreeBinary() ), transitionRates( tr ) {
    // add the lambda parameter as a parent
    addParameter( transitionRates );
    
    update();
}


FreeBinaryRateMatrixFunction::FreeBinaryRateMatrixFunction(const FreeBinaryRateMatrixFunction &n) : TypedFunction<RateMatrix>( n ), transitionRates( n.transitionRates ) {
    // no need to add parameters, happens automatically
}


FreeBinaryRateMatrixFunction::~FreeBinaryRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



FreeBinaryRateMatrixFunction* FreeBinaryRateMatrixFunction::clone( void ) const {
    return new FreeBinaryRateMatrixFunction( *this );
}


void FreeBinaryRateMatrixFunction::update( void ) {
    // get the information from the arguments for reading the file
    const std::vector<double>& r = transitionRates->getValue();
    
    // set the base frequencies
    static_cast< RateMatrix_FreeBinary* >(value)->setTransitionRates(r);
    
    value->updateMatrix();
}



void FreeBinaryRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == transitionRates) {
        transitionRates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}

