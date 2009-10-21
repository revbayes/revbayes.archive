/*
 * RbTypeInfo.cpp
 *
 *  Created on: 29 sep 2009
 *      Author: Sebastian
 */

#include "RbTypeInfo.h"
#include "RbDataType.h"
#include <typeinfo>

/**
 * @brief Constructor for one dimensional objects
 *
 * This is the constructor for simple objects as integer and double.
 * Note, the default element type is the same as the object type and the default length is a one element array with value 1!
 *
 * @param ot    the type of the object
 */
RbTypeInfo::RbTypeInfo(std::type_info& ot) : objectType(ot),  elementType(ot){
    length = new int[1];
    length[0] = 1;
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
RbTypeInfo::RbTypeInfo(std::type_info& ot, std::type_info& et, int* l) : objectType(ot),  elementType(et){
    length = l;
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
 * @brief Getter for the type-id of the object.
 *
 * This function is the getter function for the object type.
 *
 * @returns     the object type id
 */
std::type_info& RbTypeInfo::getObjectType() const{
    return objectType;
}

/**
 * @brief Getter for the type-id of the elements of the object.
 *
 * This function is the getter function for the element type, if the object is from a container type (e.g. vector).
 *
 * @returns     the element type id
 */
std::type_info& RbTypeInfo::getElementType() const{
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
