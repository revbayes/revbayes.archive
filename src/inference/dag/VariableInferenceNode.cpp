//
//  VariableInferenceNode.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "VariableInferenceNode.h"


VariableInferenceNode::VariableInferenceNode( const RbValue<void*> &v, const std::string &n) : InferenceDagNode( v, n ), factorRoot( NULL ) {
    
}

VariableInferenceNode::VariableInferenceNode( const VariableInferenceNode &v) : InferenceDagNode( v ), factorRoot( v.factorRoot ) {
    
}

VariableInferenceNode::~VariableInferenceNode() {
    
}


void VariableInferenceNode::addParentNode(InferenceDagNode *p) {
    parents.insert( p );
}



StochasticInferenceNode* VariableInferenceNode::getFactorRoot( void ) const {
    return factorRoot;
}


/** 
 * Mark all our children for recalculation. 
 * The difference between this function and touch is that touch also results into a call of likelihood updates.
 * Here, we only want to recalculate the probability.
 */
void VariableInferenceNode::markChildrenForRecalculation( void ) {
    
    for (std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++) {
        (*i)->markForRecalculation();
    }
}


void VariableInferenceNode::setFactorRoot(StochasticInferenceNode *r) {
    factorRoot = r;
}
