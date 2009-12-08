/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG.
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
#include "RbMonitor.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"
#include "RbObject.h"

#include <iostream>


/**
 * @brief DAGNode default constructor
 *
 * This constructor creates an empty DAGNode.
 *
 */
DAGNode::DAGNode()
    : storedValue(NULL), value(NULL), changed(false), 
      children(), parents() {
      moves = NULL;
      touchedProbability = true;
      touchedLikelihood = true;
}


/**
 * @brief DAGNode constructor from value
 *
 * This constructor creates a DAGNode with a variable attached to the node.
 *
 * @param val   The value of the DAG node
 *
 */
DAGNode::DAGNode(RbObject *val)
    : storedValue(NULL), value(val), changed(false), 
      children(), parents() {
      moves = NULL;
      touchedProbability = true;
      touchedLikelihood = true;
}


/**
 * @brief DAGNode copy constructor
 *
 * This constructor creates a DAGNode as a copy of another
 * DAGNode.
 *
 * @param d     The DAG node to clone.
 *
 */
DAGNode::DAGNode(const DAGNode &d)
    :   storedValue(d.storedValue->clone()), value(d.value->clone()),
        changed(d.changed), 
        children(), parents() {
        
      touchedProbability = d.touchedProbability;
      touchedLikelihood = d.touchedLikelihood;

    for (std::set<DAGNode*>::iterator i=d.children.begin(); i!=d.children.end(); i++)
            children.insert((DAGNode*)(*i)->clone());

    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
            parents.insert((DAGNode*)(*i)->clone());
            
    (*moves) = (*d.moves);
}


/**
 * @brief DAGNode destructor
 *
 * This is the standard destructor for all DAG nodes.
 *
 */
DAGNode::~DAGNode(void) {

	if ( value != NULL )
		delete value;
	if ( storedValue != NULL )
		delete storedValue;
}

void DAGNode::accept() {
//std::cerr << "accept" << std::endl;
    // keep new value
    keep();
    // keep the affected parents
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
    	(*i)->keepAffectedParents();
    	(*i)->keep();
    }
    // keep the affected children
    for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
    	(*i)->keepAffectedChildren();
    	(*i)->keep();
    }
	
	// call accept for the move
	lastMove->acceptMove();
}

void DAGNode::addMonitor(RbMonitor* m) {
	monitors.insert(m);
}

void DAGNode::addMove(RbMove* m, double w) {
	if (moves != NULL) {
		moves->addMove(m,w);
	}
}


/**
 * @brief Compare DAG nodes
 *
 * To be the same, the DAG nodes must have the same parents and children,
 * as well as the same value and storedValue. It is the pointers to these
 * objects we need to compare, not their current values.
 *
 */
bool DAGNode::equals(const RbObjectWrapper* x) const {

    const DAGNode* d = dynamic_cast<const DAGNode*>(x);

    if (d == NULL)
        return false;

	if (value != d->value || storedValue != d->storedValue)
        return false;

    if (changed != d->changed || touchedProbability != d->touchedProbability || touchedLikelihood != d->touchedLikelihood)
        return false;

    if (children.size() != d->children.size() || parents.size() != d->parents.size())
        return false;

    for (std::set<DAGNode*>::iterator i=d->children.begin(); i!=d->children.end(); i++)
        if (children.find(*i) == children.end())
            return false;

    for (std::set<DAGNode*>::iterator i=d->parents.begin(); i!=d->parents.end(); i++)
        if (parents.find(*i) == parents.end())
            return false;

    return true;
}


/** Get class vector describing type of object */
const StringVector& DAGNode::getClass() const {

    static StringVector rbClass = StringVector(RbNames::DAGNode::name) + RbObjectWrapper::getClass();
    return rbClass;
}


double DAGNode::getLnLikelihood(void) {
	if (touchedLikelihood == true) {
	    double lnLikelihood = 0.0;
	    for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
	        lnLikelihood += (*i)->getLnProbability();
	    }
	    currentLikelihood = lnLikelihood;
	    touchedLikelihood = false;
	}
    return currentLikelihood;
}

double DAGNode::getLnLikelihoodRatio(void) {
    currentLikelihood = getLnLikelihood();
//std::cerr << "current likelihood = " << currentLikelihood << std::endl;
//std::cerr << "stored likelihood = " << storedLikelihood << std::endl;
    return currentLikelihood - storedLikelihood;
}

double DAGNode::getLnPriorRatio(void) {
    return getLnProbabilityRatio();
}

RbMove* DAGNode::getNextMove(void) {
	if (moves == NULL) {
		return NULL;
	}
	lastMove = moves->getNext();
    return lastMove;
}

double DAGNode::getUpdateWeight(void) {
	if (moves == NULL) 
		return 0.0;
	return moves->getUpdateWeight();
}

double DAGNode::performMove(void) {
	if (moves == NULL) {
		return 0.0;
	}	
	store();
	RbMove* m = getNextMove();
	double hr = m->performMove();
	
	// mark this node as changed for recalculations
	changed = true;
	touch();
	
	// propagate the change to the children
    // touch the affected parents
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
    	(*i)->touchAffectedParents();
    	(*i)->touch();
    }
    // touch the affected children
    for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
    	(*i)->touchAffectedChildren();
    	(*i)->touch();
    }
	
	return hr;
}

void DAGNode::monitor(int i) {
	if (!monitors.empty()) {
    	for (std::set<RbMonitor*>::iterator it=monitors.begin(); it!= monitors.end(); it++){
    	    (*it)->monitor(i);
    	}
    }
}

/**
 * @brief Print children
 *
 * This function prints the children DAG nodes to an ostream with a header
 * for each child.
 *
 * @param o     The ostream for printing.
 *
 */
void DAGNode::printChildren(std::ostream& o) const {

	if ( children.empty() )	
		{
		o << "No children" << std::endl;
		return;
		}

    int count = 1;
	for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++, count++) {
		o << "children[" << count << "]:" << std::endl;
        o << (*i) << std::endl;
    }

	o << std::endl;
}


/**
 * @brief Print parents
 *
 * This function prints the parent DAG nodes to an ostream with a header
 * for each parent.
 *
 * @param o     The ostream for printing.
 *
 */
void DAGNode::printParents(std::ostream& o) const {

	if ( parents.empty() )
		{
		o << "No parents" << std::endl;
		return;
		}

    int count = 1;
	for (std::set<DAGNode*>::iterator i=parents.begin(); i != parents.end(); i++, count++) {
		o << "parents[" << count << "]:" << std::endl;
        o << (*i) << std::endl;
    }

	o << std::endl;
}


void DAGNode::reject() {
//std::cerr << "reject" << std::endl;
    // restore new value
    restore();
    
    // restore the affected parents
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
    	(*i)->restoreAffectedParents();
    	(*i)->restore();
    }
    
    // restore the affected children
    for (std::set<DAGNode*>::iterator i=children.begin(); i!=children.end(); i++) {
    	(*i)->restoreAffectedChildren();
    	(*i)->restore();
    }
	
	// call accept for the move
	lastMove->rejectMove();
}

/**
 * @brief Restore the old value
 *
 * This function restores the old value of the DAG node. It is typically called
 * during MCMC sampling when a proposed value is rejected.
 *
 */
void DAGNode::restore() {

    RbObject* temp;

    if (changed) {
        temp        = value;
        value       = storedValue;
        storedValue = temp;
    }
//    if (touched) {
        currentLikelihood = storedLikelihood;
        currentProbability = storedProbability;    
//    }

    
    keep();     // Sets touched and changed to false
}


/**
 * @brief Set value
 *
 * This function sets the value of the node.
 *
 */
void DAGNode::setValue(RbObject* val) {

    if (value != NULL)
        delete value;

    value = val;
}

void DAGNode::store(void) {
    *storedValue = *value;
    storedLikelihood = currentLikelihood;
    storedProbability = currentProbability;
}

/**
 * @brief Prdouble value
 *
 * Prints value for user.
 *
 * @param o     The stream for printing
 *
 */
void DAGNode::printValue(std::ostream &o) const {

    o << value->toString() << std::endl;
}

