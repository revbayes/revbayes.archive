//
//  BiogeographyRateMapFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

BiogeographyRateMapFunction::BiogeographyRateMapFunction(const TypedDagNode<std::vector<double> > *glr, const TypedDagNode<double> *dp, size_t nc) : TypedFunction<RateMap>( new RateMap_Biogeography( nc ) ), distancePower( dp ), gainLossRates( glr ) {
    // add the lambda parameter as a parent
    addParameter( distancePower );
    addParameter( gainLossRates );
    
    update();
}


BiogeographyRateMapFunction::BiogeographyRateMapFunction(const BiogeographyRateMapFunction &n) : TypedFunction<RateMap>( n ), distancePower( n.distancePower ), gainLossRates( n.gainLossRates ) {
    // no need to add parameters, happens automatically
}


BiogeographyRateMapFunction::~BiogeographyRateMapFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BiogeographyRateMapFunction* BiogeographyRateMapFunction::clone( void ) const {
    return new BiogeographyRateMapFunction( *this );
}


void BiogeographyRateMapFunction::update( void ) {
    
    // set the gainLossRate
    const std::vector<double>& glr = gainLossRates->getValue();
    static_cast< RateMap_Biogeography* >(value)->setGainLossRates(glr);
    
    // set the distancePower
    double dp = distancePower->getValue();
    static_cast< RateMap_Biogeography* >(value)->setDistancePower(dp);
    
    value->updateMap();
}



void BiogeographyRateMapFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == gainLossRates)
    {
        gainLossRates = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
    else if (oldP == distancePower) {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}

