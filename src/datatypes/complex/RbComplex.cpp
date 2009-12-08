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

#include "RbComplex.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"

#include <sstream>


/** Get class vector describing type of object */
const StringVector& RbComplex::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::RbComplex::name) + RbObject::getClass();
	return rbClass;
}


/** Execute method with preset args: map to builtin or to function object if class has the method */
RbObject* RbComplex::executeMethod(int funcId) {

    std::ostringstream msg;
    msg << "No member method with id " << funcId;
    throw (RbException(msg));
}


/** Execute method with args passed in: map to builtin function or to function object */
RbObject* RbComplex::executeMethod(const std::string& funcName, std::vector<Argument>& args) {

    throw (RbException("No member method '" + funcName + "'"));
}


/** Does the named method exist? */
bool RbComplex::existsMethod(const std::string& funcName, std::vector<Argument>& args) const {

	return false;
}


/** Get method descriptions for all overloaded functions with the same name; get all if name is "" */
std::vector<FunctionDescr> RbComplex::getMethodDescr(const std::string& name) const {

    return std::vector<FunctionDescr>();
}


/** Set arguments of method */
void RbComplex::setArguments(const std::string& funcName, std::vector<Argument>& args) {

    throw (RbException("No member method '" + funcName + "'"));
}


/**
 * @brief Print object info
 *
 * Print object info to string, including type, member variables
 * and their values, and member functions.
 *
 * @param o The ostream for printing
 *
 */
std::string RbComplex::toString() const {

    std::ostringstream o;

    for (std::map<std::string, RbObject*>::const_iterator i=members.begin(); i!=members.end(); i++) {
        o << "." << i->first << " = ";
        o << getMember(i->first)->briefInfo();
    }

//    std::map<std::string, RbFunction*> methods = getMethods();
//
//    for (std::map<std::string, RbFunction*>::iterator i=methods.begin(); i!=methods.end(); i++) {
//    	i->second->print(o);
//    }

    return o.str();
}


/** Add empty slot for a member variable */
bool RbComplex::addMember(const std::string& name, const std::string& type) {

    throw (RbException("Object does not allow the addition of member variables"));
}


/** Add member variable */
bool RbComplex::addMember(const std::string& name, RbObjectWrapper* val) {

    throw (RbException("Object does not allow the addition of member variables"));
}


/** Delete member variable */
void RbComplex::deleteMember(const std::string& name) {

    throw (RbException("No member '" + name + "'"));
}


/** Get member variable */
const RbObjectWrapper* RbComplex::getMember(const std::string& name) const {

    throw (RbException("No member '" + name + "'"));
}


/** Get a description of a member variable; the whole list if name="" */
std::vector<MemberDescr> RbComplex::getMemberDescr(const std::string& name) const {

    return std::vector<MemberDescr>();
}


/** Set member variable */
void RbComplex::setMember(const std::string& name, RbObjectWrapper* val) {

    throw (RbException("No member '" + name + "'"));
}


/** Get element */
RbComplex* RbComplex::getElement(const IntVector& index) const {

    throw (RbException("Object does not have elements"));
    return NULL;    // Dummy return
}


/** Get element length in each dimension */
const IntVector& RbComplex::getElementLength() const {

    throw (RbException("Object does not have elements"));
}


/** Get element type */
const std::string& RbComplex::getElementType() const {

    throw (RbException("Object does not have elements"));
}


/** Set element */
void RbComplex::setElement(const IntVector& index, RbObject* val) {

    throw (RbException("Object does not have elements"));
}


