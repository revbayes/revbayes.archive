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
#include "DAGNode.h"
#include "RbNames.h"
#include "StringVector.h"


/** Construct from argument label and DAG node */
Argument::Argument(const std::string& argLabel, DAGNode* arg)
    : RbInternal() {

    label   = argLabel;
    dagNode = arg;
}


/** Copy constructor: Make independent copy of DAG node */
Argument::Argument(const Argument& x)
    : RbInternal() {

    label   = x.label;
    if (x.dagNode == NULL)
        dagNode = NULL;
    else
        dagNode = x.dagNode->clone();

}


/** Destructor: Delete wrapper */
Argument::~Argument() { 

	delete dagNode;

} 


/** Assignment operator: Make independent copy of DAG node */
Argument& Argument::operator=(const Argument& x) {

    if (this != &x) {
        delete dagNode;
        if (x.dagNode == NULL)
            dagNode = NULL;
        else
            dagNode = x.dagNode->clone();
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
    dagNode->printValue(o);

    return o.str();
}


