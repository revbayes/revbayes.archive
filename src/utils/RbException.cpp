/**
 * @file
 * This file contains the implementation of RbException, which
 * is used to handle eceptions in RevBayes.
 *
 * @brief Implementation of RbException
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"

#include <string>
#include <iostream>


/** Static string with names of exception types for printing */
std::string RbException::exceptionName[] = { "DEFAULT", "QUIT" };


/** Default constructor */
RbException::RbException(void)
    : RbInternal(), exceptionType(DEFAULT), message() {
}


/** Message constructor */
RbException::RbException(const std::string& msg)
    : RbInternal(), exceptionType(DEFAULT), message(msg) {
}


/** Message constructor from stringstream */
RbException::RbException(const std::ostringstream& msg)
    : RbInternal(), exceptionType(DEFAULT), message(msg.str()) {
}


/** General constructor */
RbException::RbException(exceptionT type, const std::string& msg)
    : RbInternal(), exceptionType(type), message(msg) {
}


/** Clone function */
RbException* RbException::clone(void) const {

	return new RbException( *this );
}


/** Get class vector describing type of object */
const StringVector& RbException::getClass(void) const { 

    static StringVector rbClass = StringVector(RbException_name) + RbInternal::getClass();
	return rbClass;
}


/** Print value for user */
void RbException::printValue(std::ostream& o) const {

	o << message;
}

std::string RbException::toString(void) const {

	std::ostringstream o;

    o << "RbExeption: type = " << exceptionName[exceptionType] << ", message = ";
    printValue(o);

    return o.str();
}


