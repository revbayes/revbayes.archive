/**
 * @file
 * This file contains the implementation of VariableNode, which is the base
 * class for variable nodes in the model DAG.
 *
 * @brief Implementation of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-13 15:50:43 +0100 (Sön, 13 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.cpp 188 2009-12-13 14:50:43Z ronquist $
 */

#include "Monitor.h"
#include "Move.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>


/** Constructor */
VariableNode::VariableNode( void ) : DAGNode( ), touched( false ) {
}

/** Copy Constructor */
VariableNode::VariableNode(const VariableNode &v) : DAGNode(v) {
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


