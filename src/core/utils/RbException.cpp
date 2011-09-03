/**
 * @file
 * This file contains the implementation of RbException, which
 * is used to handle eceptions in RevBayes.
 *
 * @brief Implementation of RbException
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-21 18:33:30 +0100 (Mån, 21 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RbException.cpp 202 2009-12-21 17:33:30Z Hoehna $
 */

#include "RbException.h"
#include "RbUtil.h"
#include "VectorString.h"

#include <string>
#include <iostream>


// Definition of the static type spec member
const TypeSpec RbException::typeSpec(RbException_name);

/** Static string with names of exception types for printing */
std::string RbException::exceptionName[] = { "DEFAULT", "QUIT" };


/** Default constructor */
RbException::RbException(void) : RbInternal(), exceptionType(DEFAULT), message() {
}


/** Message constructor */
RbException::RbException(const std::string& msg) : RbInternal(), exceptionType(DEFAULT), message(msg) {
}


/** Message constructor from stringstream */
RbException::RbException(const std::ostringstream& msg) : RbInternal(), exceptionType(DEFAULT), message(msg.str()) {
}


/** General constructor */
RbException::RbException(exceptionT type, const std::string& msg) : RbInternal(), exceptionType(type), message(msg) {
}


/** Clone function */
RbException* RbException::clone(void) const {

	return new RbException( *this );
}


/** Get class vector describing type of object */
const VectorString& RbException::getClass(void) const { 

    static VectorString rbClass = VectorString(RbException_name) + RbInternal::getClass();
	return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& RbException::getTypeSpec(void) const {
    return typeSpec;
}


/** Print value for user */
void RbException::printValue(std::ostream& o) const {

	o << message;
}

std::string RbException::getMessage(void) const {

    return message;
}

void RbException::setMessage(std::string msg) {

    message = msg;
}

std::string RbException::richInfo(void) const {

	std::ostringstream o;

    o << "RbExeption: type = " << exceptionName[exceptionType] << ", message = ";
    printValue(o);

    return o.str();
}


