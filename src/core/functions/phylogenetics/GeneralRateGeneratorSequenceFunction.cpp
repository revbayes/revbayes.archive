//
//  GeneralRateGeneratorSequenceFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "GeneralRateGeneratorSequenceFunction.h"

#include "ConstantNode.h"
#include "RateGeneratorSequenceUsingMatrix.h"
#include "RateMatrix_JC.h"
#include "RbException.h"

using namespace RevBayesCore;

GeneralRateGeneratorSequenceFunction::GeneralRateGeneratorSequenceFunction(size_t ns, size_t nc)
    : TypedFunction<RateGeneratorSequence>( new RateGeneratorSequenceUsingMatrix(ns, nc) )
{
    homogeneousRateMatrix = new ConstantNode<RateGenerator>("homogeneousRateMatrix", new RateMatrix_JC(ns));
    
    this->addParameter(homogeneousRateMatrix);
    
    update();
}


GeneralRateGeneratorSequenceFunction::GeneralRateGeneratorSequenceFunction(const GeneralRateGeneratorSequenceFunction &n) : TypedFunction<RateGeneratorSequence>( n )
{
    homogeneousRateMatrix = n.homogeneousRateMatrix;
}


GeneralRateGeneratorSequenceFunction::~GeneralRateGeneratorSequenceFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GeneralRateGeneratorSequenceFunction* GeneralRateGeneratorSequenceFunction::clone( void ) const
{
    return new GeneralRateGeneratorSequenceFunction( *this );
}


void GeneralRateGeneratorSequenceFunction::update( void )
{

    const RateGenerator& rm = homogeneousRateMatrix->getValue();
//    static_cast<RateGeneratorSequence*>(value)->setHomogeneousRateMatrix(&rm);

    value->updateMap();
}

void GeneralRateGeneratorSequenceFunction::setRateMatrix(const TypedDagNode<RateGenerator>* r)
{
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL )
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }

    // set the value
    homogeneousRateMatrix = r;

    // add the parameter
    this->addParameter( homogeneousRateMatrix );
}

void GeneralRateGeneratorSequenceFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == homogeneousRateMatrix)
    {
        homogeneousRateMatrix = static_cast<const TypedDagNode<RateGenerator>* >( newP );
    }
}

//std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double> >& x)
//{
//    o << "";
//    return o;
//}
