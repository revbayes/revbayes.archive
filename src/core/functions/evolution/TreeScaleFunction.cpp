//
//  TreeScaleFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/6/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "TreeScaleFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

TreeScaleFunction::TreeScaleFunction(const TypedDagNode<TimeTree> *t, const TypedDagNode<double> *s) : TypedFunction<TimeTree>( new TimeTree() ), tau( t ), scale( s ) {
    // add the lambda parameter as a parent
    addParameter( tau );
    addParameter( scale );
    
//    value->setTopology( &(tau->getValue()), false );
    *value = tau->getValue();
    
    update();
}


TreeScaleFunction::~TreeScaleFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



TreeScaleFunction* TreeScaleFunction::clone( void ) const
{
    return new TreeScaleFunction( *this );
}


void TreeScaleFunction::keep(DagNode *affecter)
{
    //delegate to base class
    TypedFunction< TimeTree >::keep( affecter );

}


void TreeScaleFunction::reInitialized( void )
{
    *value = tau->getValue();
}


void TreeScaleFunction::restore(DagNode *restorer)
{
    //delegate to base class
    TypedFunction< TimeTree >::restore( restorer );
}


void TreeScaleFunction::touch(DagNode *toucher)
{
    
    //delegate to base class
    TypedFunction< TimeTree >::touch( toucher );
    
    if ( toucher == scale )
    {
        const std::set<size_t> &touchedIndices = toucher->getTouchedElementIndices();
        touchedNodeIndices.insert(touchedIndices.begin(), touchedIndices.end());
    }
    
}


void TreeScaleFunction::update( void )
{
    TimeTree tree = tau->getValue();
    const double &v = scale->getValue();
    for (size_t i = 0; i < tree.getNumberOfNodes(); i++)
    {
        tree.setAge(i, v * tree.getAge(i));
    }
    *value = tree;
}



void TreeScaleFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<TimeTree>* >( newP );
        *value = tau->getValue();
    }
    else if (oldP == scale)
    {
        scale = static_cast<const TypedDagNode<double>* >( newP );
    }
}


