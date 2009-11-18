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


//#include "RbList.h"
#include "RbObject.h"

#include <string>
#include <ostream>
#include <iostream>

class RbList;

/** Initialize static class attribute */
//static RbList::rbClass(std::vector<string>(1,"object"));


/**
 * @brief Get class attribute
 *
 * Get the class attribute of the object. Implemented
 * here only as an indication of how it should be
 * implemented in derived classes.
 *
 */
const RbList* RbObject::getClass() const {

	return &rbClass;
}


/**
 * @brief Print object to ostream
 *
 * Print object to ostream. The current implementation
 * makes sure the name of the object is printed if
 * this function is not overridden, which works for now.
 *
 * @param o The ostream for printing
 *
 */
void RbObject::print(std::ostream& o) const {

	o << name;
}


/** Constructors, destructors and functions I do not use -- Fredrik **********/
RbObject::RbObject(const char* n)
    : name(n) {
}

RbObject::RbObject(const std::string& n)
    : name(n) {
}

/** Destructor is not needed -- Fredrik */
RbObject::~RbObject() {
}

std::string RbObject::getName() const {

	return name;
}

void RbObject::setName(std::string& n) {

	name = n;
}

void RbObject::setName(const char* n){

	name = n;
}

std::string RbObject::toString() const {

	return name;
}
