/**
 * @file
 * This file contains the implementation of DAGNodeContainer,
 * which is used to hold vectors or multidimensional arrays of
 * DAG nodes.
 *
 * @brief Implementation of DAGNodeContainer
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "Container.h"
#include "DAGNodeContainer.h"
#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <algorithm>
#include <iostream>
#include <sstream>


/** Construct vector with one node x */
DAGNodeContainer::DAGNodeContainer(DAGNode* x) : VariableNode(x->getType()) {

    nodes.push_back(x);

    parents.insert(x);
    x->addChildNode(this);
    
    changed      = false;
    length       = IntVector(1);
    value        = new Container(x->getValue()->clone());
    storedValue  = value->clone();
    names        = NULL;
	isDagExposed = true;
}


/** Construct vector with n copies of x */
DAGNodeContainer::DAGNodeContainer(size_t n, DAGNode* x) : VariableNode(x->getType()) {

    nodes.push_back(x);
    for (size_t i=1; i<n; i++)
        nodes.push_back(x->clone());

    for (std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }

    changed      = false;
    length       = IntVector(int(n));
    value        = new Container(n, x->getValue()->clone());
    storedValue  = value->clone();
    names        = NULL;
	isDagExposed = true;
}


/** Construct empty vector of length  n */
DAGNodeContainer::DAGNodeContainer(size_t n, const std::string& valType)
    : VariableNode(valType) {

    for (size_t i=0; i<n; i++)
        nodes.push_back(NULL);

    changed      = false;
    length       = IntVector(int(n));
    value        = new Container(length, valueType);
    storedValue  = value->clone();
    names        = NULL;
	isDagExposed = true;
}


/** Construct array of given dimensions containing copies of x */
DAGNodeContainer::DAGNodeContainer(const IntVector& len, DAGNode* x)
    : VariableNode(x->getValueType()) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive length"));
        n *= len[i];
    }

    nodes.push_back(x);
    for (int i=1; i<n; i++)
        nodes.push_back(x->clone());

    for (std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }

    changed      = false;
    length       = len;
    value        = new Container(len, x->getValue()->clone());
    storedValue  = value->clone();
    names        = NULL;
	isDagExposed = true;
}


/** Construct empty array of given dimensions */
DAGNodeContainer::DAGNodeContainer(const IntVector& len, const std::string& valType)
    : VariableNode(valType) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] < 0)
            throw (RbException("Negative length"));
        n *= len[i];
    }

    for (int i=0; i<n; i++)
        nodes.push_back(NULL);

    changed     = false;
    length      = len;
    value       = new Container(len, valueType);
    storedValue = value->clone();
    names       = NULL;
	isDagExposed = true;
}

/** Copy constructor needed to make sure nodes elements are independent */
DAGNodeContainer::DAGNodeContainer(const DAGNodeContainer& x)
    : VariableNode(x) {

    changed = x.changed;
    length  = x.length;

    for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++) {
        DAGNode* theNode = (*i)->clone();
        nodes.push_back(theNode);
        parents.insert(theNode);
        theNode->addChildNode(this);
    }

    value       = x.value->clone();
    storedValue = x.storedValue->clone();
    if (x.names == NULL)
        names = NULL;
    else
        names = x.names->clone();
	isDagExposed = true;
}

/** Destructor needed to destroy value, stored value and nodes */
DAGNodeContainer::~DAGNodeContainer() {

    if (numRefs() != 0)
        throw RbException ("Cannot delete node with references");

    for(std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++) {
        DAGNode* theNode = (*i);
        if (theNode != NULL) {
            theNode->removeChildNode(this);
            if (theNode->numRefs() == 0)
                delete theNode;
        }
    }
    parents.clear();
    nodes.clear();
    delete names;

    delete value;
    delete storedValue;
}


/** Assignment operator; make sure we get independent nodes */
DAGNodeContainer& DAGNodeContainer::operator=(const DAGNodeContainer& x) {

    if (this != &x) {

        if (valueType != x.valueType || getDim() != x.getDim())
            throw RbException("Type or dim mismatch");

        /* Remove nodes and parents */
        for(std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++) {
            DAGNode* theNode = (*i);
            if (theNode != NULL) {
                theNode->removeChildNode(this);
                theNode->setName("");
                if (theNode->numRefs() == 0)
                    delete theNode;
            }
        }
        parents.clear();
        nodes.clear();
        delete names;
        names = NULL;

        changed = x.changed;
        length  = x.length;

        /* Add nodes and parents */
        for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++) {
            DAGNode* theNode = (*i)->clone();
            nodes.push_back(theNode);
            parents.insert(theNode);
            theNode->addChildNode(this);
        }

        value       = x.value->clone();
        storedValue = x.storedValue->clone();
        if (x.names == NULL)
            names = NULL;
        else
            names = x.names->clone();
    }

    return (*this);
}


/** Subscript operator (vector index) */
DAGNode*& DAGNodeContainer::operator[](const IntVector& i) {

    return nodes[getOffset(i)];
}


/** Subscript const operator (vector index) */
DAGNode* const& DAGNodeContainer::operator[](const IntVector& i) const {

    return nodes[getOffset(i)];
}


/** Subscript operator */
DAGNode*& DAGNodeContainer::operator[](size_t i) {

    if (i >= nodes.size())
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Subscript const operator */
DAGNode* const& DAGNodeContainer::operator[](size_t i) const {

    if (i >= nodes.size())
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Return begin iterator */
ContainerIterator DAGNodeContainer::begin(void) const {

    IntVector temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i] = 0;

    return ContainerIterator(temp, getLength());
}


/** Clear container */
void DAGNodeContainer::clear(void) {

    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
        (*i)->removeChildNode(this);
        (*i)->setName("");
        if ((*i)->numRefs() == 0)
            delete(*i);
    }

    for (size_t i=0; i<length.size(); i++)
        length[i] = 0;

    nodes.clear();
}


/** Clone object */
DAGNodeContainer* DAGNodeContainer::clone() const {

    return new DAGNodeContainer(*this);
}


/** Clone entire graph */
DAGNodeContainer* DAGNodeContainer::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNodeContainer*)(this)) != newNodes.end())
        return (DAGNodeContainer*)(newNodes[(DAGNode*)(this)]);

    /* Make pristine copy */
    DAGNodeContainer* copy = new DAGNodeContainer(length, valueType);
    newNodes[(DAGNode*)(this)] = copy;

    copy->touched     = true;
    copy->changed     = false;
    copy->length      = length;
    copy->value       = value->clone();
    copy->storedValue = copy->value->clone();
    if (names == NULL)
        copy->names = NULL;
    else
        copy->names = names->clone();

    /* Clone parents */
    copy->nodes.clear();
    for (std::vector<DAGNode*>::const_iterator i=nodes.begin(); i!= nodes.end(); i++) {
        if ((*i) == NULL)
            copy->nodes.push_back(NULL);
        else {
            DAGNode* theParentCopy = (*i)->cloneDAG(newNodes);
            copy->parents.insert(theParentCopy);
            copy->nodes.push_back(theParentCopy);
            theParentCopy->addChildNode(copy);
        }
    }
    copy->update();
    copy->keepAffected();

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Return end iterator */
ContainerIterator DAGNodeContainer::end(void) const {

    IntVector temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i]--;

    ContainerIterator tempIt(temp, getLength());
    return ++tempIt;
}


/** Get affected nodes: touch and pass through to next stochastic node */
void DAGNodeContainer::getAffected(std::set<StochasticNode*>& affected) {

    /* If we have already touched this node, we are done; otherwise, get the affected children */
    if (!touched) {
        touched = true;
        changed = false;
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->getAffected(affected);
    }
}


/** Get class vector describing type of object */
const StringVector& DAGNodeContainer::getClass(void) const {

    static StringVector rbClass = StringVector(DAGNodeContainer_name) + VariableNode::getClass();
    return rbClass;
}


/** Get element name */
std::string DAGNodeContainer::getElementName(const IntVector& index) const {

    if (names == NULL)
        return "";
    else
        return (*names)[getOffset(index)];
}


/** Get offset to element or subcontainer; also check index */
size_t DAGNodeContainer::getOffset(const IntVector& index) const {

    if (index.size() > length.size())
        throw (RbException("Too many indices"));
    for (size_t i=0; i<index.size(); i++) {
        if (index[i] < 0 || index[i] >= length[i])
            throw (RbException("Index out of range"));
    }

    size_t offset = 0;
    int numVals = 1;
    int i;
    for (i=int(length.size())-1; i>=int(index.size()); i--)
         numVals *= length[i];
    for (; i>=0; i--) {
        offset += (index[i] * numVals);
        numVals *= length[i];
    }
 
    return offset;
}


/** Get stored value */
const RbObject* DAGNodeContainer::getStoredValue(void) {

    if (!touched)
        return value;

    update();
    return storedValue;
}


/** Get subcontainer */
DAGNodeContainer* DAGNodeContainer::getSubContainer(const IntVector& index) const {

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Create a new vector of the right size
    IntVector tempLength;
    for (size_t i=index.size(); i<length.size(); i++)
        tempLength.push_back(length[i]);
    DAGNodeContainer* temp = new DAGNodeContainer(tempLength, valueType);

    // Fill it with content
    int numSubvals = 1;
    for (size_t i=length.size()-1; i>=index.size(); i--)
        numSubvals *= length[i];

    std::vector<DAGNode*>::iterator i = temp->nodes.begin();
    std::vector<DAGNode*>::const_iterator j = nodes.begin() + offset;
    for (; i!=temp->nodes.end(); i++, j++)
        (*i) = (*j);

    return temp;
}


/** Get value element */
const RbObject* DAGNodeContainer::getValElement(const IntVector& index) const {

    // Check that the index is to a value element
    if (int(index.size()) < getDim())
        throw (RbException("Index error: Not value element"));

    // Split the index up
    IntVector containerIndex = index;
    containerIndex.resize(length.size());
    IntVector valueIndex;
    for (size_t i=length.size(); i<index.size(); i++)
        valueIndex.push_back(length[i]);        

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(containerIndex);

    // Get element
    DAGNode* element = nodes[offset];

    // Return value
    if (element == NULL)
        return NULL;
    if (valueIndex.size() == 0)
        return element->getValue();
    else if (element->getValue() == NULL)
        return NULL;
    else if (!element->isType(RbComplex_name))
        throw RbException("Object does not have elements");
    else
        return ((RbComplex*)(element->getValue()))->getElement(valueIndex);
}


/** Get value */
const RbObject* DAGNodeContainer::getValue(void) {

    update();
    return value;
}


/** Get const value if possible */
const RbObject* DAGNodeContainer::getValue(void) const {

    if (touched && !changed)
        throw RbException("Constant value not available");

    return value;
}


/** Get variable element */
const DAGNode* DAGNodeContainer::getVarElement(const IntVector& index) const {

    // Check that the index is to a variable element
    if (index.size() != length.size())
        throw (RbException("Index error: Not index to variable"));

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Get non-const variable element for friend class Frame */
DAGNode* DAGNodeContainer::getVarElement(const IntVector& index) {

    // Check that the index is to a variable element
    if (index.size() != length.size())
        throw (RbException("Index error: Not index to variable"));

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Keep value of node and affected variable nodes */
void DAGNodeContainer::keepAffected(void) {

    if (touched) {
        if (!changed)
            update();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->keepAffected();
    }
    touched = changed = false;
}


/** Print struct for user */
void DAGNodeContainer::printStruct(std::ostream& o) const {

    if (touched && !changed)
        throw RbException ("Cannot print value while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class  = " << getClass() << std::endl;
    o << "&.dim    = " << getDim() << std::endl;
    o << "&.length = " << getLength() << std::endl;
    o << "&.value  = " << value << std::endl;
    o << "&.parents = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "&.children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Print value for user */
void DAGNodeContainer::printValue(std::ostream& o) const {

    if (touched && !changed)
        throw RbException ("Cannot print value while in touched state");

    value->printValue(o);
}


/** Reset container length in different dimensions */
void DAGNodeContainer::resize(const IntVector& len) {

    // Check if there is anything to do
    if (len.equals(&length))
        return;

    // Check that new length specification has the right number of dimensions
    if (len.size() != length.size())
        throw (RbException("Cannot resize container: dimensions do not match"));

    // Calculate handy numbers
    IntVector numValsSource = IntVector(len.size(), 0), numValsTarget = IntVector(len.size(), 0);
    int numSourceVals = 1, numTargetVals = 1;
    for (int i=int(len.size())-1; i>=0; i--) {
        numSourceVals *= length[i];
        numTargetVals *= len[i];
        numValsSource[i] = numSourceVals;
        numValsTarget[i] = numTargetVals;
    }

    // Create new empty nodes vector
    StringVector* tempNames;
    std::vector<DAGNode*> tempNodes = std::vector<DAGNode*>(numValsTarget[0]);
    if (names != NULL)
        tempNames = new StringVector(numValsTarget[0]);

    // Reorganize content
    if (nodes.size() > 0) {
        int sourceIndex = 0, targetIndex = 0;
        do {
            for (int i=0; i<length[length.size()-1]; i++) {
                tempNodes[targetIndex] = nodes[sourceIndex];
                if (names != NULL)
                    tempNames[targetIndex] = names[sourceIndex];
                targetIndex++;
                sourceIndex++;
            }

            for (int i=int(length.size())-1; i>=0; i--) {
                if (sourceIndex % numValsSource[i] == 0) {
                    targetIndex += numValsTarget[i] - (targetIndex % numValsTarget[i]);
                }
            }
        } while (sourceIndex < (int)(nodes.size()) && targetIndex < numValsTarget[0]);
    }

    // Copy the new container and length specification
    nodes.clear();
    nodes  = tempNodes;
    length = len;
    if (names != NULL) {
        delete names;
        names = tempNames;
    }

    // Resize value container
    value->resize(length);
    storedValue->resize(length);
}


/** Restore value of node and affected variable nodes */
void DAGNodeContainer::restoreAffected(void) {

    if (touched) {
        if (changed) {
            Container* temp = value;
            value = storedValue;
            storedValue = temp;
        }
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->restoreAffected();
        }
    }
    touched = changed = false;
}


/** Set value element or elements from value */
void DAGNodeContainer::setElement(const IntVector& index, RbObject* val) {

    // Resize if necessary
    if (index.size() == length.size()) {
        IntVector tempLen  = length;
        bool      growSize = false;
        for (size_t i=0; i<index.size(); i++) {
            if (index[i] >= tempLen[i]) {
                tempLen[i] = index[i] + 1;
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    size_t offset = getOffset(index);

    // Find target node
    std::vector<DAGNode*>::iterator targetIt = nodes.begin() + offset;

    // Check if parser wants to set multiple elements
    if (index.size() < length.size()) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw(RbException("Invalid assignment: Container to subcontainer"));

        // Check that the source is a container
        RbComplex* source = dynamic_cast<RbComplex*>(val);
        if (source == NULL || source->getDim() == 0)
            throw RbException("Source does not have elements");
    
        // Count number of elements
        const IntVector& sourceLen = source->getLength();
        int numSourceElements = 1;
        for (size_t i=0; i<sourceLen.size(); i++)
            numSourceElements *= sourceLen[i];
        int numTargetElements = 1;
        for (size_t i=index.size(); i<length.size(); i++)
            numTargetElements *= length[i];

        // Throw an error if a mismatch
        if (numSourceElements != numTargetElements)
            throw RbException("Unequal source and target sizes");

        // Cycle through assignments
        for (ContainerIterator i=source->begin(); i!=source->end(); ++i) {

            // Do the assignment
            const RbObject* elem = source->getElement(i);
            if (elem->isType(getValueType())) {
                if ((*targetIt) != NULL && (*targetIt)->isType(StochasticNode_name))
                    ((StochasticNode*)(*targetIt))->setValue(elem->clone());
                else {
                    if ((*targetIt) != NULL) {
                        (*targetIt)->removeChildNode(this);
                        if ((*targetIt)->numRefs() == 0)
                            delete (*targetIt);
                        parents.erase(*targetIt);
                    }
                    (*targetIt) = new ConstantNode(elem->clone());
                    (*targetIt)->addChildNode(this);
                    parents.insert(*targetIt);
                }
            }
            else
                throw (RbException("Incompatible types"));

            // Increase target iterator
            targetIt++;
        }
        delete val;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (val->isType(getValueType())) {
        if ((*targetIt) != NULL && (*targetIt)->isType(StochasticNode_name)) {
            // We just set the value of the stochastic node
            ((StochasticNode*)(*targetIt))->setValue(val);
        }
        else {
            // We replace the node with a constant node
            if ((*targetIt) != NULL) {
                (*targetIt)->removeChildNode(this);
                if ((*targetIt)->numRefs() == 0)
                    delete (*targetIt);
                parents.erase(*targetIt);
            }
            (*targetIt) = new ConstantNode(val);
            (*targetIt)->addChildNode(this);
            parents.insert(*targetIt);
        }
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    changed = false;
    touchAffected();
}


/** Set element from DAG node */
void DAGNodeContainer::setElement(const IntVector& index, DAGNode* var) {

    // Resize if necessary
    if (index.size() == length.size()) {
        IntVector tempLen  = length;
        bool      growSize = false;
        for (size_t i=0; i<index.size(); i++) {
            if (index[i] >= tempLen[i]) {
                tempLen[i] = index[i] + 1;
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    size_t offset = getOffset(index);

    // Get target iterator
    std::vector<DAGNode*>::iterator targetIt = nodes.begin() + offset;

    // Check if parser wants to set multiple elements
    if (index.size() < length.size()) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw(RbException("Invalid assignment"));

        // Check that the source is a container
        if (var->getDim() == 0)
            throw (RbException("Source does not have elements"));

        DAGNodeContainer* source = dynamic_cast<DAGNodeContainer*>(var);
    
        // Count number of elements
        const IntVector& sourceLen = source->getLength();
        int numSourceElements = 1;
        for (size_t i=0; i<sourceLen.size(); i++)
            numSourceElements *= sourceLen[i];
        int numTargetElements = 1;
        for (size_t i=index.size(); i<length.size(); i++)
            numTargetElements *= length[i];

        // Throw an error if a mismatch
        if (numSourceElements != numTargetElements)
            throw (RbException("Unequal source and target sizes"));

        // Cycle through assignments
        for (ContainerIterator i=source->begin(); i!=source->end(); ++i) {

            // Do the assignment
            const DAGNode* elem = (*source)[i];
            if (Workspace::userWorkspace().isXOfTypeY(elem->getType(), valueType)) {
                if ((*targetIt) != NULL) {
                    (*targetIt)->removeChildNode(this);
                    if ((*targetIt)->numRefs() == 0)
                        delete (*targetIt);
                    parents.erase(*targetIt);
                }
                (*targetIt) = elem->clone();
                (*targetIt)->addChildNode(this);
                parents.insert(*targetIt);
            }
            else
                throw (RbException("Incompatible types"));

            // Increase target iterator
            targetIt++;
        }
        if (var->numRefs() == 0)
            delete var;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (var == NULL || Workspace::userWorkspace().isXOfTypeY(var->getValueType(), valueType)) {
        if ((*targetIt) != NULL) {
            // If conversion of constant to stochastic node, save value
            if ((*targetIt)->isType(ConstantNode_name) &&
                var->isType(StochasticNode_name) &&
                (*targetIt)->getValue() != NULL &&
                (*targetIt)->getValue()->isType(var->getValueType())) {
                ((StochasticNode *)var)->setValue((*targetIt)->getValue()->clone());
            }
            (*targetIt)->removeChildNode(this);
            (*targetIt)->setName("");
            if ((*targetIt)->numRefs() == 0)
                delete (*targetIt);
            parents.erase(*targetIt);
        }
        (*targetIt) = NULL;
        if (var != NULL) {
            if (var->getName() != "")
                throw RbException("Cannot use reference in DAG node container");
            std::ostringstream varName;
            varName << getName();
            for (size_t i=0; i<index.size(); i++)
                varName << "[" << index[i]+1 << "]";
            var->setName(varName.str());
            var->addChildNode(this);
            parents.insert(var);
        }
        (*targetIt) = var;
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    changed = false;
    touchAffected();
}


/** Set element name */
void DAGNodeContainer::setElementName(const IntVector& index, const std::string& name) {

    if (names == NULL)
        names = new StringVector(nodes.size());

    (*names)[getOffset(index)] = RbString(name);
}


/** Set length in each dimension: reorganize the content of the container */
void DAGNodeContainer::setLength(const IntVector& len) {

    if (len.size() != length.size())
        throw (RbException("Cannot change number of dimensions"));

    int numVals = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive dimension length"));
        numVals *= len[i];
    }

    if (numVals != (int)(nodes.size()))
        throw (RbException("Lengths do not match container size"));

    length = len;
}


/** Set name of container and all elements */
void DAGNodeContainer::setName(const std::string& name) {

    DAGNode::setName(name);

    std::vector<DAGNode*>::iterator i=nodes.begin();
    for (ContainerIterator index=begin(); index!=end(); i++, index++) {
        if ((*i) != NULL) {
            std::ostringstream varName;
            varName << name;
            for (size_t k=0; k<index.size(); k++)
                varName << "[" << index[k] + 1 << "]";
            (*i)->setName(varName.str());
        }
    }
}


/** Set value of container */
void DAGNodeContainer::setValue(RbObject* val) {

    throw RbException("Invalid assignment of value to node container");
}


/** Swap parent node in nodes and parents */
void DAGNodeContainer::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    std::vector<DAGNode*>::iterator it = std::find(nodes.begin(), nodes.end(), oldNode);
    if (it == nodes.end())
        throw RbException("Node is not in container");
    it = nodes.erase(it);
    nodes.insert(it, newNode);

    touched = true;
    changed = false;
    touchAffected();
}


/** Get complete info about object */
std::string DAGNodeContainer::toString(void) const {

    std::ostringstream o;

    o << "DAGNode container:" << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << getLength() << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "value       = " << value << std::endl;
    o << "storedValue = " << storedValue;

    return o.str();
}


/** Tell affected nodes that upstream value has been reset */
void DAGNodeContainer::touchAffected(void) {

    touched = true;
    changed = false;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!= children.end(); i++)
        (*i)->touchAffected();
}


/** Update value and stored value */
void DAGNodeContainer::update(void) {

    if (touched && !changed) {
        Container* temp = storedValue;
        storedValue = value;
        value = temp;
        std::vector<DAGNode*>::iterator i = nodes.begin();
        ContainerIterator j = value->begin();
        for (; i!=nodes.end(); i++, j++) {
            if ((*i) == NULL)
                (*value)[j] = NULL;
            else
                (*value)[j] = (*i)->getValue()->clone();
        }
        changed = true;
    }
}


