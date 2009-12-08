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
#include "DAGNodeContainer.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"

#include <iostream>
#include <sstream>


/** Construct vector with one node x */
DAGNodeContainer::DAGNodeContainer(DAGNode* x) {

    nodes.push_back(x);

    valueType = x->getValue()->getType();
    length    = IntVector(1);
    value     = NULL;
}


/** Construct vector with n copies of x */
DAGNodeContainer::DAGNodeContainer(int n, DAGNode* x) {

    nodes.push_back(x);
    for (int i=1; i<n; i++)
        nodes.push_back((DAGNode*)(x->clone()));

    valueType = x->getValue()->getType();
    length    = IntVector(n);
    value     = NULL;       // Fill in if needed
}


/** Construct empty vector of length  n */
DAGNodeContainer::DAGNodeContainer(int n, const std::string& type) {

    for (int i=0; i<n; i++)
        nodes.push_back(NULL);

    valueType = type;
    length    = IntVector(n);
    value     = NULL;       // Fill in if needed
}


/** Construct array of given dimensions containing copies of x */
DAGNodeContainer::DAGNodeContainer(const IntVector& len, DAGNode* x) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<length.size(); i++) {
        if (length[i] <= 0)
            throw (RbException("Nonpositive length"));
        n *= length[i];
    }

    nodes.push_back(x);
    for (int i=1; i<n; i++)
        nodes.push_back((DAGNode*)(x->clone()));

    valueType = x->getValue()->getType();
    length    = len;
    value     = NULL;       // Fill in if needed
}


/** Construct empty array of given dimensions */
DAGNodeContainer::DAGNodeContainer(const IntVector& len, const std::string& type) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<length.size(); i++) {
        if (length[i] <= 0)
            throw (RbException("Nonpositive length"));
        n *= length[i];
    }

    for (int i=0; i<n; i++)
        nodes.push_back(NULL);

    valueType = type;
    length    = len;
    value     = NULL;       // Fill in if needed
}


/** Copy constructor needed to make sure nodes elements are independent */
DAGNodeContainer::DAGNodeContainer(const DAGNodeContainer& x)
    : RbObjectWrapper(x) {

    for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++)
        nodes.push_back((DAGNode*)(*i)->clone());

    if (x.value == NULL)
        value = NULL;
    else
        value = x.value->clone();       // Fill in if needed
}


/** Destructor needed to destroy elements, and value, if it has been filled in */
DAGNodeContainer::~DAGNodeContainer() {

    for (std::vector<DAGNode*>::const_iterator i=nodes.begin(); i!=nodes.end(); i++)
        delete (*i);
    
    delete value;
}


/** Assignment operator; make sure we get independent nodes */
DAGNodeContainer& DAGNodeContainer::operator=(const DAGNodeContainer& x) {

    if (this != &x) {
        RbObjectWrapper::operator=(x);

        valueType = x.valueType;
        length = x.length;

        for (std::vector<DAGNode*>::iterator i=nodes.begin(); i!=nodes.end(); i++)
            delete (*i);
        nodes.clear();

        for (std::vector<DAGNode*>::const_iterator i=x.nodes.begin(); i!=x.nodes.end(); i++)
            nodes.push_back((DAGNode*)((*i)->clone()));         
    }

    value     = NULL;       // Fill in if needed

    return (*this);
}


/** Clone function */
DAGNodeContainer* DAGNodeContainer::clone() const {

    return new DAGNodeContainer(*this);
}


/** Get class vector describing type of object */
const StringVector& DAGNodeContainer::getClass() const {

    static StringVector rbClass = StringVector(RbNames::DAGNodeContainer::name) + RbObjectWrapper::getClass();
    return rbClass;
}


/** Get element */
const DAGNode* DAGNodeContainer::getElement(const IntVector& index) const {

    // Check that the index is to an element
    if (index.size() != length.size())
        throw (RbException("Not index to element"));

    // Get offset; this throws an error if something wrong with index
    int offset = getOffset(index);

    // Return element
    return nodes[offset];
}


/** Get next index so we can iterate through a container */
void DAGNodeContainer::getNextIndex(IntVector& index, const IntVector& len) const {

    for (int i=len.size()-1; i>=0; i--) {
        index[i]++;
        if (index[i] < len[i])
            break;
        else
            index[i] = 0;
    }
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


//** Get subcontainer */
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


/** Get value: convert to object container with the values */
const RbObject* DAGNodeContainer::getValue(void) const {

    // TODO: Implement

    // Allocate object container if needed

    // Cycle through container and set object container values

    // Return object container

    return NULL;
}


/** Set element or elements from object */
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

        // Check that the source is a container
        RbComplex* source = dynamic_cast<RbComplex*>(val);
        if (source->getElementDim() == 0)
            throw (RbException("Source does not have elements"));
    
        // Count number of elements
        const IntVector& sourceLen = source->getElementLength();
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
        for (IntVector i=IntVector(sourceLen.size(), 0); i[0]<sourceLen[0]; getNextIndex(i, sourceLen)) {

            // Do the assignment
            const RbObject* elem = source->getElement(i);
            if (elem->isType(valueType)) {
                delete nodes[offset];
                nodes[offset] = new ConstantNode(elem->clone());
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
    if (val->isType(valueType)) {
        delete nodes[offset];
        nodes[offset] = new ConstantNode(val);
    }
    else
        throw (RbException("Incompatible types"));
}


/** Set element from DAG node wrapper */
void DAGNodeContainer::setElement(const IntVector& index, RbObjectWrapper* val) {

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

        // Check that the source is a container
        DAGNodeContainer* source = dynamic_cast<DAGNodeContainer*>(val);
        if (source->getElementDim() == 0)
            throw (RbException("Source does not have elements"));
    
        // Count number of elements
        const IntVector& sourceLen = source->getElementLength();
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
        for (IntVector i=IntVector(sourceLen.size(), 0); i[0]<sourceLen[0]; getNextIndex(i, sourceLen)) {

            // Do the assignment
            const DAGNode* elem = source->getElement(i);
            if (elem->getValue()->isType(valueType)) {
                delete nodes[offset];
                nodes[offset] = elem->clone();
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
    DAGNode* theNode = dynamic_cast<DAGNode*>(val);
    if (theNode == NULL)
        throw (RbException("Unknown wrapper type"));
    if (theNode->getValue()->isType(valueType)) {
        delete nodes[offset];
        nodes[offset] = theNode;
    }
    else
        throw (RbException("Incompatible types"));
}


/** Print value for user */
void DAGNodeContainer::printValue(std::ostream& o) const {

    if (length.size() != 2) {
        o << "[";
        for (std::vector<DAGNode*>::const_iterator i = nodes.begin(); i!= nodes.end(); i++) {
            if (i != nodes.begin())
                o << ", ";
            (*i)->getValue()->printValue(o);
        }
        o <<  "]";
    }
    else if (length.size() == 2) {
        std::vector<DAGNode*>::const_iterator i=nodes.begin();
        for (int j=0; j<length[0]; j++) {
            o << "[";
            for (int k=0; k<length[1]; k++, i++) {
                if (i != nodes.begin())
                    o << ", ";
                (*i)->getValue()->printValue(o);
            }
            o <<  "]";
        }
    }
}


/** Reset container sizes in different dimensions */
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


/** Set element length: reorganize the content of the container */
void DAGNodeContainer::setElementLength(const IntVector& len) {

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


/** Get complete info about object */
std::string DAGNodeContainer::toString(void) const {

    std::ostringstream o;
    o <<  briefInfo() << std::endl;
    printValue(o);

    return o.str();
}

