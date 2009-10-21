/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG as well as for the nodes in a
 * syntax tree.
 *
 * @brief Implementation of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface Distribution
 * @extends RbObject
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbDataType.h"
#include <iostream>

using namespace std;

/**
 * @brief DAGNode constructor
 *
 * This constructor creates a DAGNode with a variable attached to the node.
 *
 * @param dt      the variable of the dag node
 */
DAGNode::DAGNode(RbDataType *dt)
    : touched(true) {

	value = dt;
	storedValue = dt->copy();
}

/**
 * @brief DAGNode copy constructor
 *
 * This constructor creates a DAGNode as a clone of another DAGNode.
 *
 * @param d      the dag node to clone.
 */
DAGNode::DAGNode(DAGNode &d) {

	//TODO implement this
}

/**
 * @brief DAGNode desctructor
 *
 * This is the standard destructor for any dag node.
 *
 */
DAGNode::~DAGNode(void) {

	if ( value != NULL )
		delete value;
	if ( storedValue != NULL )
		delete storedValue;
}

//TODO not sure what this does
void DAGNode::keepAffected() {

    if (changed) {
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->keepAffected();
        keep();
    }
}

void DAGNode::printChildren(std::ostream& o) const {

	if ( children.empty() )	
		{
		o << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator c=children.begin(); c != children.end(); c++)
		o << (*c) << " ";

	o << std::endl;
}

void DAGNode::printParents(std::ostream& o) const {

	if ( parents.empty() )
		{
		o << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator p=parents.begin(); p != parents.end(); p++)
		o << (*p) << " ";

	o << std::endl;
}

/**
 * @brief Restore the old value
 *
 * This function restores the old value of the dag node. This might be wanted during the MCMC if a value was proposed but rejected.
 *
 */
void DAGNode::restore() {

    RbDataType* temp;
    
    if (changed) {
        temp        = value;
        value       = storedValue;
        storedValue = temp;
    }

    keep();     // Sets touched and changed to false
}

/**
 * @brief Restore affected nodes.
 *
 * This function calls all nodes which are affected by this dag node and restores them.
 *
 */
void DAGNode::restoreAffected() {

    if (changed) {
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->restoreAffected();
        restore();
    }
}

/**
 * @brief Thouch affected nodes.
 *
 * This function touches all affected nodes, i.e. marks them as changed.
 *
 */
void DAGNode::touchAffected() {

    if (!touched) {
        touch();
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->touchAffected();
    }
}
