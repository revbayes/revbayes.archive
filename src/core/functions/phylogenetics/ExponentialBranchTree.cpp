//
//  ExponentialBranchTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-24.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "ExponentialBranchTree.h"


using namespace RevBayesCore;


// constructor(s)
ExponentialBranchTree::ExponentialBranchTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< std::vector<double> >* n): TypedFunction< std::vector< double > >( new std::vector< double >(t->getValue().getNumberOfNodes() -1, 0.0 ) ), tau(t),
nodeval( n ) {
    this->addParameter( tau );
    this->addParameter( nodeval );
    
    update();
}


ExponentialBranchTree::ExponentialBranchTree(const ExponentialBranchTree &n): TypedFunction< std::vector< double > >( n ), tau( n.tau ), nodeval( n.nodeval ) {
    
}


ExponentialBranchTree* ExponentialBranchTree::clone(void) const {
    return new ExponentialBranchTree( *this );
}


void ExponentialBranchTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == tau ) {
        tau = static_cast< const TypedDagNode<TimeTree> * >( newP );
    }
    
    if ( oldP == nodeval ) {
        nodeval = static_cast< const TypedDagNode<std::vector<double> >* >( newP );
    }
}

void ExponentialBranchTree::update(void)    {
    
    // get the root
    const TopologyNode& root = tau->getValue().getRoot();
    recursiveUpdate(root);
    
}

void ExponentialBranchTree::recursiveUpdate(const RevBayesCore::TopologyNode &from)    {

    size_t index = from.getIndex();

    if (! from.isRoot())    {
        // get the index
        
        size_t upindex = from.getParent().getIndex();

        double x1 = nodeval->getValue()[index];
        double x2 = nodeval->getValue()[upindex];

        double y = 0.5 * (exp(x1) + exp(x2));
        
        // we store this val here
        (*value)[index] = y;
    }
    
    // simulate the val for each child (if any)
    size_t numChildren = from.getNumberOfChildren();
    for (size_t i = 0; i < numChildren; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}

