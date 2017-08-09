//
//  PhylogeneticDistanceRateModifierFunction.cpp
//  revbayes-branch-proj
//
//  Created by Michael Landis on 2/24/17.
//  Copyright © 2017 Michael Landis. All rights reserved.
//

#include "PhylogeneticDistanceRateModifierFunction.h"
#include "PhylogeneticDistanceRateModifier.h"
#include "TypedDagNode.h"
#include <cmath>

using namespace RevBayesCore;

PhylogeneticDistanceRateModifierFunction::PhylogeneticDistanceRateModifierFunction(const TypedDagNode<Tree>* t,
                                                                                   const TypedDagNode<double>* s) :
TypedFunction<CharacterHistoryRateModifier>( new PhylogeneticDistanceRateModifier(0, t->getValue().getNumberOfNodes()-1 ) ),
tau(t),
scale(s)
{
    // add the parameters as parents
    addParameter(tau);
    addParameter(scale);
    
    update();
}

PhylogeneticDistanceRateModifierFunction::PhylogeneticDistanceRateModifierFunction(const PhylogeneticDistanceRateModifierFunction& m) : TypedFunction<CharacterHistoryRateModifier>( m )
{
    tau  = m.tau;
    scale = m.scale;
}


PhylogeneticDistanceRateModifierFunction::~PhylogeneticDistanceRateModifierFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


PhylogeneticDistanceRateModifierFunction* PhylogeneticDistanceRateModifierFunction::clone( void ) const
{
    return new PhylogeneticDistanceRateModifierFunction( *this );
}


void PhylogeneticDistanceRateModifierFunction::update( void )
{
    
    // get values to update
    const Tree& t = tau->getValue();
    const double& s = scale->getValue();
    
    // apply updates to PhylogeneticDistanceRateModifier
    static_cast<PhylogeneticDistanceRateModifier*>(value)->setTree(t);
    static_cast<PhylogeneticDistanceRateModifier*>(value)->setScale(s);

    
}



void PhylogeneticDistanceRateModifierFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
    }
    else if (oldP == scale)
    {
        scale = static_cast<const TypedDagNode<double>* >( newP );
    }
}
