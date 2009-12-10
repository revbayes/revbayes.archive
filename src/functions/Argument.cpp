/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include <sstream>

#include "Argument.h"
#include "RbNames.h"
#include "RbObjectWrapper.h"
#include "StringVector.h"


/** Construct from argument label and argument wrapper */
Argument::Argument(const std::string& argLabel, RbObjectWrapper* arg)
    : RbInternal() {

    label   = argLabel;
    wrapper = arg;
}


/** Copy constructor: Make independent copy of wrapper */
Argument::Argument(const Argument& x)
    : RbInternal() {

    label   = x.label;
    wrapper = x.wrapper->clone();
}


/** Destructor: Delete wrapper */
Argument::~Argument() { 

	delete wrapper; 
} 


/** Assignment operator: Make independent copy of wrapper */
Argument& Argument::operator=(const Argument& x) {

    if (this != &x) {
        delete wrapper;
        wrapper = x.wrapper->clone();
        label = x.label;
    }

    return (*this);
}


/** Get class vector describing type of object */
const StringVector& Argument::getClass(void) const { 

    static StringVector rbClass = StringVector(Argument_name) + RbInternal::getClass();
	return rbClass; 
}


/** Complete info about object */
std::string Argument::toString(void) const {

    std::ostringstream o;
    o << "Argument: label = \"" << label << "\", value = ";
    wrapper->printValue(o);

    return o.str();
}


