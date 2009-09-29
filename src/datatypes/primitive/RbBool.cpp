/**
 * @file
 * This file contains the class implementing a RbDataType for logic values.
 * This is the class for the REvBayes software handling simple logic values.
 * All functionality of normal logical values is provided in this class,
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

#include "RbBool.h"
#include "../RbDataType.h"
#include "../../utils/RbDumpState.h"
#include <iostream>

/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value.
 *
 * @param v          initial value of the data type
 *
 */
RbBool::RbBool(const bool v)
    : RbAbstractDataType("RbBool"), value(v) {
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
RbBool::RbBool(const bool v, std::string& name)
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
RbBool::RbBool(const RbBool& d)
    : RbAbstractDataType(d.name), value(d.value) {
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
RbObject* RbBool::clone(void) {

	RbObject *x = new RbBool( *this );
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
void RbBool::print(std::ostream &c) const {

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
bool RbBool::operator ==(RbObject& o) const {

	if (typeid(RbBool) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbBool& tmp = ((RbBool&)o);
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
				RbBool& tmp = ((RbBool&)*newType);
				return value == tmp.getValue();
			}
		}
	}

	return false;
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
