/**
 * @file
 * This file contains the partial implementation of RbObject,
 * which is the REvBayes abstract base class for all objects.
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


#include "RbObject.h"
#include <string>
#include <ostream>


const std::string& RbObject::getName() const {

	return name;
}

void RbObject::print(std::ostream &c) const {

	c<<toString();
}

void RbObject::setName(std::string& n) {

	name = n;
}

std::string RbObject::toString() const {

	return name;
}
