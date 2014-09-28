//
//  BiogeographyRateMapFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "BiogeographyRateMapFunction.h"
#include "ConstantNode.h"
#include "RateMatrix_JC.h"
#include "RbException.h"

using namespace RevBayesCore;

BiogeographyRateMapFunction::BiogeographyRateMapFunction(size_t nc, bool fe) : TypedFunction<RateMap>( new RateMap_Biogeography( nc, fe ) )
{
//    homogeneousGainLossRates            = new ConstantNode<std::vector<double> >("homogeneousGainLossRates", new std::vector<double>(2,0.5));
//    heterogeneousGainLossRates          = NULL;
    homogeneousRateMatrix               = new ConstantNode<RateMatrix>("homogeneousRateMatrix", new RateMatrix_JC(2));
    heterogeneousRateMatrices           = NULL;
    homogeneousClockRate                = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates             = NULL;
    geographyRateModifier               = NULL;
    rootFrequencies                     = new ConstantNode<std::vector<double> >("rootFrequencies", new std::vector<double>(2,0.5));
    
    branchHeterogeneousClockRates       = false;
    branchHeterogeneousRateMatrices     = false;
    useGeographicDistance               = false;
    
    this->addParameter(homogeneousRateMatrix);
    this->addParameter(homogeneousClockRate);
    this->addParameter(rootFrequencies);
    
    update();
}


BiogeographyRateMapFunction::BiogeographyRateMapFunction(const BiogeographyRateMapFunction &n) : TypedFunction<RateMap>( n )
{
//    homogeneousGainLossRates = n.homogeneousGainLossRates;
//    heterogeneousGainLossRates = n.heterogeneousGainLossRates;
    homogeneousRateMatrix = n.homogeneousRateMatrix;
    heterogeneousRateMatrices = n.heterogeneousRateMatrices;
    homogeneousClockRate = n.homogeneousClockRate;
    heterogeneousClockRates = n.heterogeneousClockRates;
    geographyRateModifier = n.geographyRateModifier;
    rootFrequencies = n.rootFrequencies;
    
    branchHeterogeneousClockRates = n.branchHeterogeneousClockRates;
    branchHeterogeneousRateMatrices = n.branchHeterogeneousRateMatrices;
    useGeographicDistance = n.useGeographicDistance;
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
        const RbVector<RateMatrix>& rm = heterogeneousRateMatrices->getValue();
        static_cast< RateMap_Biogeography* >(value)->setHeterogeneousRateMatrices(rm);
    }
    else
    {
        const RateMatrix& rm = homogeneousRateMatrix->getValue();
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

void BiogeographyRateMapFunction::setRateMatrix(const TypedDagNode<RateMatrix>* r)
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

void BiogeographyRateMapFunction::setClockRate(const TypedDagNode< std::vector< double > > *r) {
    
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

//void BiogeographyRateMapFunction::setCountRateModifier(const TypedDagNode<CountRateModifier> *crm) {
//    
//    // remove the old parameter first
//    if ( countRateModifier != NULL )
//    {
//        this->removeParameter( countRateModifier );
//        countRateModifier = NULL;
//    }
//    
//    // set the value
//    useCountRateModifier = true;
//    countRateModifier = crm;
//    
//    // add the parameter
//    this->addParameter( countRateModifier );
//    
//}

void BiogeographyRateMapFunction::setRootFrequencies(const TypedDagNode<std::vector<double> > *f)
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
        homogeneousRateMatrix = static_cast<const TypedDagNode<RateMatrix>* >( newP );
    }
    else if (oldP == heterogeneousRateMatrices)
    {
        heterogeneousRateMatrices = static_cast<const TypedDagNode<RbVector<RateMatrix> >* >( newP );
    }
    else if (oldP == homogeneousClockRate)
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates)
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == geographyRateModifier)
    {
        geographyRateModifier = static_cast<const TypedDagNode<GeographyRateModifier>* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode<std::vector<double> >* >( newP );
    }
}

std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x)
{
    o << "";
    return o;
}