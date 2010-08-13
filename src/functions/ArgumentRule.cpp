/**
 * @file
 * This file contains the implementation of ArgumentRule, which is
 * the base class for objects used to describe rules for arguments
 * passed to functions.
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
#include "ConstantNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
#include "Workspace.h"

#include <sstream>


/** Construct rule without default value; use "" for no label. */
ArgumentRule::ArgumentRule(const std::string& argName, const std::string& valType, int varDim)
    : RbInternal() {

    label           = argName;
    valueType       = valType;
    dim             = varDim;
    defaultVariable = NULL;
    wrapperRule     = false;
}


/** Construct rule with explicit type and dim as well as default value. */
ArgumentRule::ArgumentRule(const std::string& argName, const std::string& valType, int varDim, RbObject* defVal)
    : RbInternal() {

    label           = argName;
    valueType       = valType;
    dim             = varDim;
    defaultVariable = new ConstantNode(defVal);
    wrapperRule     = false;
}


/** Construct rule with default value and implicit type and dim from default value. */
ArgumentRule::ArgumentRule(const std::string& argName, RbObject* defVal)
    : RbInternal() {

    label           = argName;
    valueType       = defVal->getType();
    dim             = 0;
    defaultVariable = new ConstantNode(defVal);
    wrapperRule     = false;
}


/** Construct rule with default variable and implicit type and dim from default variable. */
ArgumentRule::ArgumentRule(const std::string& argName, DAGNode* defVar)
    : RbInternal() {

    label           = argName;
    valueType       = defVar->getValueType();
    dim             = defVar->getDim();
    defaultVariable = defVar;
    wrapperRule     = true;
}


/** Copy constructor */
ArgumentRule::ArgumentRule(const ArgumentRule& x)
    : RbInternal(x) {

    label           = x.label;
    valueType       = x.valueType;
    dim             = x.dim;
    wrapperRule     = x.wrapperRule;
    if (x.defaultVariable != NULL && x.wrapperRule == false)
        defaultVariable = x.defaultVariable->clone();
    else
        defaultVariable = x.defaultVariable;
}


/** Destructor deletes relevant values */
ArgumentRule::~ArgumentRule(void) {

    if (!wrapperRule)
        delete defaultVariable;
}


/** Assignment operator */
ArgumentRule& ArgumentRule::operator=(const ArgumentRule& x) {

    if (this != &x) {
        label           = x.label;
        valueType       = x.valueType;
        dim             = x.dim;
        if (defaultVariable != NULL && wrapperRule == false)
            delete defaultVariable;
        if (x.defaultVariable != NULL && x.wrapperRule == false)
            defaultVariable = x.defaultVariable->clone();
        else
            defaultVariable = x.defaultVariable;
        wrapperRule     = x.wrapperRule;
    }

    return (*this);
}


/** Get class vector describing type of object */
const StringVector& ArgumentRule::getClass(void) const { 

    static StringVector rbClass = StringVector(ArgumentRule_name) + RbInternal::getClass();
	return rbClass; 
}


/** Get default value */
RbObject* ArgumentRule::getDefaultValue(void) const {

    if (!hasDefault())
        throw RbException("Missing default value");

    if (defaultVariable->getValue() == NULL)
        return NULL;
    else
        return defaultVariable->getValue()->clone();
}


/** Get default variable */
DAGNode* ArgumentRule::getDefaultVariable(void) const {

    if (!hasDefault())
        throw RbException("Missing default value");

    return defaultVariable->clone();
}


/** Get default variable ptr ('&' argument) */
DAGNode* ArgumentRule::getDefaultVariablePtr(void) const {

    if (!hasDefault())
        throw RbException("Missing default value");

    return defaultVariable;
}


/** Does the argument have a default value? */
bool ArgumentRule::hasDefault(void) const {

    return defaultVariable != NULL;
}


/** Test if argument is valid; for consistency, we also evaluate the argument here */
bool ArgumentRule::isArgValid(DAGNode* var) const {

    if (var == NULL)
        return false;

    const RbObject* val = var->getValue();

    if (dim == var->getDim() && Workspace::userWorkspace().isXOfTypeY(var->getValueType(), valueType) == true)
        return true;

    if (val->isConvertibleTo(valueType, dim) == true)
        return true;

    return false;
}


/** Print value for user (in descriptions of functions, for instance) */
void ArgumentRule::printValue(std::ostream &o) const {

    o << "<" << valueType;
    for (int i=0; i<dim; i++)
        o << "[]";
    if (wrapperRule == true)
        o << "&";
    o << ">";
    o << " \"" << label << "\"";
    if (defaultVariable != NULL) {
        o << " = ";
        if (wrapperRule == true)
            o << defaultVariable->getName();
        else
            defaultVariable->printValue(o);
    }
}


/** Provide complete information about object */
std::string ArgumentRule::toString(void) const {

    std::ostringstream o;

    o << "ArgumentRule:" << std::endl;
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

    return o.str();
}


