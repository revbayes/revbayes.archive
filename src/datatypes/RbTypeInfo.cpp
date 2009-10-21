/*
 * RbTypeInfo.cpp
 *
 *  Created on: 29 sep 2009
 *      Author: Sebastian
 */

#include "RbTypeInfo.h"
#include "RbDataType.h"
#include "RbException.h"
#include <typeinfo>

/**
 * @brief Constructor for one dimensional objects
 *
 * This is the constructor for simple objects as integer and double.
 * Note, the default element type is the same as the object type and the default length is a one element array with value 1!
 *
 * @param ot    the type of the object
 */
RbTypeInfo::RbTypeInfo(const std::type_info& ot) : objectType(ot),  elementType(ot){
    length = new int[1];
    length[0] = 1;
    dimensions = 0;
}

/**
 * @brief Constructor for multidimensional objects
 *
 * This is the constructor for multidimensional objects as vectors.
 * The object type should be a vector or similar and the element type specify the contained element.
 * The length is an array specifying the length of each dimension, e.g. a one element array of size n=4
 * if this a vector with for elements and no nested elements. A 4x4 matrix would have the length[0]=4 and length[1]=4.
 *
 * @param ot    the type of the object
 */
RbTypeInfo::RbTypeInfo(const std::type_info& ot, const std::type_info& et, int* l, int d) : objectType(ot),  elementType(et){
    length = l;
    dimensions = d;
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor .
 *
 * @param ot    the object to copy
 */
RbTypeInfo::RbTypeInfo(const RbTypeInfo& ti) : objectType(ti.getObjectType()),  elementType(ti.getElementType()){
    length = ti.getLength();
    dimensions = ti.getDimensions();
}

/* @brief Destructor
 *
 * Default destructor
 *
 */
RbTypeInfo::~RbTypeInfo(){
    delete length;
}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* RbTypeInfo::clone(void) const {

    RbObject *x = new RbTypeInfo( *this );
    return x;
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void RbTypeInfo::print(std::ostream &c) const {

    std::string message = "print function of RbTypeInfo not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbTypeInfo::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbTypeInfo not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void RbTypeInfo::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbTypeInfo not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbTypeInfo::operator==(const RbObject& o) const {

    if (typeid(RbTypeInfo) == typeid(o)){
        // we are from the same type, which is perfect :)
        RbTypeInfo& tmp = ((RbTypeInfo&)o);
        return (*this) == tmp;
    }

    return false;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbTypeInfo::operator==(const RbTypeInfo& o) const {
    bool equal = true;

    if (objectType == o.getObjectType() && elementType == o.getElementType()){
        // object and element type are the same
        // now just need to check the dimensions
        if (dimensions == o.getDimensions()){
            for (int i=0; i<dimensions; i++){
                if (length[i] != o.getLength()[i]){
                    equal = false;
                }
            }
        }
        else {
            equal = false;
        }
    }
    else {
        equal = false;
    }

    return equal;
}

/**
 * @brief Getter for the type-id of the object.
 *
 * This function is the getter function for the object type.
 *
 * @returns     the object type id
 */
const std::type_info& RbTypeInfo::getObjectType() const{
    return objectType;
}

/**
 * @brief Getter for the type-id of the elements of the object.
 *
 * This function is the getter function for the element type, if the object is from a container type (e.g. vector).
 *
 * @returns     the element type id
 */
const std::type_info& RbTypeInfo::getElementType() const{
    return elementType;
}

/**
 * @brief Getter for the length of the object.
 *
 * This function is the getter function for the length of the object.
 *
 * @returns     returns 1 for simple objects as int or double, -1 for a flexible length and n for a fixed number of elements. The return type is an integer array with a length for each dimension.
 */
int* RbTypeInfo::getLength() const{
    return length;
}

/**
 * @brief Getter for the length of the object.
 *
 * This function is the getter function for the length of the object.
 *
 * @returns     returns 1 for simple objects as int or double, -1 for a flexible length and n for a fixed number of elements. The return type is an integer array with a length for each dimension.
 */
int RbTypeInfo::getDimensions() const {
    return dimensions;
}
