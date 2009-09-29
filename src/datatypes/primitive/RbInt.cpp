/**
 * @file
 * This file contains the class implementing a RbDataType for integers.
 * This is the class for the REvBayes software handling simple integer numbers.
 * All functionality of normal integer numbers is provided in this class,
 * but it also is used as a data type for this program. Therefore, it can be used as return values
 * for functions, distributions, etc and it can be used as parameters as well.
 *
 * @brief Implementation of the class RbInt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-07, version 1.0
 * @interface RbDataType
 * @extends RbAbstractDataType
 *
 * $Id$
 */

#include "RbInt.h"
#include "RbDouble.h"
#include "RbDataType.h"
#include "RbDumpState.h"
#include "RbException.h"
#include <iostream>

/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value.
 *
 * @param v          initial value of the data type
 *
 */
RbInt::RbInt(const int v)
    : RbAbstractDataType("RbInt"), value(v) {
}

/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value and name/id of this instance.
 *
 * @param v          initial value of the data type
 * @param name       name of this instance
 *
 */
RbInt::RbInt(const int v, std::string& name)
    : RbAbstractDataType(name), value(v) {
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param d          object to copy
 *
 */
RbInt::RbInt(const RbInt& d)
    : RbAbstractDataType(d.name), value(d.value) {
}

/**
 * @brief destructor
 *
 * This is the standard destructor
 *
 *
 */
RbInt::~RbInt(void) {
	// forced a crash in testInt
	//delete &value;
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
RbObject* RbInt::clone(void) {

	RbObject *x = new RbInt( *this );
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
void RbInt::print(std::ostream &c) const {

	c << value << std::endl;
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
void RbInt::dump(std::ostream& c){

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
void RbInt::resurrect(const RbDumpState& x){

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
bool RbInt::isConvertible(const RbDataType& dt) const {
	if (typeid(RbInt) == typeid(dt)){
		return true;
	}
	else if (typeid(RbDouble) == typeid(dt)){
		return true;
	}

	return false;
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
bool RbInt::isConvertible(const RbTypeInfo& dt) const {
	if (typeid(*this) == dt.getTypeid()){
		return true;
	}
	else if (typeid(RbDouble) == dt.getTypeid()){
		return true;
	}

	return false;
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
RbDataType* RbInt::convertTo(const RbTypeInfo& dt) const {
	if (typeid(*this) == dt.getTypeid()){
		return new RbInt(value);
	}
	else if (typeid(RbDouble) == dt.getTypeid()){
		return new RbDouble(value);
	}

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
RbDataType* RbInt::convertTo(const RbDataType& dt) const {
	if (typeid(RbInt) == typeid(dt)){
		return new RbInt(value);
	}
	else if (typeid(RbDouble) == typeid(dt)){
		return new RbDouble(value);
	}

	return NULL;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbInt::operator==(RbObject& o) const {

	if (typeid(RbInt) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbInt& tmp = ((RbInt&)o);
		return value == tmp.getValue();
	}
	else {
		RbDataType& dt = dynamic_cast<RbDataType&>(o);
		if ((&dt) != 0) {
			if (isConvertible(dt)){
				//can I convert myself into the type of o
				RbDataType* newType = convertTo(dt);
				return ((*newType) == dt);
			}
			else if (dt.isConvertible(*this)){
				//try to convert o into my data type
				RbDataType* newType = dt.convertTo(*this);
				RbInt& tmp = ((RbInt&)*newType);
				return value == tmp.getValue();
			}
		}
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
bool RbInt::operator==(RbInt& o) const {
	return value == o.getValue();
}

/**
 * @brief overloaded addition operators
 *
 * This function adds o to this object
 *
 * @param o           the object to add to this instance
 *
 */
RbDataType& RbInt::operator+(RbDataType& o) const {

	if (typeid(RbInt) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbInt& tmp = ((RbInt&)o);
		return *(new RbInt (value + tmp.getValue()));
	}
	else {
		std::string myName = typeid(*this).name();
		std::string oName = typeid(o).name();
		std::string message = "Addition of " + myName + " and " + oName + " is not supported.";
		RbException e;
		e.setMessage(message);
		throw e;
	}

	// fake return
	return *(new RbInt(0));
}

/**
 * @brief overloaded addition operators
 *
 * This function adds o to this object
 *
 * @param o           the object to add to this instance
 *
 */
RbInt& RbInt::operator+(RbInt& o) const {
	return *(new RbInt(value + o.getValue()));
}

/**
 * @brief setter for value
 *
 * This function set the value to d
 *
 * @param d           the new value
 *
 */
void RbInt::setValue(int v) {
	value = v;
}

/**
 * @brief getter for value
 *
 * This function get the value of this object
 *
 * @return           the value
 *
 */
int RbInt::getValue(void) const {
	return value;
}
