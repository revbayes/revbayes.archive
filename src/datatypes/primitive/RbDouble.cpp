/**
 * @file
 * This file contains the class implementing a RbDataType for double.
 * This is the class for the REvBayes software handling simple floating point numbers.
 * All functionality of normal floating point numbers is provided in this class,
 * but it also is used as a data type for this program. Therefore, it can be used as return values
 * for functions, distributions, etc and it can be used as parameters as well.
 *
 * @brief Implementation of the class RbDouble
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

#include "RbDouble.h"
#include "../RbDataType.h"
#include "../../utils/RbDumpState.h"
#include <iostream>

/**
 * @brief the name of this data type
 *
 * The name of this data type which is used for association or referencing.
 * Data types in REvBayes can be checked from which type they are by asking for its dataType.
 *
 */
const std::string RbDouble::dataType="RbDouble";

/**
 * @brief constructor
 *
 * This is the constructor creating a new RbDouble instance with the given initial value.
 *
 * @param v          initial value of the data type
 *
 */
RbDouble::RbDouble(const double v)
    : RbAbstractDataType("RbDouble"), value(v) {
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
RbDouble::RbDouble(const double v, std::string& name)
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
RbDouble::RbDouble(const RbDouble& d)
    : RbAbstractDataType(d.name), value(d.value) {
}

/**
 * @brief destructor
 *
 * This is the standard deconstructor
 *
 *
 */
RbDouble::~RbDouble(void) {
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
RbObject* RbDouble::clone(void) {

	RbObject *x = new RbDouble( *this );
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
void RbDouble::print(ostream &c) const {

	c << value << endl;
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
void RbDouble::dump(std::ostream& c){

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
void RbDouble::resurrect(const RbDumpState& x){

}

/**
 * @brief get name for this data type
 *
 * This function get name for this data type.
 * It is basically only a convinience function to access the static member dataType from a base class reference.
 *
 * @see RbDataType.getType()
 *
 */
const std::string&  RbDouble::getType(void) const{
	return dataType;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbDouble::operator ==(RbObject& o) const {

	if (typeid(RbDouble) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbDouble& tmp = ((RbDouble&)o);
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
			else if (dt.isConvertible(dataType)){
				//try to convert o into my data type
				RbDataType* newType = dt.convertTo(dataType);
				RbDouble& tmp = ((RbDouble&)*newType);
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
void RbDouble::setValue(double d) {
	value = d;
}

/**
 * @brief getter for value
 *
 * This function get the value of this object
 *
 * @return           the value
 *
 */
double RbDouble::getValue(void) const {
	return value;
}
