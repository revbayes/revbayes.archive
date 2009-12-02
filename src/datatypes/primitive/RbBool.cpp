/**
 * @file
 * This file contains the implementation of RbBool, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of the class RbInt
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include <iostream>

#include "RbBool.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbPrimitive.h"
#include "StringVector.h"

const StringVector RbBool::rbClass = StringVector("bool") + RbPrimitive::rbClass;


/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value.
 *
 * @param v          initial value of the data type
 *
 */
RbBool::RbBool(const bool v)
    : RbPrimitive(), value(v) {
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param d          object to copy
 *
 */
RbBool::RbBool(const RbBool& d)
    : RbPrimitive(), value(d.value) {
}

/**
 * @brief destructor
 *
 * This is the standard deconstructor
 *
 *
 */
RbBool::~RbBool(void) {
	delete &value;
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
RbObject* RbBool::clone(void) const {

	RbObject *x = new RbBool( *this );
	return x;
}

RbObject* RbBool::convertTo(const std::string& type) const {

	RbObject* retObj = NULL;
    if (type == "int") 
    	{
        RbInt* temp = new RbInt(value);
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
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbBool::dump(std::ostream& c){

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
void RbBool::resurrect(const RbDumpState& x){

}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbBool::equals(const RbObject* o) const {

	return false; // FOR NOW
}

/**
 * @brief setter for value
 *
 * This function set the value to d
 *
 * @param d           the new value
 *
 */
void RbBool::setValue(bool v) {
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
bool RbBool::getValue(void) const {

	return value;
}

RbObject& RbBool::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbBool& x = dynamic_cast<const RbBool&> (obj);

        RbBool& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbBool& x = dynamic_cast<const RbBool&> (*(obj.convertTo("bool")));

            RbBool& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to bool");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbBool& RbBool::operator=(const RbBool& obj) {

    value = obj.value;
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
void RbBool::printValue(std::ostream &o) const {

    o << value << std::endl;
}

/**
 * @brief complete info
 *
 * return complete object info.
 *
 * @return o     The string describing this object
 *
 */
std::string RbBool::toString(void) const {

	char temp[30];
	sprintf(temp, "%d", value);
	std::string tempStr = temp;
    return "Value = " + tempStr;
}

bool RbBool::lessThan(const RbObject* o) const {

	
}

RbObject* RbBool::add(const RbObject* o) const {

}

RbObject* RbBool::subtract(const RbObject* o) const {

}

RbObject* RbBool::multiply(const RbObject* o) const {

}

RbObject* RbBool::divide(const RbObject* o) const {

}

RbObject* RbBool::raiseTo(const RbObject* o) const {

}
