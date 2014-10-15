//
//  LengthTree.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-21.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//LengthTree


#include "LengthTree.h"

#include <cmath>

using namespace RevBayesCore;

LengthTree::LengthTree(const TypedDagNode< TimeTree > *t, const TypedDagNode< RbVector<double> > *v) : TypedFunction< RbVector<double> >( new RbVector<double>(v->getValue().size(), 1.0) ), vals( v ), tree( t ) {
    // add the parameters as parents
    this->addParameter( vals );
    this->addParameter( tree );
    
    update();
}


LengthTree::LengthTree(const LengthTree& l) : TypedFunction< RbVector<double> >(l), vals(l.vals), tree(l.tree) {}


LengthTree* LengthTree::clone( void ) const {
    
    return new LengthTree( *this );
}


void LengthTree::update( void ) {

    // get the root
    const TopologyNode& root = tree->getValue().getRoot();
    recursiveUpdate(root);
}

void LengthTree::recursiveUpdate( const TopologyNode& from) {
    
    size_t index = from.getIndex();
    if (from.isRoot())  {
        (*value)[index] = 0;
    }
    else    {
        double v2 = (*vals).getValue()[index];
        double v1 = (*vals).getValue()[from.getParent().getIndex()];
        (*value)[index] = exp(0.5 * (v1 + v2));
    }
    
    size_t nchild = from.getNumberOfChildren();
    
    for (size_t i = 0; i < nchild; ++i) {
        const TopologyNode& child = from.getChild(i);
        recursiveUpdate(child);
    }
}


void LengthTree::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == vals )
    {
        vals = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}
