//
//  InferenceDag.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceDagNode.h"
#include "VariableInferenceNode.h"


InferenceDagNode::InferenceDagNode(const RbValue<void*> &v) : value( v ) {
    
}


InferenceDagNode::InferenceDagNode(const InferenceDagNode &d) : value( d.value ) {
    
}

InferenceDagNode::~InferenceDagNode() {
    
}


void InferenceDagNode::addChildNode(VariableInferenceNode *c) {
    children.insert( c );
}


/**
 * Get all affected nodes this DAGNode.
 * This means we call getAffected() of all children. getAffected() is pure virtual.
 */
void InferenceDagNode::getAffectedNodes(std::set<StochasticInferenceNode* > &affected) {
    
    // get all my affected children
    for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->getAffected(affected);
}


const std::set<InferenceDagNode*>& InferenceDagNode::getParents( void ) const {
    return parents;
}


/**
 * Keep the value of the node.
 * This function delegates the call to keepMe() and calls keepAffected() too.
 */
void InferenceDagNode::keep(void) {
    // keep myself first
    keepMe();
    
    // next, keep all my children
    keepAffected();
}

/**
 * Tell affected variable nodes to keep the current value.
 */
void InferenceDagNode::keepAffected() {
    
    // keep all my children
    for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->keepMe();
}


const RbValue<void*>& InferenceDagNode::getValue( void ) const {
    return value;
}


/**
 * Restore this DAGNode.
 * This means we call restoreMe() and restoreAffected(). There is a default implementation of restoreAffected()
 * which call restoreMe() of all children of this node. restoreMe() is pure virtual.
 */
void InferenceDagNode::restore(void) {
    // first restore myself
    restoreMe();
    
    // next, restore all my children
    restoreAffected();
}


/**
 * Restore all affected nodes this DAGNode.
 * This means we call restoreMe() of all children. restoreMe() is pure virtual.
 */
void InferenceDagNode::restoreAffected(void) {
    
    // next, restore all my children
    for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->restoreMe();
}


/**
 * Touch the DAG node.
 *
 * This function should be called if the value of the variable has changed or if you want this node to be reevaluated. 
 * The function will automatically call the touchMe() which is implemented differently in the different DAG node types.
 *
 * Since the DAG node was touch and possibly changed, we tell affected variable nodes to keep the current value.
 */
void InferenceDagNode::touch() {
    // first touch myself
    touchMe();
    
    // next, touch all my children
    touchAffected();
}


/**
 * Tell affected variable nodes to touch themselves (i.e. that they've been touched).
 */
void InferenceDagNode::touchAffected() {
    
    // touch all my children
    for ( std::set<VariableInferenceNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchMe();
}



