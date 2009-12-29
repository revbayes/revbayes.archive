/**
 * @file
 * This file contains the implementation of ArgumentRule, which is
 * used to describe rules for arguments passed to functions.
 *
 * @brief Implementation of ArgumentRule 
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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbUndefined.h"
#include "StringVector.h"

#include <sstream>

/** Construct rule without default value; use "" for no label */
ArgumentRule::ArgumentRule(const std::string& argName, const std::string& type, int dim) : RbInternal() {

    label        = argName;
    requiredType = type;
    numDim       = dim;
    defaultValue = new RbUndefined;
    minValue     = new RbUndefined;
    maxValue     = new RbUndefined;
}


/** Construct rule with default value; use "" for no label. We use info from default value to set type and dim */
ArgumentRule::ArgumentRule(const std::string& argName, RbObject* defVal) : RbInternal() {

    label        = argName;
    requiredType = defVal->getType();
    numDim       = defVal->getDim();
    defaultValue = defVal;
    minValue     = new RbUndefined;
    maxValue     = new RbUndefined;
}


/** Construct rule with required type, which could be a base class of the class of the default value */
ArgumentRule::ArgumentRule(const std::string& lbl, const std::string& t, RbObject* dv) : RbInternal() {

    label        = lbl;
    requiredType = t;
    numDim       = dv->getDim();
    defaultValue = dv;
    minValue     = new RbUndefined;
    maxValue     = new RbUndefined;
}


/** Construct rule with required type, default value, min and max */
ArgumentRule::ArgumentRule(const std::string& lbl, const std::string& t, RbObject* dv, RbObject* mnv, RbObject* mxv) : RbInternal() {

    label        = lbl;
    requiredType = t;
    numDim       = dv->getDim();
    defaultValue = dv;
    minValue     = mnv;
    maxValue     = mxv;
}


/** Copy constructor */
ArgumentRule::ArgumentRule(const ArgumentRule& a) : RbInternal() {

    label        = a.label;
    requiredType = a.requiredType;
    numDim       = a.numDim;
    defaultValue = a.defaultValue->clone();
    minValue     = a.minValue->clone();
    maxValue     = a.maxValue->clone();
}


/** Destructor deletes relevant values */
ArgumentRule::~ArgumentRule(void) {

	delete defaultValue;
	delete minValue;
	delete maxValue;
}


/** Assignment operator */
ArgumentRule& ArgumentRule::operator=(const ArgumentRule& a) {

    if (this != &a) {
        label        = a.label;
        requiredType = a.requiredType;
        numDim       = a.numDim;
        defaultValue = a.defaultValue->clone();
        minValue     = a.minValue->clone();
        maxValue     = a.maxValue->clone();
    }

    return (*this);
}


/** Get class vector describing type of object */
const StringVector& ArgumentRule::getClass(void) const { 

    static StringVector rbClass = StringVector(ArgumentRule_name) + RbInternal::getClass();
	return rbClass; 
}


/** Test if argument is valid */
bool ArgumentRule::isArgValid(const DAGNode* var) const {

    const RbObject* val = var->getValue();

    if (!val->isType(requiredType) && !val->isConvertibleTo(requiredType))
        return false;

    if (numDim != var->getDim())
        return false;

    // TODO: Test against min and max -- Why not in a derived class?
    return true;
}


/** Provide complete information about object */
std::string ArgumentRule::toString(void) const {

    std::ostringstream o;

    o << "ArgumentRule" << std::endl;
    o << "Label = " << label << std::endl;
    o << "Type = "  << requiredType << std::endl;
    o << "Dim  = "  << numDim << std::endl;
    o << "Value = ";
    defaultValue->printValue(o);
    o << std::endl;
    o << "Min = ";
    minValue->printValue(o);
    o << std::endl;
    o << "Max = ";
    maxValue->printValue(o);

    return o.str();
}

/** Print value for user (in descriptions of functions, for instance */
void ArgumentRule::printValue(std::ostream &o) const {

    o << requiredType;
    for (int i=0; i<numDim; i++)
        o << "[]";
    o << " \"" << label << "\"";
    if (!defaultValue->isType(RbNames::RbUndefined::name)) {
        o << " = ";
        defaultValue->printValue(o);
    }
}


