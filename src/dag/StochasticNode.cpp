/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated probability density function.
 *
 * @brief Implementation of StochasticNode
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

#include "Distribution.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"

/** @note MemberTable is a typedef for std::map<std::string, DAGNode*> defined in RbComplex.h. */

/** Constructor from distribution */
StochasticNode::StochasticNode(Distribution* dist)
    : VariableNode(dist->getReturnType()) {

    /* Get distribution parameters */
    const MemberTable& params = distribution->getMembers();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for (MemberTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        if ((*i).second->isParentInDAG(this, done))
            throw RbException("Invalid assignment: Cycles in the DAG");
    }

    /* Set parent(s) and add myself as a child to these */
    for (MemberTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        parents.insert((*i).second);
        (*i).second->addChildNode(this);
    }

    clamped      = false;

    distribution = dist;

    value        = distribution->rv();
    storedValue  = value->clone();
}


/** Copy constructor */
StochasticNode::StochasticNode(const StochasticNode& x)
    : VariableNode(x) {

    clamped = x.clamped;

    distribution = (Distribution*)(x.distribution->clone());

    value = x.value->clone();
    storedValue = x.storedValue->clone();  
}


/** Destructor */
StochasticNode::~StochasticNode() {

    delete distribution;
    delete value;
    if (storedValue != NULL)
        delete storedValue;
}


/** Assignment operator */
StochasticNode& StochasticNode::operator=(const StochasticNode& x) {

    if (this != &x) {

        VariableNode::operator=(x);

        clamped = x.clamped;

        distribution = (Distribution*)(x.distribution->clone());

        value = x.value->clone();
        storedValue = x.storedValue->clone();  
    }

    return (*this);
}


/** Calculate the conditional probability of the node, do not rely on stored values */
double StochasticNode::calculateLnProbability(void) {

	return (distribution->lnPdf(value));
}



/** Clamp the node to an observed value */
void StochasticNode::clamp(RbObject* observedVal) {

    if (storedValue != NULL) {
        delete storedValue;
        storedValue = NULL;
    }

    value = observedVal;
    clamped = true;
    touched = false;
}


/** Clone this object */
StochasticNode* StochasticNode::clone(void) const {

    return new StochasticNode(*this);
}


/** Get affected nodes: insert this node but stop recursion here */
void StochasticNode::getAffected(std::set<StochasticNode*>& affected) {

    affected.insert(this);
}


/** Get class vector describing type of object */
const StringVector& StochasticNode::getClass() const {

    static StringVector rbClass = StringVector(StochasticNode_name) + VariableNode::getClass();
    return rbClass;
}


/** Get the ln likelihood ratio of this node: delegate to distribution */
double StochasticNode::getLnLikelihoodRatio(void) {

    if (touched) {
        double lnLikelihood = 0.0;
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            lnLikelihood += (*i)->getLnPriorRatio();
    }
    else
        return 0.0;
}


/** Get the ln prior ratio of this node: delegate to distribution if touched */
double StochasticNode::getLnPriorRatio(void) {

    return distribution->lnPdfRatio(value, storedValue);
}


/** Get stored value */
const RbObject* StochasticNode::getStoredValue(void) {

    return storedValue;
}


/** Get value element */
const RbObject* StochasticNode::getValElement(const IntVector& index) const {

    if (value->getDim() == 0)
        throw RbException("Object does not have elements");

    return ((RbComplex*)(value))->getElement(index);
}


/** Get value */
const RbObject* StochasticNode::getValue(void) {

    return value;
}


/** Get value (const) */
const RbObject* StochasticNode::getValue(void) const {

    return value;
}


/** Get value pointer for move; tell and get affected */
RbObject* StochasticNode::getValuePtr(std::set<StochasticNode*>& affected) {

    touched = true;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->getAffected(affected);

    return value;
}


/** Keep the current value of the node and tell affected */
void StochasticNode::keep() {

    // TODO: See if assignment is faster
    // This requires Sebastian's overloaded assignment operator
    if (touched) {
        delete storedValue;
        storedValue = value->clone();
    }

    touched = false;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->keepAffected();
}


/** Tell affected variable nodes to keep the current value: stop the recursion here */
void StochasticNode::keepAffected() {
}


/** Print struct for user */
void StochasticNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class        = " << getClass() << std::endl;
    o << "&.distribution = " << distribution << std::endl;
    o << "&.value        = " << getValue() << std::endl;
}


/** Print value for user */
void StochasticNode::printValue(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print value while in touched state");

    value->printValue(o);
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restore() {

    // TODO: See if assignment works instead, if it is faster
    if (touched) {
        delete value;
        value = storedValue->clone();
    }

    touched = false;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->restoreAffected();

}


/** Tell affected variable nodes to restore the old value: stop the recursion here */
void StochasticNode::restoreAffected() {
}


/** Set element: set value element */
void StochasticNode::setElement(const IntVector& index, RbObject* val) {

    if (clamped)
        throw RbException("Cannot set value element of clamped node");

    if (value->getDim() == 0)
        throw RbException("Object does not have elements");

    ((RbComplex*)(value))->setElement(index, val);
    storedValue = val->clone();
}


/** Set value: same as clamp, but do not clamp */
void StochasticNode::setValue(RbObject* val) {

    if (clamped)
        throw RbException("Cannot set value of clamped node");

    value = val;
    storedValue = val->clone();
}


/** Complete info about object */
std::string StochasticNode::toString(void) const {

    std::ostringstream o;
    o << "StochasticNode:" << std::endl;
    o << "Clamped      = " << (clamped ? "true" : "false") << std::endl;
    o << "Touched      = " << (touched ? "true" : "false") << std::endl;
    o << "Distribution = ";
    distribution->printValue(o);
    o << std::endl;
    o << "Value        = ";
    value->printValue(o);
    o << std::endl;
    o << "Stored value = ";
    if (storedValue == NULL)
        o << "NULL";
    else
        storedValue->printValue(o);

    return o.str();
}


/** Unclamp the value; use the clamped value as initial value */
void StochasticNode::unclamp(void) {

    clamped = false;
    storedValue = value->clone();
}


