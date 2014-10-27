//
//  FreeKRateMatrixFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "FreeKRateMatrixFunction.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"

using namespace RevBayesCore;

FreeKRateMatrixFunction::FreeKRateMatrixFunction(const TypedDagNode<std::vector<double> > *tr, const TypedDagNode<std::vector<double> > *sf) : TypedFunction<RateMatrix>( new RateMatrix_FreeK(sf->getValue().size()) ), transitionRates( tr ), stationaryFrequencies( sf ) {
    // add the lambda parameter as a parent
    addParameter( transitionRates );
    addParameter( stationaryFrequencies );
    
    update();
}


FreeKRateMatrixFunction::FreeKRateMatrixFunction(const FreeKRateMatrixFunction &n) : TypedFunction<RateMatrix>( n ), transitionRates( n.transitionRates ), stationaryFrequencies(n.stationaryFrequencies) {
    // no need to add parameters, happens automatically
}


FreeKRateMatrixFunction::~FreeKRateMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



FreeKRateMatrixFunction* FreeKRateMatrixFunction::clone( void ) const {
    return new FreeKRateMatrixFunction( *this );
}


void FreeKRateMatrixFunction::update( void ) {
    // get the information from the arguments for reading the file
    const std::vector<double>& r = transitionRates->getValue();
    const std::vector<double>& f = stationaryFrequencies->getValue();

    // set the base frequencies
    static_cast< RateMatrix_FreeK* >(value)->setTransitionRates(r);
    static_cast< RateMatrix_FreeK* >(value)->setStationaryFrequencies(f);
    
    value->updateMatrix();
}



void FreeKRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == transitionRates) {
        transitionRates = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == stationaryFrequencies) {
        stationaryFrequencies = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}

