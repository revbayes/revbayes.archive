/**
 * @file
 * This file contains the implementation of some functions in RbAbstractDataType,
 *  which is the REvBayes base class for the data types handled by the parser.
 *
 * @brief Implementation of RbAbstractDataType (some functions)
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date:$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id:$
 */

#include "RbAbstractDataType.h"
#include "RbDataType.h"
#include "../utils/RbException.h"
#include <iostream>
#include <string>


/** Define the abstract type */
//const string RbAbstractDataType::dataType = std::string("");


/** Constructor from type name (C style) */
RbAbstractDataType::RbAbstractDataType(const char* name)
    : RbDataType(name) {
}

/** Constructor from type name (C++ stye) */
RbAbstractDataType::RbAbstractDataType(const std::string& name)
    : RbDataType(name) {
}

/** Destructor frees names and dim attributes */
RbAbstractDataType::~RbAbstractDataType() {

//    delete labels;
//    delete &dim;
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbAbstractDataType::isConvertible(const RbDataType& dt) const {
	return (typeid(this) == typeid(dt));
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbAbstractDataType::isConvertible(const std::string& dt) const {
	return this->getType() == dt;
}

/**
 * @brief convertible to
 *
 * This function converts this data type into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          a new instance of the given data type with the same value as before, NULL if we cannot convert
 *
 */
RbDataType* RbAbstractDataType::convertTo(const std::string& dt) const {
	return NULL;
}

/**
 * @brief convertible to
 *
 * This function converts this data type into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          a new instance of the given data type with the same value as before, NULL if we cannot convert
 *
 */
RbDataType* RbAbstractDataType::convertTo(const RbDataType& dt) const {
	return NULL;
}

///** Set names attribute */
//void RbAbstractDataType::setNames(const RbString &newNames) {
//
//    if ( newNames.getSize() == getSize() ) {
//        delete names;
//        names = newNames.copy();
//    }
//}

///** Set dim attribute */
//void RbAbstractDataType::setDim(const int* &newDim) {
//
//    int dimSize = 1;
//    for (RbInt::const_iterator i=newDim.begin(); i!=newDim.end(); i++)
//        dimSize *= (*i);
//
//    if ( dimSize == getSize() ) {
//        delete dim;
//        dim = newDim.copy();
//    }
//}

/**
 * @brief addition operator
 *
 * This function adds two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to add
 * @returns          a new instance of this data type with the added values
 * @throws            RbException for not supported operations
 *
 */
RbDataType& RbAbstractDataType::operator+(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Addition of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief subtraction operator
 *
 * This function subtracts two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to subtract
 * @returns          a new instance of this data type with the subtracted values
 * @throws            RbException for not supported operations
 *
 */
RbDataType& RbAbstractDataType::operator-(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Subtraction of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief multiplication operator
 *
 * This function multiplies two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to multiply
 * @returns          a new instance of this data type with the multiplied values
 * @throws            RbException for not supported operations
 *
 */
RbDataType& RbAbstractDataType::operator*(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Multiplication of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief division operator
 *
 * This function divides two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to divide
 * @returns          a new instance of this data type with the divided values
 * @throws            RbException for not supported operations
 *
 */
RbDataType& RbAbstractDataType::operator/(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Division of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief exponentiation operator
 *
 * This function exponentiates two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to exponentiate
 * @returns          a new instance of this data type with the exponetiated values
 * @throws            RbException for not supported operations
 *
 */
RbDataType& RbAbstractDataType::operator^(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Exponentiation of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

