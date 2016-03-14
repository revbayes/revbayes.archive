////
////  GeneralRateGeneratorSequenceFunction.cpp
////  rb_mlandis
////
////  Created by Michael Landis on 4/3/14.
////  Copyright (c) 2014 Michael Landis. All rights reserved.
////
//
//#include "GeneralRateGeneratorSequenceFunction.h"
//#include "RateMatrix_JC.h"
//#include "RateGeneratorSequence.h"
//#include "ConstantNode.h"
//#include "RbException.h"
//
//using namespace RevBayesCore;
//
//GeneralRateGeneratorSequenceFunction::GeneralRateGeneratorSequenceFunction(size_t ns, size_t nc) : TypedFunction<RateGeneratorSequence>( new RateGeneratorSequence(ns, nc) )
//{
//    homogeneousRateMatrix               = new ConstantNode<RateGenerator>("homogeneousRateMatrix", new RateMatrix_JC(ns));
//    heterogeneousRateMatrices           = NULL;
//    homogeneousClockRate                = new ConstantNode<double>("clockRate", new double(1.0) );
//    heterogeneousClockRates             = NULL;
//    rootFrequencies                     = new ConstantNode<RbVector<double> >("rootFrequencies", new RbVector<double>(ns,1.0/ns));
//    
//    branchHeterogeneousClockRates       = false;
//    branchHeterogeneousRateMatrices    = false;
//    
//    this->addParameter(homogeneousRateMatrix);
//    this->addParameter(homogeneousClockRate);
//    this->addParameter(rootFrequencies);
//    
//    update();
//}
//
//
//GeneralRateGeneratorSequenceFunction::GeneralRateGeneratorSequenceFunction(const GeneralRateGeneratorSequenceFunction &n) : TypedFunction<RateGeneratorSequence>( n )
//{
//    homogeneousRateMatrix = n.homogeneousRateMatrix;
//    heterogeneousRateMatrices = n.heterogeneousRateMatrices;
//    homogeneousClockRate = n.homogeneousClockRate;
//    heterogeneousClockRates = n.heterogeneousClockRates;
//    rootFrequencies = n.rootFrequencies;
//    
//    branchHeterogeneousClockRates = n.branchHeterogeneousClockRates;
//    branchHeterogeneousRateMatrices = n.branchHeterogeneousRateMatrices;
//}
//
//
//GeneralRateGeneratorSequenceFunction::~GeneralRateGeneratorSequenceFunction( void ) {
//    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
//}
//
//
//
//GeneralRateGeneratorSequenceFunction* GeneralRateGeneratorSequenceFunction::clone( void ) const
//{
//    return new GeneralRateGeneratorSequenceFunction( *this );
//}
//
//
//void GeneralRateGeneratorSequenceFunction::update( void )
//{
//    
//    // set the gainLossRate
//    if (branchHeterogeneousRateMatrices)
//    {
//        const RbVector<RateGenerator>& rm = heterogeneousRateMatrices->getValue();
//        static_cast<RateGeneratorSequence*>(value)->setHeterogeneousRateMatrices(rm);
//    }
//    else
//    {
//        const RateGenerator& rm = homogeneousRateMatrix->getValue();
//        static_cast<RateGeneratorSequence*>(value)->setHomogeneousRateMatrix(&rm);
//    }
//    
//    if (branchHeterogeneousClockRates)
//    {
//        const std::vector<double>& r = heterogeneousClockRates->getValue();
//        static_cast< RateGeneratorSequence* >(value)->setHeterogeneousClockRates(r);
//    }
//    else
//    {
//        const double& r = homogeneousClockRate->getValue();
//        static_cast< RateGeneratorSequence* >(value)->setHomogeneousClockRate(r);
//    }
//    
//    value->updateMap();
//}
//
//void GeneralRateGeneratorSequenceFunction::setRateMatrix(const TypedDagNode<RateGenerator>* r)
//{
//    // remove the old parameter first
//    if ( homogeneousRateMatrix != NULL )
//    {
//        this->removeParameter( homogeneousRateMatrix );
//        homogeneousRateMatrix = NULL;
//    }
//    else
//    {
//        this->removeParameter( heterogeneousRateMatrices );
//        heterogeneousRateMatrices = NULL;
//    }
//    
//    // set the value
//    branchHeterogeneousRateMatrices = false;
//    homogeneousRateMatrix = r;
//    
//    // add the parameter
//    this->addParameter( homogeneousRateMatrix );
//}
//
//void GeneralRateGeneratorSequenceFunction::setClockRate(const TypedDagNode< double > *r) {
//    
//    // remove the old parameter first
//    if ( homogeneousClockRate != NULL )
//    {
//        this->removeParameter( homogeneousClockRate );
//        homogeneousClockRate = NULL;
//    }
//    else // heterogeneousClockRate != NULL
//    {
//        this->removeParameter( heterogeneousClockRates );
//        heterogeneousClockRates = NULL;
//    }
//    
//    // set the value
//    branchHeterogeneousClockRates = false;
//    homogeneousClockRate = r;
//    
//    // add the parameter
//    this->addParameter( homogeneousClockRate );
//}
//
//void GeneralRateGeneratorSequenceFunction::setClockRate(const TypedDagNode< RbVector< double > > *r) {
//    
//    // remove the old parameter first
//    if ( homogeneousClockRate != NULL )
//    {
//        this->removeParameter( homogeneousClockRate );
//        homogeneousClockRate = NULL;
//    }
//    else // heterogeneousClockRate != NULL
//    {
//        this->removeParameter( heterogeneousClockRates );
//        heterogeneousClockRates = NULL;
//    }
//    
//    // set the value
//    branchHeterogeneousClockRates = true;
//    heterogeneousClockRates = r;
//    
//    // add the parameter
//    this->addParameter( heterogeneousClockRates );
//    
//}
//
//void GeneralRateGeneratorSequenceFunction::setRootFrequencies(const TypedDagNode<RbVector<double> > *f)
//{
//    if (rootFrequencies != NULL)
//    {
//        this->removeParameter(rootFrequencies);
//        rootFrequencies = NULL;
//    }
//    
//    rootFrequencies = f;
//    this->addParameter(rootFrequencies);
//}
//
//void GeneralRateGeneratorSequenceFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
//{
//    if (oldP == homogeneousRateMatrix)
//    {
//        homogeneousRateMatrix = static_cast<const TypedDagNode<RateGenerator>* >( newP );
//    }
//    else if (oldP == heterogeneousRateMatrices)
//    {
//        heterogeneousRateMatrices = static_cast<const TypedDagNode<RbVector<RateGenerator> >* >( newP );
//    }
//    else if (oldP == homogeneousClockRate)
//    {
//        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
//    }
//    else if (oldP == heterogeneousClockRates)
//    {
//        heterogeneousClockRates = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
//    }
//    else if (oldP == rootFrequencies)
//    {
//        rootFrequencies = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
//    }
//}
//
////std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x)
////{
////    o << "";
////    return o;
////}
