/**
 * @file
 * This file contains the implementation of Monitor, used to hold
 * information about the monitoring of a variable DAG node.
 *
 * @brief Implementation of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "Monitor.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <sstream>


/** Constructor */
Monitor::Monitor(VariableNode* node, int freq)
    : RbComplex(), theNode(node), samplingFrequency(freq) {
}


/** Clone the object */
Monitor* Monitor::clone(void) const {

    return new Monitor(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Monitor::convertTo(const std::string& type) const {

    throw (RbException("Conversion to " + type + " not supported"));
    return NULL;
}

/** Equals comparison */
bool Monitor::equals(const RbObject* obj) const {

    const Monitor* p = dynamic_cast<const Monitor*>(obj);
    if (p == NULL)
        return NULL;

    bool result = true;
    result = result && theNode == p->theNode;
    result = result && samplingFrequency == p->samplingFrequency;

    return result;
}


/** Get class vector describing type of object */
const VectorString& Monitor::getClass() const {

    static VectorString rbClass = VectorString(Monitor_name) + RbComplex::getClass();
    return rbClass;
}

/** Is the object convertible to an object of another class type? */
bool Monitor::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Monitor value unconditionally */
void Monitor::monitor(std::ofstream& o) const {

    theNode->printValue(o);
}


/** Monitor value at generation gen */
void Monitor::monitor(int gen, std::ofstream& o) const {

    if (gen % samplingFrequency == 0) {
        theNode->printValue(o);
    }
}


/** Print header for monitored values */
void Monitor::printHeader(std::ofstream& o) const {

    if (theNode->getName() != "")
        o << theNode->getName();
    else
        o << "Unnamed " << theNode->getType();
}


/** Print value for user */
void Monitor::printValue(std::ostream& o) const {

    o << "Monitor: interval = " << samplingFrequency << "; node = '" << theNode->getName() << "'";
}


/** Complete info about object */
std::string Monitor::toString(void) const {

    std::ostringstream o;
    o << "Monitor: interval = " << samplingFrequency << "; node = '" << theNode->getName() << "'";

    return o.str();
}


