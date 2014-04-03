//
//  BiogeographyRateFunctionMatrixFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "BiogeographyRateFunctionMatrixFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

BiogeographyRateFunctionMatrixFunction::BiogeographyRateFunctionMatrixFunction(const TypedDagNode<double> *d, size_t nc) : TypedFunction<RateFunctionMatrix>( new RateMatrix_Biogeography(2, nc) ), distancePower( d ) {
    // add the lambda parameter as a parent
    addParameter( distancePower );
    
    update();
}


BiogeographyRateFunctionMatrixFunction::BiogeographyRateFunctionMatrixFunction(const BiogeographyRateFunctionMatrixFunction &n) : TypedFunction<RateFunctionMatrix>( n ), distancePower( n.distancePower ) {
    // no need to add parameters, happens automatically
}


BiogeographyRateFunctionMatrixFunction::~BiogeographyRateFunctionMatrixFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BiogeographyRateFunctionMatrixFunction* BiogeographyRateFunctionMatrixFunction::clone( void ) const {
    return new BiogeographyRateFunctionMatrixFunction( *this );
}


void BiogeographyRateFunctionMatrixFunction::update( void ) {
    // get the information from the arguments for reading the file
    double d = distancePower->getValue();
    
    // set the base frequencies
    static_cast< RateMatrix_Biogeography* >(value)->setDistancePower( d );
    
    value->updateMatrix();
}



void BiogeographyRateFunctionMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == distancePower) {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}


