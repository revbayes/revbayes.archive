/**
 * @file
 * This file contains the implementation of Container,
 * which is a generic container of RbObject objects.
 *
 * @brief Implementation of Container
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

#include "Container.h"
#include "ContainerIterator.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"
#include "Simplex.h"
#include "VectorString.h"

#include <iostream>
#include <sstream>



/** Construct empty vector, should only be used by parser */
Container::Container(void)
    : RbComplex(), elementType(RbObject_name) {

    length = VectorInteger(0);
}

/** Construct vector with one node x */
Container::Container(RbObject* x)
    : RbComplex(), elementType(x->getType()) {

    elements.push_back(x);

    length = VectorInteger(1);
}


/** Construct vector with n copies of x */
Container::Container(size_t n, RbObject* x)
    : RbComplex(), elementType(x->getType()) {

    elements.push_back(x);
    for (size_t i=1; i<n; i++)
        elements.push_back(x->clone());

    length       = VectorInteger(int(n));
}


/** Construct array of given dimensions containing copies of x */
Container::Container(const VectorInteger& len, RbObject* x)
    : RbComplex(), elementType(x->getType()) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive length"));
        n *= len[i];
    }

    elements.push_back(x);
    for (int i=1; i<n; i++)
        elements.push_back(x->clone());

    length       = len;
}


/** Construct empty array of given dimensions */
Container::Container(const VectorInteger& len, const std::string& elemType)
    : RbComplex(), elementType(elemType) {

    if (len.size() == 0)
        throw (RbException("Length specification empty"));

    int n = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] < 0)
            throw (RbException("Negative length"));
        n *= len[i];
    }

    for (int i=0; i<n; i++)
        elements.push_back(NULL);

    length       = len;
}


/** Copy constructor needed to make sure elements are independent */
Container::Container(const Container& x)
    : RbComplex(x), elementType(x.elementType) {

    length  = x.length;

    for (std::vector<RbObject*>::const_iterator i=x.elements.begin(); i!=x.elements.end(); i++) {
        if ((*i) != NULL)
            elements.push_back((*i)->clone());
        else
            elements.push_back(NULL);
    }

    length       = x.length;
}


/** Destructor needed to destroy elements */
Container::~Container() {

    for (std::vector<RbObject*>::const_iterator i=elements.begin(); i!=elements.end(); i++)
        delete (*i);
}


/** Assignment operator; make sure we get independent elements */
Container& Container::operator=(const Container& x) {

    if (this != &x) {

        std::vector<RbObject*>::const_iterator i;
        for (i=x.elements.begin(); i!=x.elements.end(); i++) {
            if (!(*i)->isType(elementType))
                break;
        }
        if (i != x.elements.end())
            throw RbException("Incompatible types in container assignment");

        RbComplex::operator=(x);

        length       = x.length;

        for (std::vector<RbObject*>::iterator i=elements.begin(); i!=elements.end(); i++)
            delete (*i);
        elements.clear();

        for (std::vector<RbObject*>::const_iterator i=x.elements.begin(); i!=x.elements.end(); i++)
            elements.push_back((*i)->clone());
    }
    return (*this);
}


/** Subscript operator (container iterator) */
RbObject*& Container::operator[](const VectorInteger& i) {

    size_t offset = getOffset(i);
    return elements[offset];
}


/** Subscript const operator (container iterator) */
RbObject* const& Container::operator[](const VectorInteger& i) const {

    size_t offset = getOffset(i);
    return elements[offset];
}


/** Subscript operator */
RbObject*& Container::operator[](size_t i) {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Subscript const operator */
RbObject* const& Container::operator[](size_t i) const {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Clone function */
Container* Container::clone(void) const {

    return new Container(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Container::convertTo(const std::string& type) const {

    throw RbException("Cannot convert Container to " + type + ".");
	
	return NULL;
}

/** Pointer-based equals comparison */
bool Container::equals(const RbObject* obj) const {

    const Container* p = dynamic_cast<const Container*>(obj);
    if (p == NULL)
        return false;

    if (length != p->length || size() != p->size())
        return false;

    bool result = true;
    std::vector<RbObject*>::const_iterator i, j;
    for (i=elements.begin(), j=p->elements.begin(); i!=elements.end(); i++, j++)
        result = result && (*i)->equals(*j);

    return result;
}


/** Get class vector describing type of object */
const VectorString& Container::getClass(void) const {

    static VectorString rbClass = VectorString(Container_name) + RbComplex::getClass();
    return rbClass;
}


/** Get offset to element or subcontainer; also check index */
size_t Container::getOffset(const VectorInteger& index) const {

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


/** Get subcontainer */
Container* Container::getSubContainer(const VectorInteger& index) const {

    // Get offset; this throws an error if something wrong with index
    size_t offset = getOffset(index);

    // Create a new vector of the right size
    VectorInteger tempLength;
    for (size_t i=index.size(); i<length.size(); i++)
        tempLength.push_back(length[i]);
    Container *temp = new Container(tempLength, getElementType());

    // Fill it with content
    int numSubvals = 1;
    for (size_t i=length.size()-1; i>=index.size(); i--)
        numSubvals *= length[i];
    for (int i=0; i<numSubvals; i++)
        temp->elements[i] = elements[i+offset];

    return temp;
}


/** Get element (read-only) */
const RbObject* Container::getElement(const VectorInteger& index) const {

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
    RbObject* element = elements[offset];

    // Return value
    if (element == NULL)
        return NULL;
    if (valueIndex.size() == 0)
        return element;
    else if (!element->isType(RbComplex_name))
        throw RbException("Object does not have elements");
    else
        return ((RbComplex*)(element))->getElement(valueIndex);
}


/** Get element ptr */
RbObject* Container::getElementPtr(const VectorInteger& index) {

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
    RbObject* element = elements[offset];

    // Return value
    if (element == NULL)
        return NULL;
    if (valueIndex.size() == 0)
        return element;
    else if (!element->isType(Container_name))
        throw RbException("Object does not have elements");
    else
        return ((Container*)(element))->getElementPtr(valueIndex);
}

/** Convert to object of another class. The caller manages the object. */
bool Container::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Set value element or elements from value */
void Container::setElement(const VectorInteger& index, RbObject* val) {

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

    // Check if parser wants to set multiple elements
    if (index.size() < length.size()) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw(RbException("Invalid assignment: Container to subcontainer"));

        // Check that the source is a container
        if (!val->isType(RbComplex_name))
            throw (RbException("Source does not have elements"));
        RbComplex* source = dynamic_cast<RbComplex*>(val);
    
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
            if (elem->isType(getElementClass()[0])) {
                if (elements[offset] != NULL)
                    delete elements[offset];
                elements[offset] = elem->clone();
            }
            else
                throw (RbException("Incompatible types"));

            // Increase offset for target elements
            offset++;
        }
        delete val;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (val->isType(getElementClass()[0])) {
        if (elements[offset] != NULL)
            delete elements[offset];
        elements[offset] = val;
    }
    else
        throw (RbException("Incompatible types"));
}


/** Print value for user */
void Container::printValue(std::ostream& o) const {

    if (length.size() == 1) {
        o << "{";
        for (std::vector<RbObject*>::const_iterator i = elements.begin(); i!= elements.end(); i++) {
            if (i != elements.begin())
                o << ", ";
            if ((*i) == NULL)
                o << "NULL";
            else
                (*i)->printValue(o);
        }
        o <<  "}";
    }
    else if (length.size() == 2) {
        // TODO: Print beautiful matrix
    }
    else {
        ContainerIterator index=begin();
        while (index!=end()) {
            /* print one row in the last dimension at a time */
            for (size_t i=0; i<length.size()-1; i++)
                o << "[" << index[i] << "]";
            o << "[]" << std::endl;
            o << "{";
            for (int i=0; i<length[length.size()-1]; i++, index++) {
                if (i != 0)
                    o << ", ";
                (*this)[index]->printValue(o);
            }
            o <<  "}";
        }
    }
}


/** Reset container length in different dimensions */
void Container::resize(const VectorInteger& len) {

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
    VectorInteger numValsSource = VectorInteger(len.size(), 0), numValsTarget = VectorInteger(len.size(), 0);
    int numSourceVals = 1, numTargetVals = 1;
    for (int i=int(len.size())-1; i>=0; i--) {
        numSourceVals *= length[i];
        numTargetVals *= len[i];
        numValsSource[i] = numSourceVals;
        numValsTarget[i] = numTargetVals;
    }

    // Create new empty elements vector
    std::vector<RbObject*> tempElements = std::vector<RbObject*>(numValsTarget[0]);

    // Reorganize content
    int sourceIndex = 0, targetIndex = 0;
    do {
        for (int i=0; i<length[length.size()-1]; i++)
            tempElements[targetIndex++] = elements[sourceIndex++];

        for (int i=int(length.size())-1; i>=0; i--) {
            if (sourceIndex % numValsSource[i] == 0) {
                targetIndex += numValsTarget[i] - (targetIndex % numValsTarget[i]);
            }
        }
    } while (sourceIndex < (int)(elements.size()));

    // Copy the new container and length specification
    elements  = tempElements;
    length = len;
}


/** Set length in each dimension: reorganize the content of the container */
void Container::setLength(const VectorInteger& len) {

    if (len.size() != length.size())
        throw (RbException("Cannot change number of dimensions"));

    int numVals = 1;
    for (size_t i=0; i<len.size(); i++) {
        if (len[i] <= 0)
            throw (RbException("Nonpositive dimension length"));
        numVals *= len[i];
    }

    if (numVals != (int)(elements.size()))
        throw (RbException("Lengths do not match container size"));

    length = len;
}


/** Get complete info about object */
std::string Container::toString(void) const {

    std::ostringstream o;
    o << "RbObject container:" << std::endl;
    o << ".elementType = " << elementType << std::endl;
    o << ".dim         = " << getDim() << std::endl; 
    o << ".length      = " << length << std::endl;
    o << ".value       = " << (*this) << std::endl;

    return o.str();
}

