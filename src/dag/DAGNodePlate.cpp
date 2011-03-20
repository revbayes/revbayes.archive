/**
 * @file
 * This file contains the implementation of DAGNodePlate, which
 * is used to hold vectors or multidimensional arrays of DAG nodes.
 *
 * @brief Implementation of DAGNodePlate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "Container.h"
#include "DAGNodePlate.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>


/** Construct plate from container (type conversion) */
DAGNodePlate::DAGNodePlate(Container* x) : VariableNode(x->getElementType()) {

    for (ContainerIterator i=x->begin(); i!=x->end(); i++) {
        ConstantNode* temp = new ConstantNode(x->getElement(i)->clone());
        nodes.push_back(temp);
        parents.insert(temp);
        temp->addChildNode(this);
    }

    changed      = false;
    length       = x->getLength();
    value        = x;
    storedValue  = value->clone();
    names        = NULL;
}

                                
/** Construct vector with one node x */
DAGNodePlate::DAGNodePlate(DAGNode* x) : VariableNode(x->getDAGType()) {

    nodes.push_back(x);

    parents.insert(x);
    x->addChildNode(this);
    
    changed      = false;
    length       = VectorInteger(1);
    value        = new Container(x->getValue()->clone());
    storedValue  = value->clone();
    names        = NULL;
}


/** Construct vector with n copies of x */
DAGNodePlate::DAGNodePlate(size_t n, DAGNode* x) : VariableNode(x->getDAGType()) {

    nodes.push_back(x);
    for (size_t i=1; i<n; i++)
        nodes.push_back(x->clone());

    for (std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }

    changed      = false;
    length       = VectorInteger(int(n));
    value        = new Container(n, x->getValue()->clone());
    storedValue  = value->clone();
    names        = NULL;
}


/** Construct empty vector of length  n */
DAGNodePlate::DAGNodePlate(size_t n, const std::string& valType)
    : VariableNode(valType) {

    for (size_t i=0; i<n; i++)
        nodes.push_back(NULL);

    changed      = false;
    length       = VectorInteger(int(n));
    value        = new Container(length, valueType);
    storedValue  = value->clone();
    names        = NULL;
}


/** Construct array of given dimensions containing copies of x */
DAGNodePlate::DAGNodePlate(const VectorInteger& len, DAGNode* x)
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
}


/** Construct empty array of given dimensions */
DAGNodePlate::DAGNodePlate(const VectorInteger& len, const std::string& valType)
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
}

/** Copy constructor needed to make sure nodes elements are independent */
DAGNodePlate::DAGNodePlate(const DAGNodePlate& x)
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
}

/** Destructor needed to destroy value, stored value and nodes */
DAGNodePlate::~DAGNodePlate() {

    if (numRefs() != 0)
        throw RbException ("Cannot delete DAGNodePlate '" + getName() + "' with references");

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
DAGNodePlate& DAGNodePlate::operator=(const DAGNodePlate& x) {

    if (this != &x) {

        if (valueType != x.valueType || getDim() != x.getDim())
            throw RbException("Type or dim mismatch");

        /* Remove nodes and parents */
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
DAGNode*& DAGNodePlate::operator[](const VectorInteger& i) {

    return nodes[getOffset(i)];
}


/** Subscript const operator (vector index) */
DAGNode* const& DAGNodePlate::operator[](const VectorInteger& i) const {

    return nodes[getOffset(i)];
}


/** Subscript operator */
DAGNode*& DAGNodePlate::operator[](size_t i) {

    if (i >= nodes.size())
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Subscript const operator */
DAGNode* const& DAGNodePlate::operator[](size_t i) const {

    if (i >= nodes.size())
        throw RbException("Index out of bounds");
    return nodes[i];
}


/** Return begin iterator */
ContainerIterator DAGNodePlate::begin(void) const {

    VectorInteger temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i] = 0;

    return ContainerIterator(temp, getLength());
}


/** Clear plate */
void DAGNodePlate::clear(void) {

    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
        (*i)->removeChildNode(this);
        if ((*i)->numRefs() == 0)
            delete(*i);
    }

    for (size_t i=0; i<length.size(); i++)
        length[i] = 0;

    nodes.clear();
}


/** Clone object */
DAGNodePlate* DAGNodePlate::clone() const {

    return new DAGNodePlate(*this);
}


/** Clone entire graph */
DAGNodePlate* DAGNodePlate::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNodePlate*)(this)) != newNodes.end())
        return (DAGNodePlate*)(newNodes[(DAGNode*)(this)]);

    /* Make pristine copy */
    DAGNodePlate* copy = new DAGNodePlate(length, valueType);
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
ContainerIterator DAGNodePlate::end(void) const {

    VectorInteger temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i]--;

    ContainerIterator tempIt(temp, getLength());
    return ++tempIt;
}


/** Get affected nodes: touch and pass through to next stochastic node */
void DAGNodePlate::getAffected(std::set<StochasticNode*>& affected) {

    /* If we have already touched this node, we are done; otherwise, get the affected children */
    if (!touched) {
        touched = true;
        changed = false;
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->getAffected(affected);
    }
}


/** Get class vector describing type of DAG node */
const VectorString& DAGNodePlate::getDAGClass(void) const {

    static VectorString rbClass = VectorString(DAGNodePlate_name) + VariableNode::getDAGClass();
    return rbClass;
}


/** Get element name */
std::string DAGNodePlate::getElementName(const VectorInteger& index) const {

    if (names == NULL)
        return "";
    else
        return (*names)[getOffset(index)];
}


/** Get offset to element or subcontainer; also check index */
size_t DAGNodePlate::getOffset(const VectorInteger& index) const {

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
const RbObject* DAGNodePlate::getStoredValue(void) {

    if (!touched)
        return value;

    update();
    return storedValue;
}


/** Get subcontainer */
DAGNodePlate* DAGNodePlate::getSubPlate(const VectorInteger& index) const {

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Create a new vector of the right size
    VectorInteger tempLength;
    for (size_t i=index.size(); i<length.size(); i++)
        tempLength.push_back(length[i]);
    DAGNodePlate* temp = new DAGNodePlate(tempLength, valueType);

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
const RbObject* DAGNodePlate::getValElement(const VectorInteger& index) const {

    // Check that the index is to a value element
    if (int(index.size()) < getDim())
        throw (RbException("Index error: Not value element"));

    // Split the index up
    VectorInteger containerIndex = index;
    containerIndex.resize(length.size());
    VectorInteger valueIndex;
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
    else if (!(element->getDim() > 0))
        throw RbException("Object does not have elements");
    else
        return (element->getValElement(valueIndex));
}


/** Get value */
const RbObject* DAGNodePlate::getValue(void) {

    update();
    return value;
}


/** Get const value if possible */
const RbObject* DAGNodePlate::getValue(void) const {

    if (touched && !changed)
        throw RbException("Constant value not available");

    return value;
}


/** Get variable element */
const DAGNode* DAGNodePlate::getVarElement(const VectorInteger& index) const {

    // Check that the index is to a variable element
    if (index.size() != length.size())
        throw (RbException("Index error: Not index to variable"));

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Get non-const reference to variable element */
DAGNode* DAGNodePlate::getElementReference(const VectorInteger& index) {

    // Check that the index is to a variable element
    if (index.size() != length.size())
        throw (RbException("Index error: Not index to variable"));

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Is it possible to mutate to newNode? */
bool DAGNodePlate::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to mutate node to contain newValue? */
bool DAGNodePlate::isMutableTo(const VectorInteger& index, const RbObject* newValue) const {

    assert (!newValue->isType(Container_name));
    
    bool isMutable = false;

    // Divide into plate index and value index
    VectorInteger plateIndex;
    VectorInteger valueIndex;
    for (size_t i=0; i<index.size(); i++) {
        if (i < (size_t)(getDim()))
            plateIndex.push_back(index[i]);
        else
            valueIndex.push_back(index[i]);
    }

    // Create a temp plate for the dry run
    DAGNodePlate* temp = NULL;

    // Check whether it is a container conversion or a value conversion
    if (plateIndex.size() < (size_t)(getDim())) {
        
        // Container conversion
        VectorInteger newLen;
        for (size_t i=0; i<plateIndex.size(); i++)
            newLen.push_back(length[i]);
        temp = new DAGNodePlate(newLen, newValue->getType());
        isMutable = isMutableTo(temp);
    }
    else {
    
        // Value conversion
        std::string refType;
        std::string compType;
        bool foundFirst = false;
        isMutable = true;
        for (std::vector<DAGNode*>::const_iterator i=nodes.begin(); i!=nodes.end(); i++) {
            if ( (*i) == NULL || (*i)->getValue() == NULL )
                continue;
            if ( foundFirst == false ) {
                if ( !(*i)->getValue()->isMutableTo(valueIndex, newValue, refType) ) {
                    isMutable = false;
                    break;
                }
                foundFirst = true;
            }
            else {
                if ( !(*i)->getValue()->isMutableTo(valueIndex, newValue, compType) || compType != refType) {
                    isMutable = true;
                    break;
                }
            }
        }
        if (isMutable)
            temp = new DAGNodePlate(length, refType);
    }

    // Finally ask children
    if (isMutable) {
        for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
            if ( !(*i)->isParentMutableTo(this, temp) ) {
                isMutable = false;
                break;
            }
        }
    }
    
    // Delete the temp plate
    if (temp != NULL)
        delete temp;
    
    return isMutable;
}


/** Is it possible to change parent node oldNode to newNode? */
bool DAGNodePlate::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // Check that all parents except the caller are convertible to the new type and dim
    for (std::set<DAGNode*>::const_iterator i=parents.begin(); i!=parents.end(); i++) {
        if ( (*i) != NULL && (*i) != const_cast<DAGNode*>(oldNode) ) {
            if ( !(*i)->getValue()->isConvertibleTo(newNode->getValueType(), newNode->getDim()) )
                return false;
        }
    }
    
    // Check that all children allow this node to permute to the new type and dim
    DAGNodePlate* temp = new DAGNodePlate(length, newNode->getValueType());
    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        if ( !(*i)->isParentMutableTo(this, temp) ) {
            delete temp;
            return false;
        }
    }
    delete temp;

    return true;
}


/** Keep value of node and affected variable nodes */
void DAGNodePlate::keepAffected(void) {

    if (touched) {
        if (!changed)
            update();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->keepAffected();
    }
    touched = changed = false;
}


/** Mutate to newNode */
void DAGNodePlate::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
    
}


/* Mutate to contain newValue */
DAGNodePlate* DAGNodePlate::mutateTo(const VectorInteger& index, RbObject* newValue) {

    throw RbException("Not implemented yet");
}


/** Print struct for user */
void DAGNodePlate::printStruct(std::ostream& o) const {

    if (touched && !changed)
        throw RbException ("Cannot print value while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class  = " << getDAGClass() << std::endl;
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
void DAGNodePlate::printValue(std::ostream& o) const {

    if (touched && !changed)
        throw RbException ("Cannot print value while in touched state");

    value->printValue(o);
}


/** Reset plate length in different dimensions */
void DAGNodePlate::resize(const VectorInteger& len) {

    // Check if there is anything to do
    if (len.equals(&length))
        return;

    // Check that new length specification has the right number of dimensions
    if (len.size() != length.size())
        throw (RbException("Cannot resize plate: dimensions do not match"));

    // Calculate handy numbers
    VectorInteger numValsSource = VectorInteger(len.size(), 0), numValsTarget = VectorInteger(len.size(), 0);
    int numSourceVals = 1, numTargetVals = 1;
    for (int i=int(len.size())-1; i>=0; i--) {
        numSourceVals *= length[i];
        numTargetVals *= len[i];
        numValsSource[i] = numSourceVals;
        numValsTarget[i] = numTargetVals;
    }

    // Create new empty nodes vector
    VectorString* tempNames;
    std::vector<DAGNode*> tempNodes = std::vector<DAGNode*>(numValsTarget[0]);
    if (names != NULL)
        tempNames = new VectorString(numValsTarget[0]);

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

    // Copy the new plate and length specification
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
void DAGNodePlate::restoreAffected(void) {

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


/** Get complete info about object */
std::string DAGNodePlate::richInfo(void) const {

    std::ostringstream o;

    o << "DAGNode plate:" << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << getLength() << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "value       = " << value << std::endl;
    o << "storedValue = " << storedValue;

    return o.str();
}


/** Set value element or elements from value */
void DAGNodePlate::setElement(const VectorInteger& index, RbObject* val) {

    // Resize if necessary
    if (index.size() == length.size()) {
        VectorInteger tempLen  = length;
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
        const VectorInteger& sourceLen = source->getLength();
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
                if ((*targetIt) != NULL && (*targetIt)->isDAGType(StochasticNode_name))
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
        if ((*targetIt) != NULL && (*targetIt)->isDAGType(StochasticNode_name)) {
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
void DAGNodePlate::setElement(const VectorInteger& index, DAGNode* var) {

    // Resize if necessary
    if (index.size() == length.size()) {
        VectorInteger tempLen  = length;
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

        // Check that the source is a plate
        if (var->getDim() == 0)
            throw (RbException("Source does not have elements"));

        DAGNodePlate* source = dynamic_cast<DAGNodePlate*>(var);
    
        // Count number of elements
        const VectorInteger& sourceLen = source->getLength();
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
            if (Workspace::userWorkspace().isXOfTypeY(elem->getValueType(), valueType)) {
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
            delete var;     // We are responsible for deleting the useless plate
        return;
    }

    // Parser wants to set a single element
    if (var == NULL || Workspace::userWorkspace().isXOfTypeY(var->getValueType(), valueType)) {
        if ((*targetIt) != NULL) {
            // If conversion of constant to stochastic node, save value
            if ((*targetIt)->isDAGType(ConstantNode_name) &&
                var->isDAGType(StochasticNode_name) &&
                (*targetIt)->getValue() != NULL &&
                (*targetIt)->getValue()->isType(var->getValueType())) {
                ((StochasticNode *)var)->setValue((*targetIt)->getValue()->clone());
            }
            (*targetIt)->removeChildNode(this);
            if ((*targetIt)->numRefs() == 0)
                delete (*targetIt);
            parents.erase(*targetIt);
        }
        (*targetIt) = NULL;
        if (var != NULL) {
            if (var->getName() != "")
                throw RbException("Cannot use reference in setting element of DAG node plate");
            std::ostringstream varName;
            varName << getName();
            for (size_t i=0; i<index.size(); i++)
                varName << "[" << index[i]+1 << "]";
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


/** Set length in each dimension: reorganize the content of the plate */
void DAGNodePlate::setLength(const VectorInteger& len) {

    if (len.size() != length.size())
        throw (RbException("Cannot change number of dimensions"));

    int numVals = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive dimension length"));
        numVals *= len[i];
    }

    if (numVals != (int)(nodes.size()))
        throw (RbException("Lengths do not match plate size"));

    length = len;
}


/** Set value of plate */
void DAGNodePlate::setValue(RbObject* val) {

    throw RbException("Invalid assignment of value to node plate");
}


/** Swap parent node in nodes and parents */
void DAGNodePlate::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    std::vector<DAGNode*>::iterator it = std::find(nodes.begin(), nodes.end(), oldNode);
    if (it == nodes.end())
        throw RbException("Node is not in plate");
    it = nodes.erase(it);
    nodes.insert(it, newNode);

    touched = true;
    changed = false;
    touchAffected();
}


/** Tell affected nodes that upstream value has been reset */
void DAGNodePlate::touchAffected(void) {

    touched = true;
    changed = false;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!= children.end(); i++)
        (*i)->touchAffected();
}


/** Update value and stored value */
void DAGNodePlate::update(void) {

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


