/**
 * @file
 * This file contains the implementation of VariableNode, which is the base
 * class for variable nodes in the model DAG.
 *
 * @brief Implementation of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id$
 */

#include "Monitor.h"
#include "Move.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "VariableNode.h"

#include <algorithm>


VariableNode::VariableNode( void ) : DAGNode(  ), touched( false ), factorRoot( NULL ) {
}


VariableNode::VariableNode(const VariableNode &v) : DAGNode(v), factorRoot( v.factorRoot ) {
    touched = false;
}


/** Destructor */
VariableNode::~VariableNode( void ) {


    /* Remove this node as a child node of parents and delete these if appropriate */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ ) {
        DAGNode* node = *i;
        node->removeChildNode( this );
    }
}

/** add a child node */
void VariableNode::addParentNode( DAGNode* p) {
    PRINTF("Adding parent with name \"%s\" to child with name \"%s\".\n",p->getName().c_str(),getName().c_str());

    // retain and add the parent
    parents.insert(p);
}


/** Get class name of object */
const std::string& VariableNode::getClassName(void) { 
    
    static std::string rbClassName = "Variable DAG node";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VariableNode::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( DAGNode::getClassTypeSpec() ) );
    
	return rbClass; 
}


/**
 * Get the factor root. The factor root is the node which starts the likelihood elimination for the eliminated subgraph.
 */
StochasticNode* VariableNode::getFactorRoot(void) const {
    return factorRoot;
}


/** 
 * Mark all our children for recalculation. 
 * The difference between this function and touch is that touch also results into a call of likelihood updates.
 * Here, we only want to recalculate the probability.
 */
void VariableNode::markChildrenForRecalculation( void ) {
    
    for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
        (*i)->markForRecalculation();
    }
}


/**
 * Set the factor root. The factor root is the node which starts the likelihood elimination for the eliminated subgraph.
 */
void VariableNode::setFactorRoot(StochasticNode* n)  {
    
    // if we get a new factor root, we might want to tell that our children (only if we are eliminated ourselves)
    if (factorRoot != n) {
        if ( isNotInstantiated()) {
            for (std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++) {
                (*i)->setFactorRoot( n );
            }
        }
    }
    
    // finally set the factor root
    factorRoot = n;
}
