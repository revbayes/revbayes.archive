/**
 * @file
 * This file contains the implementation of DeterministicNode, which is derived
 * from DAGNode. DeterministicNode is used for DAG nodes associated with an
 * expression (equation) that determines their value.
 *
 * @brief Implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "DeterministicNode.h"
#include "RbException.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"

/** Constructor from function; get arguments from the function object */
DeterministicNode::DeterministicNode(RbFunction* func)
    : DAGNode(), function(func) {

    std::vector<RbObjectWrapper*>& arguments = func->getProcessedArguments();
    for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
        parents.insert(*i);

    //arguments = function->processArguments(args);
//
//    value = function->execute(arguments);
}


/** Basic constructor */
DeterministicNode::DeterministicNode(RbFunction* func, const std::vector<Argument>& args)
    : DAGNode(), function(func) {

    //arguments = function->processArguments(args);
    //for (std::vector<DAGNode*>::iterator i=arguments.begin(); i!=arguments.end(); i++)
    //    parents.insert(*i);
//
//    value = function->execute(arguments);
}

/** Copy constructor */
DeterministicNode::DeterministicNode(const DeterministicNode& d)
    : DAGNode(d), function(d.function) {
}


/** Clone this object */
DeterministicNode* DeterministicNode::clone() const {

	return new DeterministicNode(*this);
}


/** Get class vector describing type of object */
const StringVector& DeterministicNode::getClass() const {

    static StringVector rbClass = StringVector(RbNames::DeterministicNode::name) + DAGNode::getClass();
    return rbClass;
}


/**
 * @brief Thouch affected nodes
 *
 * This function touches all affected DAG nodes, i.e. marks them as changed.
 *
 */
void DeterministicNode::touchAffectedChildren() {

//    if (!touched) {
        for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->touchAffectedChildren();
        	(*i)->touch();
        }
//    }
}

/**
 * @brief Thouch affected nodes
 *
 * This function touches all affected DAG nodes, i.e. marks them as changed.
 *
 */
void DeterministicNode::touchAffectedParents() {

//    if (!touched) {
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
            (*i)->touchAffectedParents();
        	(*i)->touch();
        }
//    }
}

/**
 * @brief Tell affected DAG nodes to keep current value
 *
 * This function calls all affected DAG nodes so that they
 * have a chance to keep the current value and discard the
 * previous value.
 *
 */
void DeterministicNode::keepAffectedChildren() {

    if (changed) {
        for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->keepAffectedChildren();
        	(*i)->keep();
        }
    }
}

/**
 * @brief Tell affected DAG nodes to keep current value
 *
 * This function calls all affected DAG nodes so that they
 * have a chance to keep the current value and discard the
 * previous value.
 *
 */
void DeterministicNode::keepAffectedParents() {

    if (changed) {
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
            (*i)->keepAffectedParents();
        	(*i)->keep();
        }
    }
}

/**
 * @brief Restore affected nodes
 *
 * This function calls all nodes that are affected by this DAG node and restores
 * them.
 *
 */
void DeterministicNode::restoreAffectedChildren() {

    for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->restoreAffectedChildren();
     	(*i)->restore();
    }
}

/**
 * @brief Restore affected nodes
 *
 * This function calls all nodes that are affected by this DAG node and restores
 * them.
 *
 */
void DeterministicNode::restoreAffectedParents() {

    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
        (*i)->restoreAffectedParents();
       	(*i)->restore();
    }
}


bool DeterministicNode::equals(const RbObject* obj) const {
	return false;
}

double DeterministicNode::getLnProbabilityRatio() {
	double lnProb = 0.0;
	for (std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++) {
		lnProb += (*i)->getLnProbabilityRatio();
	}
	return lnProb;
}

double DeterministicNode::getLnProbability() {
	if (touchedProbability == true) { 
		double lnProb = 0.0;
		for (std::set<DAGNode*>::iterator i = children.begin(); i != children.end(); i++) {
			lnProb += (*i)->getLnProbability();
		}
		currentProbability = lnProb;
		touchedProbability = false;
	}
	return currentProbability;
}


/** Get value intelligently */
const RbObject* DeterministicNode::getValue() {

    if (isTouched() && !isChanged()) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        value = function->execute();
        changed = true;
    }

    return value;
}


/**
 * @brief Print function
 *
 * This function prints complete info about the  object.
 *
 * @param o           The ostream for printing
 *
 */
void DeterministicNode::printValue(std::ostream &o) const {

    o << value->toString() << std::endl;
}

std::string DeterministicNode::toString(void) const {

    return value->toString();
}


