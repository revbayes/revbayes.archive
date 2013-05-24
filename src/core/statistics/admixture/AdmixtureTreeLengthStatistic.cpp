//
//  AdmixtureTreeLengthStatistic.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/16/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureTreeLengthStatistic.h"
#include "AdmixtureTree.h"
#include "AdmixtureNode.h"

using namespace RevBayesCore;

AdmixtureTreeLengthStatistic::AdmixtureTreeLengthStatistic(const TypedDagNode<AdmixtureTree> *t) : TypedFunction<double>( new double(0.0) ), tree( t ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    update();
}

AdmixtureTreeLengthStatistic::AdmixtureTreeLengthStatistic(const AdmixtureTreeLengthStatistic &n) : TypedFunction<double>( n ), tree( n.tree ) {
    // no need to add parameters, happens automatically
}


AdmixtureTreeLengthStatistic* AdmixtureTreeLengthStatistic::clone( void ) const {
    return new AdmixtureTreeLengthStatistic( *this );
}

void AdmixtureTreeLengthStatistic::update( void ) {
    
    //double treeHeight = tree->getValue().getRoot().getAge();
    double treeLength = 0.0;
    
    for (size_t i = 0; i < tree->getValue().getNumberOfNodes(); i++)
    {
        treeLength += tree->getValue().getNode(i).getBranchLength();
    }
    
    *value = treeLength;
}


void AdmixtureTreeLengthStatistic::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<AdmixtureTree>* >( newP );
    }
}
