//
//  GeneralRateMapFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "GeneralRateMapFunction.h"
#include "RateMatrix_JC.h"
#include "RateMap.h"
#include "ConstantNode.h"
#include "RbException.h"

using namespace RevBayesCore;

GeneralRateMapFunction::GeneralRateMapFunction(size_t ns, size_t nc) : TypedFunction<RateMap>( new RateMap(ns, nc) )
{
    homogeneousRateMatrix               = new ConstantNode<RateMatrix>("homogeneousRateMatrix", new RateMatrix_JC(ns));
    heterogeneousRateMatrices           = NULL;
    homogeneousClockRate                = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates             = NULL;
    rootFrequencies                     = new ConstantNode<RbVector<double> >("rootFrequencies", new RbVector<double>(ns,1.0/ns));
    
    branchHeterogeneousClockRates       = false;
    branchHeterogeneousRateMatrices    = false;
    
    this->addParameter(homogeneousRateMatrix);
    this->addParameter(homogeneousClockRate);
    this->addParameter(rootFrequencies);
    
    update();
}


GeneralRateMapFunction::GeneralRateMapFunction(const GeneralRateMapFunction &n) : TypedFunction<RateMap>( n )
{
    homogeneousRateMatrix = n.homogeneousRateMatrix;
    heterogeneousRateMatrices = n.heterogeneousRateMatrices;
    homogeneousClockRate = n.homogeneousClockRate;
    heterogeneousClockRates = n.heterogeneousClockRates;
    rootFrequencies = n.rootFrequencies;
    
    branchHeterogeneousClockRates = n.branchHeterogeneousClockRates;
    branchHeterogeneousRateMatrices = n.branchHeterogeneousRateMatrices;
}


GeneralRateMapFunction::~GeneralRateMapFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GeneralRateMapFunction* GeneralRateMapFunction::clone( void ) const {
    return new GeneralRateMapFunction( *this );
}


void GeneralRateMapFunction::update( void ) {
    
    // set the gainLossRate
    if (branchHeterogeneousRateMatrices)
    {
        const RbVector<RateMatrix>& rm = heterogeneousRateMatrices->getValue();
        static_cast<RateMap*>(value)->setHeterogeneousRateMatrices(rm);
    }
    else
    {
        const RateMatrix& rm = homogeneousRateMatrix->getValue();
        static_cast<RateMap*>(value)->setHomogeneousRateMatrix(&rm);
    }
    
    if (branchHeterogeneousClockRates)
    {
        const std::vector<double>& r = heterogeneousClockRates->getValue();
        static_cast< RateMap* >(value)->setHeterogeneousClockRates(r);
    }
    else
    {
        const double& r = homogeneousClockRate->getValue();
        static_cast< RateMap* >(value)->setHomogeneousClockRate(r);
    }
    
    value->updateMap();
}

void GeneralRateMapFunction::setRateMatrix(const TypedDagNode<RateMatrix>* r)
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

void GeneralRateMapFunction::setClockRate(const TypedDagNode< double > *r) {
    
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

void GeneralRateMapFunction::setClockRate(const TypedDagNode< RbVector< double > > *r) {
    
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

void GeneralRateMapFunction::setRootFrequencies(const TypedDagNode<RbVector<double> > *f)
{
    if (rootFrequencies != NULL)
    {
        this->removeParameter(rootFrequencies);
        rootFrequencies = NULL;
    }
    
    rootFrequencies = f;
    this->addParameter(rootFrequencies);
}

void GeneralRateMapFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
        heterogeneousClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == rootFrequencies)
    {
        rootFrequencies = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}

//std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x)
//{
//    o << "";
//    return o;
//}
