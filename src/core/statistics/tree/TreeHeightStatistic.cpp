//
//  TreeHeightStatistic.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TreeHeightStatistic.h"

using namespace RevBayesCore;

TreeHeightStatistic::TreeHeightStatistic(const TypedDagNode<TimeTree> *t) : TypedFunction<double>( new double(0.0) ), 
    tree( t ) 
{
    // add the tree parameter as a parent
    addParameter( tree );
    
    update();
}


TreeHeightStatistic::TreeHeightStatistic(const TreeHeightStatistic &n) : TypedFunction<double>( n ), 
tree( n.tree ) 
{
    // no need to add parameters, happens automatically
}


TreeHeightStatistic::~TreeHeightStatistic( void ) 
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



TreeHeightStatistic* TreeHeightStatistic::clone( void ) const 
{
    
    return new TreeHeightStatistic( *this );
}


void TreeHeightStatistic::update( void ) 
{
     
    const TopologyNode& r = tree->getValue().getRoot();
    *value = r.getAge();
}



void TreeHeightStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == tree) 
    {
        tree = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    
}


