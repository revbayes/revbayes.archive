//
//  BiogeographyRateMapFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "ConstantNode.h"
#include "RateGenerator.h"
#include "RateMatrix_JC.h"
#include "RbException.h"

using namespace RevBayesCore;

BiogeographyRateMapFunction::BiogeographyRateMapFunction(size_t nc, bool fe, unsigned mrs) : TypedFunction<RateMap>( new RateMap_Biogeography( nc, fe, mrs ) )
{
//    homogeneousGainLossRates            = new ConstantNode<RbVector<double> >("homogeneousGainLossRates", new RbVector<double>(2,0.5));
//    heterogeneousGainLossRates          = NULL;
    homogeneousRateMatrix               = new ConstantNode<RateGenerator>("homogeneousRateMatrix", new RateMatrix_JC(2));
    heterogeneousRateMatrices           = NULL;
    homogeneousClockRate                = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates             = NULL;
    geographyRateModifier               = NULL;
    rootFrequencies                     = new ConstantNode< RbVector<double> >("rootFrequencies", new RbVector<double>(2,0.5));
    
    branchHeterogeneousClockRates       = false;
    branchHeterogeneousRateMatrices     = false;
    useGeographicDistance               = false;
    
    this->addParameter(homogeneousRateMatrix);
    this->addParameter(homogeneousClockRate);
    this->addParameter(rootFrequencies);
    
    update();
}


BiogeographyRateMapFunction::~BiogeographyRateMapFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



BiogeographyRateMapFunction* BiogeographyRateMapFunction::clone( void ) const {
    return new BiogeographyRateMapFunction( *this );
}


void BiogeographyRateMapFunction::update( void ) {
    
    // touch specialization for granular updates?
    
    // set the gainLossRate
    if (branchHeterogeneousRateMatrices)
    {
        // Disabled for now due to ostream errors...
        const RbVector<RateGenerator>& rm = heterogeneousRateMatrices->getValue();
        static_cast< RateMap_Biogeography* >(value)->setHeterogeneousRateMatrices(rm);
    }
    else
    {
        const RateGenerator& rm = homogeneousRateMatrix->getValue();
        static_cast< RateMap_Biogeography* >(value)->setHomogeneousRateMatrix(&rm);
    }

    if (branchHeterogeneousClockRates)
    {
        const std::vector<double>& r = heterogeneousClockRates->getValue();
        static_cast< RateMap_Biogeography* >(value)->setHeterogeneousClockRates(r);
    }
    else
    {
        const double& r = homogeneousClockRate->getValue();
        static_cast< RateMap_Biogeography* >(value)->setHomogeneousClockRate(r);
    }
    
    // set the distancePower
    if (useGeographicDistance)
    {
        const GeographyRateModifier& drm = geographyRateModifier->getValue();
        static_cast< RateMap_Biogeography* >(value)->setGeographyRateModifier(drm);
    }
    
    if (rootFrequencies != NULL)
    {
        const std::vector<double>& f = rootFrequencies->getValue();
        static_cast<RateMap_Biogeography*>(value)->setRootFrequencies(f);
    }
    
    value->updateMap();
}

void BiogeographyRateMapFunction::setRateMatrix(const TypedDagNode<RateGenerator>* r)
{
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL )
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousRateMatrices );
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousRateMatrices = false;
    homogeneousRateMatrix = r;
    
    // add the parameter
    this->addParameter( homogeneousRateMatrix );
}


void BiogeographyRateMapFunction::setClockRate(const TypedDagNode< double > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL )
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
    
    // add the parameter
    this->addParameter( homogeneousClockRate );
}

void BiogeographyRateMapFunction::setClockRate(const TypedDagNode< RbVector< double > > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL )
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
    
    // add the parameter
    this->addParameter( heterogeneousClockRates );
    
}

void BiogeographyRateMapFunction::setGeographyRateModifier(const TypedDagNode<GeographyRateModifier> *drm) {
    
    // remove the old parameter first
    if ( geographyRateModifier != NULL )
    {
        this->removeParameter( geographyRateModifier );
        geographyRateModifier = NULL;
    }
    
    // set the value
    useGeographicDistance = true;
    geographyRateModifier = drm;
    
    // add the parameter
    this->addParameter( geographyRateModifier );

}

void BiogeographyRateMapFunction::setRootFrequencies(const TypedDagNode< RbVector<double> > *f)
{
    if (rootFrequencies != NULL)
    {
        this->removeParameter(rootFrequencies);
        rootFrequencies = NULL;
    }
    
    rootFrequencies = f;
    this->addParameter(rootFrequencies);
}

void BiogeographyRateMapFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == homogeneousRateMatrix)
    {
        homogeneousRateMatrix = static_cast<const TypedDagNode<RateGenerator>* >( newP );
    }
    else if (oldP == heterogeneousRateMatrices)
    {
        heterogeneousRateMatrices = static_cast<const TypedDagNode<RbVector<RateGenerator> >* >( newP );
    }
    else if (oldP == homogeneousClockRate)
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates)
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == geographyRateModifier)
    {
        geographyRateModifier = static_cast<const TypedDagNode<GeographyRateModifier>* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}
