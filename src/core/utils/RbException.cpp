/**
 * @file
 * This file contains the implementation of RbException, which
 * is used to handle eceptions in RevBayes.
 *
 * @brief Implementation of RbException
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "RbException.h"
#include "RbUtil.h"

#include <string>
#include <iostream>


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


/** Get class name of object */
const std::string& RbException::getClassName(void) { 
    
    static std::string rbClassName = "Exception";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbException::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RbException::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
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


