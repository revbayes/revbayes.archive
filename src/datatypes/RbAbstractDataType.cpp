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


/** Constructor from name (C style) */
RbAbstractDataType::RbAbstractDataType(const char* name)
    : RbDataType(name) {
}

/** Constructor from name (C++ stye) */
RbAbstractDataType::RbAbstractDataType(const std::string& name)
    : RbDataType(name) {
}

/** Destructor frees names and dim attributes */
RbAbstractDataType::~RbAbstractDataType() {

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
 * @param dt         the data type we want to convert to (RbTypeInfo)
 * @returns          true, if it can be converted
 *
 */
bool RbAbstractDataType::isConvertible(const RbTypeInfo& t) const {
	return typeid(this) == t.getTypeid();
}

/**
 * @brief convert to
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
 * @brief convert to
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
 * @brief Addition operator
 *
 * This function adds two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to add
 * @returns          a new instance of this data type with the added values
 * @throws           RbException for not supported operations
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
 * @brief Subtraction operator
 *
 * This function subtracts two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to subtract
 * @returns          a new instance of this data type with the subtracted values
 * @throws           RbException for not supported operations
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
 * @brief Multiplication operator
 *
 * This function multiplies two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to multiply
 * @returns          a new instance of this data type with the multiplied values
 * @throws           RbException for not supported operations
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
 * @brief Division operator
 *
 * This function divides two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to divide
 * @returns          a new instance of this data type with the divided values
 * @throws           RbException for not supported operations
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
 * @throws           RbException for not supported operations
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

/**
 * @brief Addition operator type info
 *
 * This function returns info about the return type of a binary addition operation. It is the abstract function and therefore only throws the exception
 * that this function is not supported. Derived classes which support this operation have to overwrite it.
 *
 * @param t          info about the data type we want to add
 * @returns          info about the resulting data type
 * @throws           RbException for not supported operations
 *
 */
RbTypeInfo& RbAbstractDataType::operator+(RbTypeInfo& t) const {
	std::string myName = typeid(*this).name();
	std::string oName = t.getTypeid().name();
	std::string message = "Addition of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Subtraction operator type info
 *
 * This function returns info about the return type of a binary subtraction operation. It is the abstract function and therefore only throws the exception
 * that this function is not supported. Derived classes which support this operation have to overwrite it.
 *
 * @param t          info about the data type we want to subtract
 * @returns          info about the resulting data type
 * @throws           RbException for not supported operations
 *
 */
RbTypeInfo& RbAbstractDataType::operator-(RbTypeInfo& t) const {
	std::string myName = typeid(*this).name();
	std::string oName = t.getTypeid().name();
	std::string message = "Subtraction of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Multiplication operator type info
 *
 * This function returns info about the return type of a binary multiplication operation. It is the abstract function and therefore only throws the exception
 * that this function is not supported. Derived classes which support this operation have to overwrite it.
 *
 * @param t          info about the data type we want to multiply
 * @returns          info about the resulting data type
 * @throws           RbException for not supported operations
 *
 */
RbTypeInfo& RbAbstractDataType::operator*(RbTypeInfo& t) const {
	std::string myName = typeid(*this).name();
	std::string oName = t.getTypeid().name();
	std::string message = "Multiplication of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Division operator type info
 *
 * This function returns info about the return type of a binary division operation. It is the abstract function and therefore only throws the exception
 * that this function is not supported. Derived classes which support this operation have to overwrite it.
 *
 * @param t          info about the data type we want to divide
 * @returns          info about the resulting data type
 * @throws           RbException for not supported operations
 *
 */
RbTypeInfo& RbAbstractDataType::operator/(RbTypeInfo& t) const {
	std::string myName = typeid(*this).name();
	std::string oName = t.getTypeid().name();
	std::string message = "Division of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Power operator type info
 *
 * This function returns info about the return type of a binary power operation. It is the abstract function and therefore only throws the exception
 * that this function is not supported. Derived classes which support this operation have to overwrite it.
 *
 * @param t          info about the data type of the power
 * @returns          info about the resulting data type
 * @throws           RbException for not supported operations
 *
 */
RbTypeInfo& RbAbstractDataType::operator^(RbTypeInfo& t) const {
	std::string myName = typeid(*this).name();
	std::string oName = t.getTypeid().name();
	std::string message = "Raising " + myName + " to the power of " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Less than operator
 *
 * This function compares two data types. It is the abstract function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @param dt         the data type we want to compare
 * @returns          true if smaller, false otherwise
 * @throws           RbException for not supported operations
 *
 */
bool RbAbstractDataType::operator<(RbDataType& o) const {
	std::string myName = typeid(*this).name();
	std::string oName = typeid(o).name();
	std::string message = "Comparison of " + myName + " and " + oName + " is not supported.";
	RbException e;
	e.setMessage(message);
	throw e;
}

/**
 * @brief Is comparable
 *
 * This function returns true if the data type is comparable, that is, if you can order two instances relative to each other. It is the abstract
 * function and therefore only throws the exception that this function is not supported.
 * Derived classes which support this operation have to overwrite it.
 *
 * @returns          true if comparable (false in the abstract base class)
 *
 */
bool RbAbstractDataType::isComparable() const {
    return false;
}
