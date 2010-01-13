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
#include "RbNames.h"
#include "StringVector.h"
#include "VariableNode.h"

#include <sstream>


/** Constructor */
Monitor::Monitor(std::ostream& o, VariableNode* node, int freq)
    : RbComplex(), outStrm(o), theNode(node), sampleFrequency(freq) {
}


/** Clone the object */
RbObject* Monitor::clone(void) const {

    return (RbObject*)(new Monitor(*this));
}


/** Equals comparison */
bool Monitor::equals(const RbObject* obj) const {

    const Monitor* p = dynamic_cast<const Monitor*>(obj);
    if (p == NULL)
        return NULL;

    bool result = true;
    result = result && outStrm == p->outStrm;
    result = result && theNode == p->theNode;
    result = result && sampleFrequency == p->sampleFrequency;

    return result;
}


/** Get class vector describing type of object */
const StringVector& Monitor::getClass() const {

    static StringVector rbClass = StringVector(Monitor_name) + RbComplex::getClass();
    return rbClass;
}


/** Monitor value at generation gen */
void Monitor::monitor(int gen) const {

    if (gen % sampleFrequency == 0) {
        theNode->printValue(outStrm);
    }
}


/** Print header for monitored values */
void Monitor::printHeader(void) const {

    // TODO: Figure out better header for elements of containers
    if (theNode->getName() != "")
        outStrm << theNode->getName();
    else
        outStrm << "Unnamed node";
}


/** Print value for user */
void Monitor::printValue(std::ostream& o) const {

    o << "Monitor: interval = " << sampleFrequency << "; node = '" << theNode->getName() << "'";
}


/** Complete info about object */
std::string Monitor::toString(void) const {

    std::ostringstream o;
    o << "Monitor: interval = " << sampleFrequency << "; node = '" << theNode->getName() << "'";

    return o.str();
}


