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

#include "ContainerIterator.h"
#include "DAGNode.h"
#include "IntVector.h"
#include "RbComplex.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"

#include <map>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>


/** Return begin iterator */
ContainerIterator RbComplex::begin(void) const {

    IntVector temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i] = 0;

    return ContainerIterator(temp, getLength());
}

/** Return end iterator */
ContainerIterator RbComplex::end(void) const {

    IntVector temp = getLength();
    for (size_t i=0; i<temp.size(); i++)
        temp[i]--;

    ContainerIterator tempIt(temp, getLength());
    return ++tempIt;
}


/** Execute method with args preprocessed: map to builtin function or to function object */
const RbObject* RbComplex::executeMethod(const std::string& name, int funcId) {

    throw (RbException("No member method '" + name + "'"));
}


/** Execute method with args passed in: map to builtin function or to function object */
const RbObject* RbComplex::executeMethod(const std::string& name, std::vector<Argument>& args) {

    throw (RbException("No member method '" + name + "'"));
}


/** Get class vector describing type of object */
const StringVector& RbComplex::getClass(void) const { 

    static StringVector rbClass = StringVector(RbComplex_name) + RbObject::getClass();
	return rbClass;
}


/** Get element */
const RbObject* RbComplex::getElement(const IntVector& index) const {

    throw (RbException("Object does not have elements"));
}


/** Get element length in each dimension */
const IntVector& RbComplex::getLength() const {

    throw (RbException("Object does not have elements"));
}


/** Get member rules */
const MemberRules& RbComplex::getMemberRules(void) const {

    throw (RbException("Object does not have members"));
}


/** Get member variable table */
const MemberTable& RbComplex::getMembers(void) const {

    throw (RbException("Object does not have members"));
}


/** Get method description table */
const MethodTable& RbComplex::getMethods(void) const {

    throw (RbException("Object does not have functions"));
}


/** Get member value */
const RbObject* RbComplex::getValue(const std::string& name) const {

    throw (RbException("No member '" + name + "'"));
}


/** Get member variable */
const DAGNode* RbComplex::getVariable(const std::string& name) const {

    throw (RbException("No member '" + name + "'"));
}

/** Set arguments of method; return an internal function id number */
int RbComplex::setArguments(const std::string& name, std::vector<Argument>& args) {

    throw (RbException("No member method '" + name + "'"));
}


/** Set member value */
void RbComplex::setValue(const std::string& name, RbObject* val) {

    delete val;
    throw (RbException("No member '" + name + "'"));
}


/** Set member variable */
void RbComplex::setVariable(const std::string& name, DAGNode* var) {

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
void RbComplex::setLength(const IntVector& len) {

    throw (RbException("Object does not have elements"));
}


