/**
 * @file
 * This file contains the partial implementation of RbObject,
 * which is the RevBayes abstract base class for all objects.
 *
 * @brief Partial implementation of RbObject
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */


//#include "StringVector.h"
#include "RbObject.h"

#include <string>
#include <ostream>
#include <iostream>


/** Initialize static class attribute */
//static StringVector RbObject::rbClass = StringVector(std::vector<std::string>(1,"object"));


/**
 * @brief Get class attribute
 *
 * Get the class attribute of the object.
 *
 */
const StringVector& RbObject::getClass() const {

	return rbClass;
}


/**
 * @brief Print object info
 *
 * Print object info to ostream. The implementation here
 * just prints the class attribute.
 *
 * @param o The ostream for printing
 *
 */
void RbObject::print(std::ostream& o) const {

	o << "Object of class: " << /* getClass() << */ std::endl;
}

