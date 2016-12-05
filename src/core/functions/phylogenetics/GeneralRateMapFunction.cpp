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
    homogeneous_rate_matrix               = new ConstantNode<RateGenerator>("homogeneous_rate_matrix", new RateMatrix_JC(ns));
    heterogeneous_rate_matrices           = NULL;
    homogeneous_clock_rate                = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneous_clock_rates             = NULL;
    root_frequencies                     = new ConstantNode<RbVector<double> >("root_frequencies", new RbVector<double>(ns,1.0/ns));
    
    branch_heterogeneous_clock_rates       = false;
    branchHeterogeneousRateMatrices    = false;
    
    this->addParameter(homogeneous_rate_matrix);
    this->addParameter(homogeneous_clock_rate);
    this->addParameter(root_frequencies);
    
    update();
}


GeneralRateMapFunction::GeneralRateMapFunction(const GeneralRateMapFunction &n) : TypedFunction<RateMap>( n )
{
    homogeneous_rate_matrix = n.homogeneous_rate_matrix;
    heterogeneous_rate_matrices = n.heterogeneous_rate_matrices;
    homogeneous_clock_rate = n.homogeneous_clock_rate;
    heterogeneous_clock_rates = n.heterogeneous_clock_rates;
    root_frequencies = n.root_frequencies;
    
    branch_heterogeneous_clock_rates = n.branch_heterogeneous_clock_rates;
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
        const RbVector<RateGenerator>& rm = heterogeneous_rate_matrices->getValue();
        static_cast<RateMap*>(value)->setHeterogeneousRateMatrices(rm);
    }
    else
    {
        const RateGenerator& rm = homogeneous_rate_matrix->getValue();
        static_cast<RateMap*>(value)->setHomogeneousRateMatrix(&rm);
    }
    
    if (branch_heterogeneous_clock_rates)
    {
        const std::vector<double>& r = heterogeneous_clock_rates->getValue();
        static_cast< RateMap* >(value)->setHeterogeneousClockRates(r);
    }
    else
    {
        const double& r = homogeneous_clock_rate->getValue();
        static_cast< RateMap* >(value)->setHomogeneousClockRate(r);
    }
    
    value->updateMap();
}

void GeneralRateMapFunction::setRateMatrix(const TypedDagNode<RateGenerator>* r)
{
    // remove the old parameter first
    if ( homogeneous_rate_matrix != NULL )
    {
        this->removeParameter( homogeneous_rate_matrix );
        homogeneous_rate_matrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneous_rate_matrices );
        heterogeneous_rate_matrices = NULL;
    }
    
    // set the value
    branchHeterogeneousRateMatrices = false;
    homogeneous_rate_matrix = r;
    
    // add the parameter
    this->addParameter( homogeneous_rate_matrix );
}

void GeneralRateMapFunction::setClockRate(const TypedDagNode< double > *r) {
    
    // remove the old parameter first
    if ( homogeneous_clock_rate != NULL )
    {
        this->removeParameter( homogeneous_clock_rate );
        homogeneous_clock_rate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneous_clock_rates );
        heterogeneous_clock_rates = NULL;
    }
    
    // set the value
    branch_heterogeneous_clock_rates = false;
    homogeneous_clock_rate = r;
    
    // add the parameter
    this->addParameter( homogeneous_clock_rate );
}

void GeneralRateMapFunction::setClockRate(const TypedDagNode< RbVector< double > > *r) {
    
    // remove the old parameter first
    if ( homogeneous_clock_rate != NULL )
    {
        this->removeParameter( homogeneous_clock_rate );
        homogeneous_clock_rate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneous_clock_rates );
        heterogeneous_clock_rates = NULL;
    }
    
    // set the value
    branch_heterogeneous_clock_rates = true;
    heterogeneous_clock_rates = r;
    
    // add the parameter
    this->addParameter( heterogeneous_clock_rates );
    
}

void GeneralRateMapFunction::setRootFrequencies(const TypedDagNode<RbVector<double> > *f)
{
    if (root_frequencies != NULL)
    {
        this->removeParameter(root_frequencies);
        root_frequencies = NULL;
    }
    
    root_frequencies = f;
    this->addParameter(root_frequencies);
}

void GeneralRateMapFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == homogeneous_rate_matrix)
    {
        homogeneous_rate_matrix = static_cast<const TypedDagNode<RateGenerator>* >( newP );
    }
    else if (oldP == heterogeneous_rate_matrices)
    {
        heterogeneous_rate_matrices = static_cast<const TypedDagNode<RbVector<RateGenerator> >* >( newP );
    }
    else if (oldP == homogeneous_clock_rate)
    {
        homogeneous_clock_rate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneous_clock_rates)
    {
        heterogeneous_clock_rates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else if (oldP == root_frequencies)
    {
        root_frequencies = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
}

//std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x)
//{
//    o << "";
//    return o;
//}
