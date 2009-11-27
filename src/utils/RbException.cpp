/*
 *  RbException.cpp
 *  REvBayes
 *
 *  Created by John Huelsenbeck on 8/21/09.
 *  Copyright 2009 University of California, Berkeley. All rights reserved.
 *
 */

#include <string>
#include <iostream>

#include "RbException.h"
#include "StringVector.h"

const StringVector RbException::rbClass = StringVector("exception") + RbObject::rbClass;

	// constructors
RbException::RbException(void) : message("message"), RbObject(){
	// default constructor; does nothing
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor
 *
 * @param d          object to copy
 *
 */
RbException::RbException(const RbException& e)
    : RbObject(), message(e.message) {
}

/**
 * @brief destructor
 *
 * This is the standard destructor
 *
 *
 */
RbException::~RbException(void) {
	//delete &message;
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
RbObject* RbException::clone(void) const {

	RbObject *x = new RbException( *this );
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
void RbException::print(std::ostream &c) const {

	c << message << std::endl;
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
void RbException::dump(std::ostream& c){

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
void RbException::resurrect(const RbDumpState& x){

}

/**
 * @brief overloaded == operators
 *
 * This function compares with this object
 *
 * @param o           the object to compare to
 *
 */
bool RbException::equals(const RbObject* o) const {

	return false;
}


	// member functions
void RbException::setMessage(std::string m){

	message = m;
}

std::string RbException::getMessage(void){

	return message;
}

RbObject& RbException::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbException& x = dynamic_cast<const RbException&> (obj);

        RbException& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbException& x = dynamic_cast<const RbException&> (*(obj.convertTo("exception")));

            RbException& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to exception");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbException& RbException::operator=(const RbException& e) {

    message = e.message;
    return (*this);
}

void RbException::printValue(std::ostream& o) const {

	o << message << std::endl;
}

std::string RbException::toString(void) const {

	return message;
}
