/**
 * @file
 * This file contains the implementation of LookupNode, which is used to
 * hold and perform lookups of variables.
 *
 * @brief Implementation of LookupNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: LookupNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "Integer.h"
#include "LookupNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty lookup node */
LookupNode::LookupNode(const TypeSpec& valType) :
    DeterministicNode(valType), variable(NULL), baseVariable(NULL), memberName(), indexArgs() {
}


/** Construct lookup of regular variable */
LookupNode::LookupNode(DAGNode* var, IndexArgs&  indxArgs) :
    DeterministicNode(var->getValueType()), variable(var), baseVariable(NULL), memberName(), indexArgs(indxArgs), valueDim(0) {

    /* Check index arguments */
    for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        if ( !Workspace::userWorkspace().isXConvertibleToY((*i)->getValueType(), (*i)->getDim(), Integer_name, 0) )
            throw RbException ("Invalid type of index argument");
    }
        
    /* Update dimension variable to reflect result of lookup */
    valueDim -= indexArgs.size();
        
    /* Check for cycles */
    std::list<DAGNode*> done;
    for (IndexArgs::const_iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        if ((*i)->isParentInDAG(this, done))
            throw RbException ("Invalid assignment: cycles in the DAG");
    }
    if (variable->isParentInDAG(this, done))
        throw RbException ("Invalid assignment: cycles in the DAG");

    /* Set parents and add this node as a child node of these */
    for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }
    parents.insert(variable);
    variable->addChildNode(this);

    /* Set value and stored value */
    value = lookup();
    storedValue = NULL;
}


/** Construct lookup of member variable */
LookupNode::LookupNode(LookupNode* baseVar, RbString* membrName, IndexArgs&  indxArgs) :
    DeterministicNode(baseVar->getMemberTypeSpec(membrName).getType()), variable(NULL), baseVariable(baseVar), memberName(membrName), indexArgs(indxArgs), valueDim(0) {

    /* Check index arguments */
    for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        if ( !Workspace::userWorkspace().isXConvertibleToY((*i)->getValueType(), (*i)->getDim(), Integer_name, 0) )
            throw RbException ("Invalid type of index argument");
    }
        
    /* Update dimension variable to reflect result of lookup */
    valueDim = baseVariable->getMemberTypeSpec(memberName).getDim() - indexArgs.size();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for (IndexArgs::const_iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        if ((*i)->isParentInDAG(this, done))
            throw RbException ("Invalid assignment: cycles in the DAG");
    }
    if (baseVariable->isParentInDAG(this, done))
        throw RbException ("Invalid assignment: cycles in the DAG");

    /* Set parents and add this node as a child node of these */
    for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }
    parents.insert(baseVariable);
    baseVariable->addChildNode(this);

    /* Set value and stored value */
    value = lookup();
    storedValue = NULL;
}


/** Copy constructor */
LookupNode::LookupNode(const LookupNode& x) : DeterministicNode(x) {

    variable     = x.variable;
    baseVariable = x.baseVariable;
    indexArgs    = x.indexArgs;
    value        = lookup();
    storedValue  = NULL;

    /* Set parents and add this node as a child node of these */
    for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }
    if (variable != NULL) {
        parents.insert(baseVariable);
        baseVariable->addChildNode(this);
    }
    if (baseVariable != NULL) {
        parents.insert(baseVariable);
        baseVariable->addChildNode(this);
    }
}


/**
 * Destructor
 *
 * @note Parent nodes, including variable or baseVariable, destroyed
 *       by DeterministicNode destructor
 */
LookupNode::~LookupNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete lookup node with references");

    delete memberName;
}


/** Assignment operator */
LookupNode& LookupNode::operator=(const LookupNode& x) {

    if (this != &x) {

        if (valueType != x.valueType)
            throw RbException("Type mismatch");
        
        touched     = false;
        changed     = false;
        delete value;
        if (storedValue)
            delete storedValue;
        delete memberName;

        /* Remove this node as a child node of parents */
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
            (*i)->removeChildNode(this);
            if ((*i)->numRefs() == 0)
                delete (*i);
        }

        variable        = x.variable;
        baseVariable    = x.baseVariable;
        memberName      = new RbString(*x.memberName);
        indexArgs       = x.indexArgs;
        value           = lookup();
        storedValue     = NULL;

        /* Set parents and add this node as a child node of these */
        for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
            parents.insert(*i);
            (*i)->addChildNode(this);
        }
        if (variable != NULL) {
            parents.insert(baseVariable);
            baseVariable->addChildNode(this);
        }
        if (baseVariable != NULL) {
            parents.insert(baseVariable);
            baseVariable->addChildNode(this);
        }
    }

    return (*this);
}


/** Clone this object */
LookupNode* LookupNode::clone(void) const {

	return new LookupNode(*this);
}


/** Clone the entire graph: clone children, swap parents */
LookupNode* LookupNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (LookupNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    LookupNode* copy = new LookupNode(valueType);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone parents */
    copy->memberName = new  RbString(*memberName);
    for (std::vector<DAGNode*>::const_iterator i=indexArgs.begin(); i!=indexArgs.end(); i++) {
        DAGNode* theParentClone = (*i)->cloneDAG(newNodes);
        copy->indexArgs.push_back(theParentClone);
        copy->parents.insert(theParentClone);
        theParentClone->addChildNode(copy);
    }
    if (variable != NULL) {
        copy->variable = variable->cloneDAG(newNodes);
        copy->parents.insert(copy->variable);
        copy->variable->addChildNode(copy);
    }
    if (baseVariable != NULL) {
        copy->baseVariable = baseVariable->cloneDAG(newNodes);
        copy->parents.insert(copy->baseVariable);
        copy->baseVariable->addChildNode(copy);
    }

    copy->value        = copy->lookup();
    copy->storedValue  = NULL;
    copy->touched = false;
    copy->changed = false;

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& LookupNode::getDAGClass() const {

    static VectorString rbClass = VectorString(LookupNode_name) + VariableNode::getDAGClass();
    return rbClass;
}


/** Get type of a named member variable */
const TypeSpec& LookupNode::getMemberTypeSpec(const RbString* name) const {

    if ( baseVariable == NULL )
        throw RbException( "Not a member variable" );

    const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseVariable->getValue() );
    if (theBaseObject == NULL)
        throw RbException( "Base variable does not have members");

    return theBaseObject->getMembers().getVariableSlot(*memberName).getTypeSpec();
}


/** Look up and get a reference to the variable */
DAGNode* LookupNode::getVariable(void) {
    
    // Look up variable
    if (indexArgs.size() == 0) {

        if (variable != NULL)
            return variable;
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseVariable->getValue() );
            if (theBaseObject == NULL)
                throw RbException( "Base variable does not have members");

            return const_cast<DAGNode *>( theBaseObject->getVariable(*memberName) );
        }
    }
    else {
        // Get the index vector from the index arguments
        VectorInteger index;
        for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++)
            index.push_back(((Integer*)((*i)->getValue()))->getValue() - 1);

        if (variable != NULL)
            return const_cast<DAGNode *>( variable->getVarElement(index) );
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseVariable->getValue() );
            if ( theBaseObject == NULL )
                throw RbException( "Base variable does not have members" );

            return const_cast<DAGNode*>( theBaseObject->getVariable(*memberName)->getVarElement(index) );
        }
    }
}


/** Is it possible to mutate node to newNode? */
bool LookupNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to mutate node to contain newValue? */
bool LookupNode::isMutableTo(const VectorInteger& index, const RbObject* newValue) const {

    assert (!newValue->isType(Container_name));
    
    bool isMutable = false;

    return isMutable;
}


/** Is it possible to change parent node oldNode to newNode? */
bool LookupNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find node among parents
    if (parents.find(const_cast<DAGNode*>(oldNode)) == parents.end())
        throw RbException("Node is not a parent");

    // Now find node among indexArgs or variable/baseVariable
    if (oldNode == variable) {
        if ( Workspace::globalWorkspace().isXConvertibleToY(newNode->getValueType(), newNode->getDim(), variable->getValueType(), variable->getDim()) )
            return true;
        else
            return false;
    }
    else if (oldNode == baseVariable) {
        if ( Workspace::globalWorkspace().isXConvertibleToY(newNode->getValueType(), newNode->getDim(), baseVariable->getValueType(), baseVariable->getDim()) )
            return true;
        else
            return false;
    }
    else {
        std::vector<DAGNode*>::const_iterator it = std::find(indexArgs.begin(), indexArgs.end(), oldNode);
        if (it == indexArgs.end())
            throw RbException("Node is not a parameter");
        if ( Workspace::globalWorkspace().isXConvertibleToY(newNode->getValueType(), newNode->getDim(), Integer_name, 0) )
            return true;
        else
            return false;
    }
    
    return false;
}


/** Look up the value */
RbObject* LookupNode::lookup(void) {
    
    // Look up value
    if (indexArgs.size() == 0) {
        if (variable != NULL)
            return variable->getValue()->clone();
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseVariable->getValue() );
            if (theBaseObject == NULL)
                throw RbException( "Base variable does not have members");

            return theBaseObject->getValue(*memberName)->clone();
        }
    }
    else {
        // Get the index vector from the index arguments
        VectorInteger index;
        for (IndexArgs::iterator i=indexArgs.begin(); i!=indexArgs.end(); i++)
            index.push_back(((Integer*)((*i)->getValue()))->getValue() - 1);

        if (variable != NULL)
            return variable->getValElement(index)->clone();
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseVariable->getValue() );
            if ( theBaseObject == NULL )
                throw RbException( "Base variable does not have members" );

            return theBaseObject->getVariable(*memberName)->getValElement(index)->clone();
        }
    }
}


/** Mutate to newNode */
void LookupNode::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
}


/* Mutate to contain newValue */
LookupNode* LookupNode::mutateTo(const VectorInteger& index, RbObject* newValue) {

    throw RbException("Not implemented yet");
}


/** Print struct for user (a lookup node should be impossible to access for user) */
void LookupNode::printStruct(std::ostream& o) const {

    throw RbException("Structure of lookup node should not be visible to user");
}


/** Swap parent node both in parents set and in processed arguments */
void LookupNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    /* First swap the node in the parents set */
    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    /* Now find and swap the node among parameters */
    if (oldNode == variable) {
        variable = newNode;
    }
    else if (oldNode == baseVariable) {
        baseVariable = dynamic_cast<LookupNode *>( newNode );
    }
    else {
        std::vector<DAGNode*>::iterator it = std::find(indexArgs.begin(), indexArgs.end(), oldNode);
        if (it == indexArgs.end())
            throw RbException("Node is not a parameter");
        it = indexArgs.erase(it);
        indexArgs.insert(it, newNode);
    }

    touched = true;
    changed = false;
    touchAffected();
}


/** Complete info about object */
std::string LookupNode::toString(void) const {

    std::ostringstream o;

    o << "LookupNode:" << std::endl;

    o << "name       = " << getName() << std::endl;
    o << "touched    = " << (touched ? "true" : "false") << std::endl;
    o << "changed    = " << (changed ? "true" : "false") << std::endl;

    if (variable != NULL) {
        o << "variable =" << std::endl;
        variable->printValue(o);
        o << std::endl;
    }
    if (baseVariable != NULL) {
        o << "baseVariable =" << std::endl;
        baseVariable->printValue(o);
        o << std::endl;
        o << "memberName = " << *memberName << std::endl;
    }

    o << "value      = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void LookupNode::update(void) {

    if (touched && !changed) {
        if (storedValue)
            delete storedValue;
        storedValue = value;
        value       = lookup();
        changed = true;
    }
}

