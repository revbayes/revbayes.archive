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

TreeScaleFunction::TreeScaleFunction(const TypedDagNode<TimeTree> *t, const TypedDagNode<double> *s, std::vector<double> m) : TypedFunction<TimeTree>( new TimeTree() ),
    tau( t ),
    scale( s ),
    tipAges( m ),
    scaleLimit(0.0)
{
    // add the lambda parameter as a parent
    addParameter( tau );
    addParameter( scale );
    
    // get scale limit from tip ages
    double maxAge = 0.0;
    for (size_t i = 0; i < tipAges.size(); i++)
    {
        if (tipAges[i] > maxAge)
            maxAge = tipAges[i];
    }
    scaleLimit = maxAge;
    
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
    
}


void TreeScaleFunction::update( void )
{
    (*value) = tau->getValue();
    
    const double &v = scale->getValue();
    
    // tip nodes have pre-set ages
    // NOTE: should be able to rescale with v* tree.getAge(i) under new setup...
    for (size_t i = 0; i < (*value).getNumberOfTips(); i++)
    {
        (*value).setAge(i, tipAges[i]);
    }    
    
    // internal node ages are scaled
    for (size_t i = (*value).getNumberOfTips(); i < (*value).getNumberOfNodes(); i++)
    {
        (*value).setAge(i, v * tau->getValue().getAge(i));
    }
    
//    for (size_t i = 0; i < tau->getValue().getNumberOfNodes(); i++) {
//        if ((*value).getBranchLength(i) < 0.0) {
//            std::cout << "TreeScale has negative brlen\n";
//        }
//    }
    
    // update newick string
    (*value).getRoot().flagNewickRecomputation();
    
}



void TreeScaleFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<TimeTree>* >( newP );
    }
    else if (oldP == scale)
    {
        scale = static_cast<const TypedDagNode<double>* >( newP );
    }
}


