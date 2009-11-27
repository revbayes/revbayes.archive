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

#include <math.h>

#include "RbBool.h"
#include "RbInt.h"
#include "RbDouble.h"
#include "RbPrimitive.h"
#include "RbDumpState.h"
#include "RbException.h"
#include "StringVector.h"
#include <iostream>

const StringVector RbInt::rbClass = StringVector("int") + RbPrimitive::rbClass;

/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value.
 *
 * @param v          initial value of the data type
 *
 */
RbInt::RbInt(const int v) : RbPrimitive(), value(v) {
    
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param d          object to copy
 *
 */
RbInt::RbInt(const RbInt& d) :  RbPrimitive(), value(d.value) {

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
RbObject* RbInt::clone(void) const {

    RbObject *x = new RbInt(*this);
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
void RbInt::dump(std::ostream& c) {
    //TODO implement

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
void RbInt::resurrect(const RbDumpState& x) {
    //TODO implement

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
bool RbInt::isConvertibleTo(const std::string& type) const {

    if (type == "bool") {
        return true;
    } else if (type == "double") {
        return true;
    } else if (type == "char") {
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
RbObject* RbInt::convertTo(const std::string& type) const {

	RbObject* retObj = NULL;
    if (type == "bool") 
    	{
        RbBool* temp = new RbBool(value == 0);
        retObj = (RbObject*)temp;
    	} 
    else if (type == "double") 
    	{
        RbDouble* temp = new RbDouble(value);
        retObj = (RbObject*)temp;
   	 	}
    return retObj;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbInt::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const RbInt* x = dynamic_cast<const RbInt*>(obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to an int
    x = dynamic_cast<const RbInt*>(obj->convertTo("int"));
    if (x == NULL)
        return false;

    bool result = (value == x->value);
    delete x;
    return result;

}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbInt::equals(const RbInt* o) const {
    return value == o->getValue();
}

/**
 * @brief overloaded addition operators
 *
 * This function adds o to this object
 *
 * @param o           the object to add to this instance
 *
 */
RbObject* RbInt::add(const RbObject* o) const {

    if (o->getClass()[0] == "int") {
        // we are from the same type, which is perfect :)
        return new RbInt(value + ((RbInt*)(o))->getValue());
  	  } 
    else if (o->getClass()[0] == "double") 
    	{
        return new RbDouble(value + ((RbDouble*)(o))->getValue());
    	}
    else {
        std::string message = "Addition of " + getClass()[0] + " and " + o->getClass()[0]
                + " is not supported.";
        RbException e;
        e.setMessage(message);
        throw e;
    }

    // fake return
    return new RbInt(0);
}

/**
 * @brief overloaded subtraction operators
 *
 * This function subtracts o to this object
 *
 * @param o           the object to subtract to this instance
 *
 */
RbObject* RbInt::subtract(const RbObject* o) const {

    if (o->getClass()[0] == "int") {
        // we are from the same type, which is perfect :)
        return new RbInt(value - ((RbInt*)(o))->getValue());
    } 
    else if (o->getClass()[0] == "double") 
    	{
        return new RbDouble(value - ((RbDouble*)(o))->getValue());
    	}
    else {
        std::string message = "Subtraction of " + getClass()[0] + " and " + o->getClass()[0]
                + " is not supported.";
        RbException e;
        e.setMessage(message);
        throw e;
    }

    // fake return
    return new RbInt(0);
}

/**
 * @brief overloaded multiplication operators
 *
 * This function multiplies o to this object
 *
 * @param o           the object to multiply to this instance
 *
 */
RbObject* RbInt::multiply(const RbObject* o) const {

    if (o->getClass()[0] == "int") {
        // we are from the same type, which is perfect :)
        return new RbInt(value * ((RbInt*)(o))->getValue());
    } 
    else if (o->getClass()[0] == "double") 
    	{
        return new RbDouble(value * ((RbDouble*)(o))->getValue());
    	}
    else {
        std::string message = "Multiplication of " + getClass()[0] + " and " + o->getClass()[0]
                + " is not supported.";
        RbException e;
        e.setMessage(message);
        throw e;
    }

    // fake return
    return new RbInt(0);
}

/**
 * @brief overloaded division operators
 *
 * This function divides o to this object
 *
 * @param o           the object to divide to this instance
 *
 */
RbObject* RbInt::divide(const RbObject* o) const {

	if (o->getClass()[0] == "int") 
		{
		// we are from the same type, which is perfect :)
		if ( ((RbInt*)(o))->getValue() == 0 )
			{
			throw RbException("Divide by zero");
			}
		return new RbInt(value / ((RbInt*)(o))->getValue());
		} 
	else if (o->getClass()[0] == "double") 
		{
		if ( ((RbDouble*)(o))->getValue() == 0.0 )
			{
			throw RbException("Divide by zero");
			}
		return new RbDouble(value / ((RbDouble*)(o))->getValue());
		}
	else 
		{
		std::string message = "Division of " + getClass()[0] + " and " + o->getClass()[0] + " is not supported.";
		RbException e;
		e.setMessage(message);
		throw e;
		}
	// fake return
	return new RbInt(0);
}

bool RbInt::lessThan(const RbObject* o) const {

	if (o->getClass()[0] == "int") 
		{
		// we are from the same type, which is perfect :)
		if ( ((RbInt*)(o))->getValue() < value )
			return true;
		} 
	else if (o->getClass()[0] == "double") 
		{
		if ( ((RbDouble*)(o))->getValue() < (double)value )
			return true;
		}
	return false;
}

/**
 * @brief overloaded power operators
 *
 * This function raises this object to the power of o
 *
 * @param o           the object to add to this instance
 *
 */
RbObject* RbInt::raiseTo(const RbObject* o) const {

    if (o->getClass()[0] == "int") 
    	{
        // we are from the same type, which is perfect :)
        return new RbDouble(pow( (double)value, (double)((RbInt*)(o))->getValue() ));
    	} 
    else if (o->getClass()[0] == "double") 
    	{
        return new RbDouble(pow( (double)value, (double)((RbDouble*)(o))->getValue() ));
    	}
    else 
    	{
        std::string message = "Raise to of " + getClass()[0] + " and " + o->getClass()[0]
                + " is not supported.";
        RbException e;
        e.setMessage(message);
        throw e;
    }

    // fake return
    return new RbInt(0);
}

/**
 * @brief getter for value
 *
 * This function get the value of this object
 *
 * @return           the value
 *
 */
RbInt::operator int() const {

    return value;
}

RbObject& RbInt::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbInt& x = dynamic_cast<const RbInt&> (obj);

        RbInt& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbInt& x = dynamic_cast<const RbInt&> (*(obj.convertTo("int")));

            RbInt& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to int");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbInt& RbInt::operator=(const RbInt& o) {

    value = o.value;
    return (*this);
}

/**
 * @brief Prdouble value
 *
 * Prints value for user.
 *
 * @param o     The stream for printing
 *
 */
void RbInt::printValue(std::ostream &o) const {

    o << value << std::endl;
}

std::string RbInt::toString(void) const {

	char temp[50];
	sprintf(temp, "%d", value);
	std::string tempStr = temp;
	return temp;
}
