/**
 * @file
 * This file contains the implementation of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated probability density function.
 *
 * @brief Implementation of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:51:54 +0100 (Tis, 12 Jan 2010) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: StochasticNode.cpp 221 2010-01-12 20:51:54Z Hoehna $
 */

#include "Distribution.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty StochasticNode */
StochasticNode::StochasticNode(const std::string& type) : VariableNode(type), clamped(false), distribution(NULL), value(NULL), storedValue(NULL) {

	isDagExposed = true;
}


/**
 * Constructor from distribution.
 *
 * @note MemberTable is a typedef for Frame defined in RbComplex.h.
 */
StochasticNode::StochasticNode(Distribution* dist) : VariableNode(dist->getVariableType()) {

    /* Get distribution parameters */
    const VariableTable& params = dist->getMembers().getVariableTable();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        done.clear();
        if ((*i).second.variable->isParentInDAG(this, done))
            throw RbException("Invalid assignment: Cycles in the DAG");
    }

    /* Set parent(s) and add myself as a child to these */
    for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        parents.insert((*i).second.variable);
        (*i).second.variable->addChildNode(this);
    }

    clamped      = false;
    distribution = dist;

    value        = distribution->rv();
    storedValue  = value->clone();
	
	isDagExposed = true;
}


/** Copy constructor */
StochasticNode::StochasticNode(const StochasticNode& x) : VariableNode(x) {

    /* Set distribution */
    distribution = (Distribution*)(x.distribution->clone());

    /* Get distribution parameters */
    const VariableTable& params = distribution->getMembers().getVariableTable();

    /* Set parent(s) and add myself as a child to these */
    for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        parents.insert((*i).second.variable);
        (*i).second.variable->addChildNode(this);
    }

    clamped      = x.clamped;
    value        = x.value->clone();
	isDagExposed = x.isDagExposed;
    if (clamped == false)
        storedValue  = value->clone();
    else
        storedValue = NULL;
}


/** Destructor */
StochasticNode::~StochasticNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete node with references"); 

    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
        (*i)->removeChildNode(this);
    parents.clear();

    delete distribution;    // This will delete any DAG nodes that need to be deleted
    delete value;
    if (storedValue != NULL)
        delete storedValue;
}


/** Assignment operator */
StochasticNode& StochasticNode::operator=(const StochasticNode& x) {

    if (this != &x) {

        if (valueType != x.valueType)
            throw RbException("Type mismatch");
        
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
            (*i)->removeChildNode(this);
        parents.clear();

        delete distribution;    // This will delete any DAG nodes that need to be deleted
        delete value;
        if (storedValue != NULL)
            delete storedValue;

        /* Set distribution */
        distribution = (Distribution*)(x.distribution->clone());

        /* Get distribution parameters */
        const VariableTable& params = distribution->getMembers().getVariableTable();

        /* Set parent(s) and add myself as a child to these */
        for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
            parents.insert((*i).second.variable);
            (*i).second.variable->addChildNode(this);
        }

        /* Set flags and value */
        clamped      = x.clamped;
        value        = x.value->clone();
        if (x.storedValue == NULL)
            storedValue = NULL;
        else
            storedValue  = x.storedValue->clone();  
    }

    return (*this);
}


/** Calculate the conditional probability of the node, do not rely on stored values */
double StochasticNode::calculateLnProbability(void) {

	return (distribution->lnPdf(value));
}



/** Clamp the node to an observed value */
void StochasticNode::clamp(RbObject* observedVal) {

    delete value;
    delete storedValue;

    value = observedVal;
    storedValue = NULL;

    clamped = true;
    touched = false;

    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
}


/** Clone this object */
StochasticNode* StochasticNode::clone(void) const {

    return new StochasticNode(*this);
}


/** Clone the entire graph: clone children, swap parents */
StochasticNode* StochasticNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (StochasticNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    StochasticNode* copy = new StochasticNode(valueType);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone parents */
    copy->distribution = (Distribution*)(distribution->clone());
    copy->clamped = clamped;
    copy->value = value->clone();
    if (storedValue == NULL)
        copy->storedValue = NULL;
    else
        copy->storedValue = copy->value->clone();

    const VariableTable& params = distribution->getMembers().getVariableTable();

    /* Replace the copy distribution params with NULL, deleting them from current DAG */
    for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        copy->distribution->setVariable((*i).first, NULL);
    }

    /* Set the copy params to their matches in the new DAG */
    for (VariableTable::const_iterator i=params.begin(); i!=params.end(); i++) {
        DAGNode* theParamClone = (*i).second.variable->cloneDAG(newNodes);
        copy->distribution->setVariable((*i).first, theParamClone);
        copy->parents.insert(theParamClone);
        theParamClone->addChildNode(copy);
    }

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Get affected nodes: insert this node but stop recursion here */
void StochasticNode::getAffected(std::set<StochasticNode*>& affected) {

    affected.insert(this);
}


/** Get class vector describing type of object */
const VectorString& StochasticNode::getClass() const {

    static VectorString rbClass = VectorString(StochasticNode_name) + VariableNode::getClass();
    return rbClass;
}


/** Get default moves from distribution */
MoveSchedule* StochasticNode::getDefaultMoves(void) {

    MoveSchedule* defaultMoves = new MoveSchedule(this, 1.0);
    defaultMoves->addMove(distribution->getDefaultMove(this));

    return defaultMoves;
}


/** Get the ln likelihood ratio of this node: delegate to distribution */
double StochasticNode::getLnLikelihoodRatio(void) {

    return distribution->lnLikelihoodRatio(value);
}


/** Get the ln prior ratio of this node: delegate to distribution if touched */
double StochasticNode::getLnPriorRatio(void) {

    if (touched)
        return distribution->lnPriorRatio(value, storedValue);
    else
        return 0.0;
}


/** Get stored value */
const RbObject* StochasticNode::getStoredValue(void) {

    if (!touched)
        return value;

    return storedValue;
}


/** Get value element */
const RbObject* StochasticNode::getValElement(const VectorInteger& index) const {

    if (value->getDim() == 0)
        throw RbException("Object does not have elements");

    return ((RbComplex*)(value))->getElement(index);
}


/** Get value */
const RbObject* StochasticNode::getValue(void) {

    return value;
}


/** Get const value if possible */
const RbObject* StochasticNode::getValue(void) const {

    if (touched)
        throw RbException("Const value not available");

    return value;
}


/** Get value pointer for move; tell and get affected */
RbObject* StochasticNode::getValuePtr(std::set<StochasticNode*>& affected) {

    touched = true;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->getAffected(affected);

    return value;
}


/** Is it possible to mutate node to newNode? */
bool StochasticNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to mutate node to contain newValue? */
bool StochasticNode::isMutableTo(const VectorInteger& index, const RbObject* newValue) const {

    assert (!newValue->isType(Container_name));
    
    bool isMutable = false;

    return isMutable;
}


/** Is it possible to change parent node oldNode to newNode? */
bool StochasticNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find the node among parent nodes
    if (parents.find(const_cast<DAGNode*>(oldNode)) == parents.end())
        throw RbException("Node is not a parent");

    /* Then find the distribution variable */
    const VariableTable& params = distribution->getMembers().getVariableTable();
    VariableTable::const_iterator it;
    for (it=params.begin(); it!=params.end(); it++) {
        if ((*it).second.variable == oldNode)
            break;
    }
    if (it == params.end())
        throw RbException("Node is not a parameter");

    // TODO: Replace with call to workspace function isXConvertibleToY
    if ( newNode->getValue()->isConvertibleTo((*it).second.type, (*it).second.dim) )
        return true;
    
    return false;
}


/** Keep the current value of the node and tell affected */
void StochasticNode::keep() {

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


/** Mutate to newNode */
void StochasticNode::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
}


/* Mutate to contain newValue */
StochasticNode* StochasticNode::mutateTo(const VectorInteger& index, RbObject* newValue) {

    throw RbException("Not implemented yet");
}


/** Print struct for user */
void StochasticNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class        = " << getClass() << std::endl;
    o << "&.distribution = " << distribution << std::endl;
    o << "&.value        = " << value << std::endl;
    o << "&.parents = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "&.children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Print value for user */
void StochasticNode::printValue(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print value while in touched state");

    value->printValue(o);
}


/** Restore the old value of the node and tell affected */
void StochasticNode::restore() {

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
void StochasticNode::setElement(const VectorInteger& index, RbObject* val) {

    if (clamped)
        throw RbException("Cannot set value element of clamped node");

    if (value->getDim() == 0)
        throw RbException("Object does not have elements");

    ((RbComplex*)(value))->setElement(index, val);
    storedValue = val->clone();

    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
}


/** Set name of variable and distribution parameters */
void StochasticNode::setName(const std::string& name) {

    DAGNode::setName(name);

    distribution->setName("&" + name + ".dist");
}


/** Set value: same as clamp, but do not clamp */
void StochasticNode::setValue(RbObject* val) {

    if (clamped)
        throw RbException("Cannot set value of clamped node");

    delete value;
    delete storedValue;

    value = val;
    storedValue = val->clone();

    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
}


/** Swap parent node both in parents set and in distribution */
void StochasticNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    /* Now deal with distribution variable */
    const VariableTable& params = distribution->getMembers().getVariableTable();
    VariableTable::const_iterator it;
    for (it=params.begin(); it!=params.end(); it++) {
        if ((*it).second.variable == oldNode)
            break;
    }
    if (it == params.end())
        throw RbException("Node is not a parameter");

    distribution->setVariable((*it).first, newNode);

    if (clamped == false) {
        delete value;
        delete storedValue;
        value = distribution->rv();
        storedValue = value->clone();
    }

    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
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

