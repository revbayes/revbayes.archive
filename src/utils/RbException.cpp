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
#include "../main/RbObject.h"


	// constructors
RbException::RbException(void) : message("message"), RbObject("RbException"){
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
    : RbObject(e.name), message(e.message) {
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
RbObject* RbException::clone(void) {

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
bool RbException::operator==(RbObject& o) const {

	if (typeid(RbException) == typeid(o)){
		// we are from the same type, which is perfect :)
		RbException& tmp = ((RbException&)o);
		return message == tmp.getMessage();
	}

	return false;
}


	// member functions
void RbException::setMessage(std::string m){
	message = m;
}

std::string RbException::getMessage(void){
	return message;
}

