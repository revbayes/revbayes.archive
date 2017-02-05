//
//  AdjacentRateModifierFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/3/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "AdjacentRateModifierFunction.h"
#include "AdjacentRateModifier.h"
#include "TypedDagNode.h"
#include <cmath>

using namespace RevBayesCore;

AdjacentRateModifierFunction::AdjacentRateModifierFunction(const TypedDagNode<double>* f, const TypedDagNode<int>* w, size_t ns, size_t nc) : TypedFunction<CharacterHistoryRateModifier>( new AdjacentRateModifier(ns, nc) ),
    factor(f),
    width(w)
{
    // add the parameters as parents
    addParameter(factor);
    addParameter(width);
    
    update();
}

AdjacentRateModifierFunction::AdjacentRateModifierFunction(const AdjacentRateModifierFunction& m) : TypedFunction<CharacterHistoryRateModifier>( m )
{
    factor = m.factor;
    width = m.width;
}


AdjacentRateModifierFunction::~AdjacentRateModifierFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}





AdjacentRateModifierFunction* AdjacentRateModifierFunction::clone( void ) const
{
    return new AdjacentRateModifierFunction( *this );
}


void AdjacentRateModifierFunction::update( void )
{
    
    double f = factor->getValue();
    size_t w = width->getValue();
    
    static_cast<AdjacentRateModifier*>(value)->setFactor(f);
    static_cast<AdjacentRateModifier*>(value)->setWidth(w);
}



void AdjacentRateModifierFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == factor)
    {
        factor = static_cast<const TypedDagNode<double>* >( newP );
        std::cout << "ARMF::this " << this << "\n";
        std::cout << "ARMF::factor " << oldP << " -> " << newP << "\n";
    }
    else if (oldP == width)
    {
        width = static_cast<const TypedDagNode<int>* >( newP );
    }
}
