/**
 * @file
 * This file contains the implementation of FunctionNode, which is derived
 * from DeterministicNode. FunctionNode is used for a DAG node associated with
 * an explicit function call that determines its value.
 *
 * @brief Implementation of FunctionNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: FunctionNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "FunctionNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty function node */
FunctionNode::FunctionNode(const TypeSpec& valType) :
    DeterministicNode(valType.getType()), valueDim(valType.getDim()) {	
}


/** Constructor from function: get parents from the function object */
FunctionNode::FunctionNode(RbFunction* func) :
    DeterministicNode(func->getReturnType().getType()), valueDim(func->getReturnType().getDim())  {

    /* Check for cycles */
    const std::vector<DAGNode*>& arguments = func->getProcessedArguments();
    std::list<DAGNode*> done;
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        if ((*i)->isParentInDAG(this, done))
            throw RbException ("Invalid assignment: cycles in the DAG");
    }

    /* Set parents and add this node as a child node of these */
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }

    /* Set the function */
    function = func;

    /* Set value and stored value */
    DAGNode* retVal = function->execute();
    value = retVal->getValue()->clone();
    delete retVal;
    storedValue = NULL;
}


/** Copy constructor */
FunctionNode::FunctionNode(const FunctionNode& x) : DeterministicNode(x), valueDim(x.valueDim) {

    function    = (RbFunction*)(x.function->clone());
    changed     = false;
    DAGNode* retVal = function->execute();
    value = retVal->getValue()->clone();
    delete retVal;
    storedValue = NULL;

    /* Set parents and add this node as a child node of these */
    const std::vector<DAGNode*>& arguments = function->getProcessedArguments();
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }
}


/** Destructor */
FunctionNode::~FunctionNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete node with references");

    /* Function does not delete processed arguments (otherwise there
       would be problems with DeterministicNode destructor) */
    if (function)
        delete function;
}


/** Assignment operator */
FunctionNode& FunctionNode::operator=(const FunctionNode& x) {

    if (this != &x) {

        if (valueType != x.valueType)
            throw RbException("Type mismatch");
        
        /* Remove this node as a child node of parents */
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
            (*i)->removeChildNode(this);
            if ((*i)->numRefs() == 0)
                delete (*i);
        }

        if (function) {
            delete function;
            function = NULL;
        }
        if (x.function)
            function    = (RbFunction*)(x.function->clone());

        if (storedValue != NULL)
            delete storedValue;
        delete value;

        DAGNode* retVal = function->execute();
        value = retVal->getValue()->clone();
        delete retVal;
        storedValue = NULL;

        /* Set parents and add this node as a child node of these */
        parents.clear();
        const std::vector<DAGNode*>& arguments = function->getProcessedArguments();
        for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
            parents.insert(*i);
            (*i)->addChildNode(this);
        }
    }

    return (*this);
}


/** Clone this object */
FunctionNode* FunctionNode::clone(void) const {

	return new FunctionNode(*this);
}


/** Clone the entire graph: clone children, swap parents */
FunctionNode* FunctionNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (FunctionNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    FunctionNode* copy = new FunctionNode(valueType);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone parents */
    copy->function = (RbFunction*)(function->clone());
    std::vector<DAGNode*>& params     = const_cast<std::vector<DAGNode*>& > (function->getProcessedArguments());
    std::vector<DAGNode*>& copyParams = const_cast<std::vector<DAGNode*>& > (copy->function->getProcessedArguments());
    copyParams.clear();
    DAGNode* retVal = copy->function->execute();
    copy->value = retVal->getValue()->clone();
    delete retVal;
    copy->storedValue = NULL;
    copy->touched = false;
    copy->changed = false;
    for (std::vector<DAGNode*>::iterator i=params.begin(); i!=params.end(); i++) {
        DAGNode* theParentClone = (*i)->cloneDAG(newNodes);
        copyParams.push_back(theParentClone);
        copy->parents.insert(theParentClone);
        theParentClone->addChildNode(copy);
    }

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& FunctionNode::getDAGClass() const {

    static VectorString rbClass = VectorString(FunctionNode_name) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Is it possible to mutate node to newNode? */
bool FunctionNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to mutate node to contain newValue? */
bool FunctionNode::isMutableTo(const VectorInteger& index, const RbObject* newValue) const {

    assert (!newValue->isType(Container_name));
    
    bool isMutable = false;

    return isMutable;
}


/** Is it possible to change parent node oldNode to newNode? */
bool FunctionNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find node among parents
    if (parents.find(const_cast<DAGNode*>(oldNode)) == parents.end())
        throw RbException("Node is not a parent");

    // Now find node among parameters
    std::vector<DAGNode*>& params = const_cast<std::vector<DAGNode*>& > (function->getProcessedArguments());
    std::vector<DAGNode*>::iterator it = std::find(params.begin(), params.end(), oldNode);
    if (it == params.end())
        throw RbException("Node is not a parameter");

    // Find the corresponding rule
    const ArgumentRules& argRules = function->getArgumentRules();
    size_t index = it - params.begin();
    ArgumentRule* theRule;
    if ( index >= argRules.size() )
        theRule = argRules[argRules.size()-1];
    else
        theRule = argRules[index];

    // See if the new node value is convertible to the required type
    if ( Workspace::globalWorkspace().isXConvertibleToY( theRule->getArgType(), theRule->getArgDim(), newNode->getValueType(), newNode->getDim() ) )
        return true;
    
    return false;
}


/** Print struct for user */
void FunctionNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class    = " << getDAGClass() << std::endl;
    o << "&.function = " << function << std::endl;
    o << "&.value    = " << value << std::endl;
    o << "&.parents = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "&.children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Complete info about object */
std::string FunctionNode::richInfo(void) const {

    std::ostringstream o;

    o << "FunctionNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;

    o << "function    = ";
    function->printValue(o);
    o << std::endl;

    o << "value       = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Swap parent node both in parents set and in processed arguments */
void FunctionNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    std::vector<DAGNode*>& params = const_cast<std::vector<DAGNode*>& > (function->getProcessedArguments());
    std::vector<DAGNode*>::iterator it = std::find(params.begin(), params.end(), oldNode);
    if (it == params.end())
        throw RbException("Node is not a parameter");
    it = params.erase(it);
    params.insert(it, newNode);

    touched = true;
    changed = false;
    touchAffected();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void FunctionNode::update(void) {

    if (touched && !changed) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        DAGNode* retVal = function->execute();
        value = retVal->getValue()->clone();
        delete retVal;
        changed = true;
    }
}

