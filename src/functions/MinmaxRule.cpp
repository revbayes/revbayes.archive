/**
 * @file
 * This file contains the implementation of MinmaxRule, which is
 * used to describe argument rules with min and max values.
 *
 * @brief Implementation of MinmaxRule 
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

#include "DAGNode.h"
#include "ConstantNode.h"
#include "MinmaxRule.h"
#include "Real.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule without default value; use "" for no label. */
MinmaxRule::MinmaxRule(const std::string& argName, const std::string& valType, RbObject* min, RbObject* max)
    : ArgumentRule(argName, valType, 0) {

    /* TODO: Does not work in gcc
    if (!Workspace::globalWorkspace().isXOfTypeY(valType, Real_name) &&
        !Workspace::globalWorkspace().isXOfTypeY(valType, RbInt_name))
        throw RbException("Wrong type for argument rule with min and max");
    */

    if (min != NULL && !min->isType(valType))
        throw RbException("Wrong type for min value");
    if (max != NULL && !max->isType(valType))
        throw RbException("Wrong type for max value");

    minVal = min;
    maxVal = max;
}


/** Construct rule with default value. */
MinmaxRule::MinmaxRule(const std::string& argName, RbObject* defVal, RbObject* min, RbObject* max)
    : ArgumentRule(argName, defVal) {

    if (!defVal->isType(Real_name) &&
        !defVal->isType(Integer_name))
        throw RbException("Wrong type for argument rule with min and max");

    if (min != NULL && !min->isType(valueType))
        throw RbException("Wrong type for min value");
    if (max != NULL && !max->isType(valueType))
        throw RbException("Wrong type for max value");

    minVal = min;
    maxVal = max;
}


/** Copy constructor */
MinmaxRule::MinmaxRule(const MinmaxRule& x)
    : ArgumentRule(x) {

    if (x.minVal != NULL)
        minVal = x.minVal->clone();
    else
        minVal = NULL;

    if (x.maxVal != NULL)
        maxVal = x.maxVal->clone();
    else
        maxVal = NULL;
}


/** Destructor deletes relevant values */
MinmaxRule::~MinmaxRule(void) {

    if (minVal != NULL)
        delete minVal;
    if (maxVal != NULL)
        delete maxVal;
}


/** Assignment operator */
MinmaxRule& MinmaxRule::operator=(const MinmaxRule& x) {

    if (this != &x) {
        ArgumentRule::operator=(x);

        if (minVal != NULL)
            delete minVal;
        if (maxVal != NULL)
            delete maxVal;
 
        if (x.minVal != NULL)
            minVal = x.minVal->clone();
        else
            minVal = NULL;

        if (x.maxVal != NULL)
            maxVal = x.maxVal->clone();
        else
            maxVal = NULL;
    }

    return (*this);
}


/** Get class vector describing type of object */
const VectorString& MinmaxRule::getClass(void) const { 

    static VectorString rbClass = VectorString(MinmaxRule_name) + ArgumentRule::getClass();
	return rbClass; 
}


/** Test if argument is valid; we evaluate it here if not done previously */
bool MinmaxRule::isArgValid(DAGNode* var) const {

    if (!ArgumentRule::isArgValid(var))
        return false;

    const RbObject* val = var->getValue();
    if (val == NULL)
        return false;

    if (val->isType(Real_name)) {
        double x = ((Real*)(val))->getValue();
        if (minVal != NULL) {
            double min = ((Real*)(minVal))->getValue();
            if (x < min)
                return false;
        }
        if (maxVal != NULL) {
            double max = ((Real*)(maxVal))->getValue();
            if (x > max)
                return false;
        }
    }
    else if (val->isType(Integer_name)) {
        int x = ((Integer*)(val))->getValue();
        if (minVal != NULL) {
            int min = ((Integer*)(minVal))->getValue();
            if (x < min)
                return false;
        }
        if (maxVal != NULL) {
            int max = ((Integer*)(maxVal))->getValue();
            if (x > max)
                return false;
        }
 
    }
    else {
        throw RbException("Value is not comparable");
    }

    return true;
}


/** Print value for user */
void MinmaxRule::printValue(std::ostream& o) const {

    ArgumentRule::printValue(o);

    if (minVal != NULL) {
        o << " [min = " << *minVal;
        if (maxVal != NULL)
            o << ", max = " << *maxVal;
        o << "]";
    }
    else if (maxVal != NULL)
        o << " [max = " << *maxVal << "]";
}


/** Provide complete information about object */
std::string MinmaxRule::toString(void) const {

    std::ostringstream o;

    o << "MinmaxRule:" << std::endl;
    o << "label = " << label << std::endl;
    o << "valueType = "  << valueType << std::endl;
    o << "dim  = "  << dim << std::endl;
    o << "defaultVariable = ";
    if (defaultVariable == NULL)
        o << "NULL";
    else
        defaultVariable->printValue(o);
    o << std::endl;
    o << "wrapperRule = " << wrapperRule << std::endl;
    o << "min         = ";
    if (minVal != NULL)
        minVal->printValue(o);
    else
        o << "NULL";
    o << std::endl;
    o << "max         = ";
    if (maxVal != NULL)
        maxVal->printValue(o);
    else
        o << "NULL";
    o << std::endl;

    return o.str();
}


