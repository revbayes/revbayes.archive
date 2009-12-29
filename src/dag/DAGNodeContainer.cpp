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
#include "ContainerIterator.h"
#include "DAGNodeContainer.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <iostream>
#include <sstream>


/** Construct vector with one node x */
DAGNodeContainer::DAGNodeContainer(DAGNode* x) : VariableNode(x->getType()) {

    nodes.push_back(x);

    parents.insert(x);
    x->addChildNode(this);
    
    changed     = false;
    length      = IntVector(1);
    value       = new Container(x->getValue()->clone());
    storedValue = value->clone();
}


/** Construct vector with n copies of x */
DAGNodeContainer::DAGNodeContainer(int n, DAGNode* x) : VariableNode(x->getType()) {

    nodes.push_back(x);
    for (int i=1; i<n; i++)
        nodes.push_back(x->clone());

    for (int i=0; i<n; i++) {
        parents.insert(nodes[i]);
        nodes[i]->addChildNode(this);
    }

    changed     = false;
    length      = IntVector(n);
    value       = new Container(n, x->getValue()->clone());
    storedValue = value->clone();
}


/** Construct empty vector of length  n */
DAGNodeContainer::DAGNodeContainer(int n, const std::string& valType)
    : VariableNode(valType) {

    for (int i=0; i<n; i++)
        nodes.push_back(NULL);

    changed     = false;
    length      = IntVector(n);
    value       = new Container(length, valueType);
    storedValue = value->clone();
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

    for (int i=0; i<n; i++) {
        parents.insert(nodes[i]);
        nodes[i]->addChildNode(this);
    }

    changed     = false;
    length      = len;
    value       = new Container(len, x->getValue()->clone());
    storedValue = value->clone();
}


/** Construct empty array of given dimensions */
DAGNodeContainer::DAGNodeContainer(const IntVector& len, const std::string& valType)
    : VariableNode(valType) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive length"));
        n *= len[i];
    }

    for (int i=0; i<n; i++)
        nodes.push_back(NULL);

    changed     = false;
    length      = len;
    value       = new Container(len, valueType);
    storedValue = value->clone();
}


/** Copy constructor needed to make sure nodes elements are independent */
DAGNodeContainer::DAGNodeContainer(const DAGNodeContainer& x)
    : VariableNode(x) {

    changed = x.changed;
    length  = x.length;

    for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++)
        nodes.push_back((*i)->clone());

    value       = x.value->clone();
    storedValue = x.storedValue->clone();
}


/** Destructor needed to destroy value and stored value; nodes are deleted through parents */
DAGNodeContainer::~DAGNodeContainer() {

    delete value;
    delete storedValue;
}


/** Assignment operator; make sure we get independent nodes */
DAGNodeContainer& DAGNodeContainer::operator=(const DAGNodeContainer& x) {

    if (this != &x) {
        VariableNode::operator=(x);

        changed = x.changed;
        length  = x.length;

        nodes.clear();
        for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++)
            nodes.push_back((*i)->clone());         

        value       = x.value->clone();
        storedValue = x.storedValue->clone();
    }

    return (*this);
}


/** Subscript operator (container iterator) */
DAGNode*& DAGNodeContainer::operator[](const ContainerIterator& i) {

    int offset = getOffset(i);
    return nodes[offset];
}


/** Subscript const operator (container iterator) */
DAGNode* const& DAGNodeContainer::operator[](const ContainerIterator& i) const {

    int offset = getOffset(i);
    return nodes[offset];
}


/** Subscript operator */
DAGNode*& DAGNodeContainer::operator[](int i) {

    if (i < 0 || i > int(nodes.size()))
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Subscript const operator */
DAGNode* const& DAGNodeContainer::operator[](int i) const {

    if (i < 0 || i > int(nodes.size()))
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Clone function */
DAGNodeContainer* DAGNodeContainer::clone() const {

    return new DAGNodeContainer(*this);
}


/** Get end iterator */
ContainerIterator DAGNodeContainer::end(void) const {

    IntVector   temp;
    for (size_t i=0; i!=length.size(); ++i)
        temp.push_back(length[i]-1);

    return ++ContainerIterator(temp);
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


/** Get offset to element or subcontainer; also check index */
int DAGNodeContainer::getOffset(const ContainerIterator& index) const {

    if (index.size() > length.size())
        throw (RbException("Too many indices"));
    for (size_t i=0; i<index.size(); i++) {
        if (index[i] < 0 || index[i] >= length[i])
            throw (RbException("Index out of range"));
    }

    int offset = 0;
    int numVals = 1;
    size_t i;
    for (i=length.size()-1; i>=index.size(); i--)
         numVals *= length[i];
    for (; i>=0; i--) {
        offset += (index[i] * numVals);
        numVals *= length[i];
    }
 
    return offset;
}


/** Get offset to element or subcontainer; also check index */
int DAGNodeContainer::getOffset(const IntVector& index) const {

    if (index.size() > length.size())
        throw (RbException("Too many indices"));
    for (size_t i=0; i<index.size(); i++) {
        if (index[i] < 0 || index[i] >= length[i])
            throw (RbException("Index out of range"));
    }

    int offset = 0;
    int numVals = 1;
    size_t i;
    for (i=length.size()-1; i>=index.size(); i--)
         numVals *= length[i];
    for (; i>=0; i--) {
        offset += (index[i] * numVals);
        numVals *= length[i];
    }
 
    return offset;
}


/** Get stored value */
const RbObject* DAGNodeContainer::getStoredValue(void) {

    update();
    return storedValue;
}


/** Get subcontainer */
DAGNodeContainer* DAGNodeContainer::getSubContainer(const IntVector& index) const {

    // Get offset; this throws an error if something wrong with index
    int offset = getOffset(index);

    // Create a new vector of the right size
    IntVector tempLength;
    for (size_t i=index.size(); i<length.size(); i++)
        tempLength.push_back(length[i]);
    DAGNodeContainer *temp = new DAGNodeContainer(tempLength, valueType);

    // Fill it with content
    int numSubvals = 1;
    for (size_t i=length.size()-1; i>=index.size(); i--)
        numSubvals *= length[i];
    for (int i=0; i<numSubvals; i++)
        temp->nodes[i] = nodes[i+offset];

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
    int offset = getOffset(containerIndex);

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


/** Get value (const) */
const RbObject* DAGNodeContainer::getValue(void) const {

    if (touched && !changed)
        throw RbException("Value is out of date");

    return value;
}


/** Get variable element */
const DAGNode* DAGNodeContainer::getVarElement(const IntVector& index) const {

    // Check that the index is to a variable element
    if (index.size() != length.size())
        throw (RbException("Index error: Not index to variable"));

    // Get offset; this throws an error if something wrong with index
    int offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Keep value of node and affected variable nodes */
void DAGNodeContainer::keepAffected(void) {

    if (touched) {
        if (!changed)
            throw RbException("Keep called without previous update");
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->keepAffected();
    }
    touched = changed = false;
}


/** Print struct for user */
void DAGNodeContainer::printStruct(std::ostream& o) const {

    o << "Wrapper:" << std::endl;
    o << "&.class  = " << getClass() << std::endl;
    o << "&.dim    = " << getDim() << std::endl;
    o << "&.length = " << getLength() << std::endl;
    o << "&.value  = " << getValue() << std::endl;
}


/** Print value for user */
void DAGNodeContainer::printValue(std::ostream& o) const {

    getValue()->printValue(o);
}


/** Reset container length in different dimensions */
void DAGNodeContainer::resize(const IntVector& len) {

    // Check if there is anything to do
    if (len.equals(&length))
        return;

    // Check that new length specification has the right number of dimensions
    if (len.size() != length.size())
        throw (RbException("Cannot resize container: dimensions do not match"));

    // Check new length specification is OK
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] < length[i])
            throw (RbException("Cannot shrink container"));
    }

    // Calculate handy numbers
    IntVector numValsSource = IntVector(len.size()), numValsTarget = IntVector(len.size());
    int numSourceVals = 1, numTargetVals = 1;
    for (int i=len.size()-1; i>=0; i--) {
        numSourceVals *= length[i];
        numTargetVals *= len[i];
        numValsSource[i] = numSourceVals;
        numValsTarget[i] = numTargetVals;
    }

    // Create new empty nodes vector
    std::vector<DAGNode*> tempNodes = std::vector<DAGNode*>(numValsTarget[0]);

    // Reorganize content
    int sourceIndex = 0, targetIndex = 0;
    do {
        for (int i=0; i<length[length.size()-1]; i++)
            tempNodes[targetIndex++] = nodes[sourceIndex++];

        for (int i=length.size()-1; i>=0; i--) {
            if (sourceIndex % numValsSource[i] == 0) {
                targetIndex += numValsTarget[i] - (targetIndex % numValsTarget[i]);
            }
        }
    } while (sourceIndex < (int)(nodes.size()));

    // Copy the new container and length specification
    nodes  = tempNodes;
    length = len;
}


/** Restore value of node and affected variable nodes */
void DAGNodeContainer::restoreAffected(void) {

    if (touched) {
        if (!changed)
            throw RbException("Restore called without previous update");
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->restoreAffected();
        }
        Container* temp = value;
        value = storedValue;
        storedValue = temp;
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
            if (index[i] > tempLen[i]) {
                tempLen[i] = index[i];
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    int offset = getOffset(index);

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
        for (ContainerIterator i=source->begin(); i<source->end(); ++i) {

            // Do the assignment
            const RbObject* elem = source->getElement(i);
            if (elem->isType(getValueType())) {
                if (nodes[offset] != NULL && nodes[offset]->isType(StochasticNode_name))
                    ((StochasticNode*)(nodes[offset]))->clamp(elem->clone());
                else {
                    if (nodes[offset] != NULL) {
                        nodes[offset]->removeChildNode(this);
                        if (nodes[offset]->numRefs() == 0)
                            delete nodes[offset];
                        parents.erase(nodes[offset]);
                    }
                    nodes[offset] = new ConstantNode(elem->clone());
                    nodes[offset]->addChildNode(this);
                    parents.insert(nodes[offset]);
                }
            }
            else
                throw (RbException("Incompatible types"));

            // Increase offset for target nodes
            offset++;
        }
        delete val;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (val->isType(getValueType())) {
        if (nodes[offset] != NULL && nodes[offset]->isType(StochasticNode_name))
            ((StochasticNode*)(nodes[offset]))->clamp(val);
        else {
            if (nodes[offset] != NULL) {
                nodes[offset]->removeChildNode(this);
                if (nodes[offset]->numRefs() == 0)
                    delete nodes[offset];
                parents.erase(nodes[offset]);
            }
            nodes[offset] = new ConstantNode(val);
            nodes[offset]->addChildNode(this);
            parents.insert(nodes[offset]);
        }
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    std::set<StochasticNode*> temp;
    getAffected(temp);
}


/** Set element from DAG node */
void DAGNodeContainer::setElement(const IntVector& index, DAGNode* var) {

    // Resize if necessary
    if (index.size() == length.size()) {
        IntVector tempLen  = length;
        bool      growSize = false;
        for (size_t i=0; i<index.size(); i++) {
            if (index[i] > tempLen[i]) {
                tempLen[i] = index[i];
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    int offset = getOffset(index);

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
            const DAGNode* elem = source->getVarElement(i);
            if (Workspace::userWorkspace().isXOfTypeY(elem->getType(), valueType)) {
                if (nodes[offset] != NULL) {
                    nodes[offset]->removeChildNode(this);
                    if (nodes[offset]->numRefs() == 0)
                        delete nodes[offset];
                    parents.erase(nodes[offset]);
                }
                nodes[offset] = elem->clone();
                nodes[offset]->addChildNode(this);
                parents.insert(nodes[offset]);
            }
            else
                throw (RbException("Incompatible types"));

            // Increase offset for target nodes
            offset++;
        }
        if (var->numRefs() == 0)
            delete var;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (var == NULL || Workspace::userWorkspace().isXOfTypeY(var->getType(), valueType)) {
        if (nodes[offset] != NULL) {
            nodes[offset]->removeChildNode(this);
            if (nodes[offset]->numRefs() == 0)
                delete nodes[offset];
            parents.erase(nodes[offset]);
        }
        nodes[offset] = var;
        nodes[offset]->addChildNode(this);
        parents.insert(nodes[offset]);
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    std::set<StochasticNode*> temp;
    getAffected(temp);
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


/** Set value of container */
void DAGNodeContainer::setValue(RbObject* val) {

    throw RbException("Invalid assignment of value to node container");
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


/** Update value and stored value */
void DAGNodeContainer::update(void) {

    if (touched && !changed) {
        Container* temp = storedValue;
        storedValue = value;
        value = temp;
        std::vector<DAGNode*>::iterator i;
        ContainerIterator j;
        for (i=nodes.begin(), j=value->begin(); i!=nodes.end(); i++, j++) {
            (*value)[j] = (*i)->getValue()->clone();
        }
        changed = true;
    }
}


