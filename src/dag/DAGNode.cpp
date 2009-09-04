/*!
 * \file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG as well as for the nodes in a
 * syntax tree.
 *
 * \brief Implementation of DAGNode
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbDataType.h"
#include <iostream>

using namespace std;

DAGNode::DAGNode(RbDataType *dt)
    : touched(true) {

	value = dt;
	storedValue = dt->copy();
}

DAGNode::DAGNode(DAGNode &d) {

	delete value;
	delete storedValue;
}

DAGNode::~DAGNode(void) {

	if ( value != NULL )
		delete value;
	if ( storedValue != NULL )
		delete storedValue;
}

void DAGNode::keepAffected() {

    if (changed) {
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->keepAffected();
        keep();
    }
}

void DAGNode::printChildren(void) const {

	if ( children.empty() )	
		{
		std::cout << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator c=children.begin(); c != children.end(); c++)
		std::cout << (*c) << " ";
	std::cout << std::endl;
}

void DAGNode::printParents(void) const {

	if ( parents.empty() )
		{
		std::cout << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator p=parents.begin(); p != parents.end(); p++)
		std::cout << (*p) << " ";
	std::cout << std::endl;
}

void DAGNode::restore() {

    RbDataType* temp;
    
    if (changed) {
        temp        = value;
        value       = storedValue;
        storedValue = temp;
    }

    keep();     // Sets touched and changed to false
}

void DAGNode::restoreAffected() {

    if (changed) {
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->restoreAffected();
        restore();
    }
}

void DAGNode::touchAffected() {

    if (!touched) {
        touch();
        for (set<DAGNode*>::iterator c=children.begin(); c!=children.end(); c++)
            (*c)->touchAffected();
    }
}
