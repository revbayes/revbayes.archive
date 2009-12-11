/**
 * @file
 * This file contains the partial implementation of RbComplex,
 * which is the RevBayes abstract base class for complex objects.
 *
 * @brief Partial implementation of RbComplex
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-08-23 15:04:35 +0200 (Sön, 23 Aug 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 *
 * $Id: RbDataType.h 9 2009-08-23 13:04:35Z ronquist $
 */

#include <map>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>

#include "MethodTable.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbObjectWrapper.h"
#include "StringVector.h"

#include <sstream>


/** Execute method with preset args: map to builtin or to function object if class has the method */
const RbObject* RbComplex::executeMethod(int funcId) {

    std::ostringstream msg;
    msg << "No member method with id " << funcId;
    throw (RbException(msg.str()));
}


/** Execute method with args passed in: map to builtin function or to function object */
const RbObject* RbComplex::executeMethod(const std::string& funcName, std::vector<Argument>& args) {

    throw (RbException("No member method '" + funcName + "'"));
}


/** Get element (atomic) class vector; same as class vector if no elements */
const StringVector& RbComplex::getAtomicClass() const {

    return getClass();
}


/** Get class vector describing type of object */
const StringVector& RbComplex::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::RbComplex::name) + RbObject::getClass();
	return rbClass;
}


/** Get element */
const RbObject* RbComplex::getElement(const IntVector& index) const {

    throw (RbException("Object does not have elements"));
}


/** Get element length in each dimension */
const IntVector& RbComplex::getElementLength() const {

    throw (RbException("Object does not have elements"));
}


/** Get names of member variables */
const std::vector<std::string>& RbComplex::getMembers(void) const {

    static std::vector<std::string> members;
    return members;
}


/** Get type: return type of member variable, with an empty '[]' for each element dimension, if any */
const std::string& RbComplex::getMemberType(const std::string& name) const {

    throw (RbException("Object does not have elements"));
}


/** Get method descriptions: return empty table by default */
const MethodTable& RbComplex::getMethodTable(void) const {

    static MethodTable methodTable;
    return methodTable;
}


/** Get member value */
const RbObject* RbComplex::getValue(const std::string& name) const {

    throw (RbException("No member '" + name + "'"));
}


/** Get member variable */
const RbObjectWrapper* RbComplex::getVariable(const std::string& name) const {

    throw (RbException("No member '" + name + "'"));
}


/** Set arguments of method; return an internal function id number */
int RbComplex::setArguments(const std::string& funcName, std::vector<Argument>& args) {

    throw (RbException("No member method '" + funcName + "'"));
}


/** Set member value element */
void RbComplex::setValElement(const std::string& name, const IntVector& index, RbObject* val) {

    delete val;
    throw (RbException("No member '" + name + "'"));
}


/** Set member value */
void RbComplex::setValue(const std::string& name, RbObject* val) {

    delete val;
    throw (RbException("No member '" + name + "'"));
}


/** Set member variable element */
void RbComplex::setVarElement(const std::string& name, const IntVector& index, RbObjectWrapper* var) {

    delete var;
    throw (RbException("No member '" + name + "'"));
}


/** Set member variable */
void RbComplex::setVariable(const std::string& name, RbObjectWrapper* var) {

    delete var;
    throw (RbException("No member '" + name + "'"));
}


/** Resize container */
void RbComplex::resize(const IntVector& len) {

    throw (RbException("Object does not have elements"));
}


/** Set element */
void RbComplex::setElement(const IntVector& index, RbObject* val) {

    delete val;
    throw (RbException("Object does not have elements"));
}


/** Set element length */
void RbComplex::setElementLength(const IntVector& len) {

    throw (RbException("Object does not have elements"));
}


