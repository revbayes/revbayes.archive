//
//  TreeAdmixtureEventCount.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/28/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "TreeAdmixtureEventCount.h"


using namespace RevBayesCore;

TreeAdmixtureEventCount::TreeAdmixtureEventCount(const TypedDagNode<AdmixtureTree> *t) : TypedFunction<size_t>( new size_t(0) ), tree( t ) {
    // add the tree parameter as a parent
    addParameter( tree );
    
    //update();
}


TreeAdmixtureEventCount::TreeAdmixtureEventCount(const TreeAdmixtureEventCount &n) : TypedFunction<size_t>( n ), tree( n.tree ) {
    *value = n.tree->getValue().getNumberOfAdmixtureChildren();
    
    // no need to add parameters, happens automatically
}


TreeAdmixtureEventCount::~TreeAdmixtureEventCount( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



TreeAdmixtureEventCount* TreeAdmixtureEventCount::clone( void ) const {
    return new TreeAdmixtureEventCount( *this );
}


void TreeAdmixtureEventCount::update( void ) {
    
    //std::cout << "TAEC update\n";
//    *value = tree->getValue().getNumberOfNodes();
    if (tree->getValue().getNumberOfAdmixtureParents() != tree->getValue().getNumberOfAdmixtureChildren())
        std::cout << "what!\t" << tree->getValue().getNumberOfAdmixtureParents() << "\t" << tree->getValue().getNumberOfAdmixtureChildren() << "\n";
    *value = tree->getValue().getNumberOfAdmixtureChildren();
}



void TreeAdmixtureEventCount::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == tree) {
        tree = static_cast<const TypedDagNode<AdmixtureTree>* >( newP );
    }
}
