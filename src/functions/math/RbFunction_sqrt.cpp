/**
 * @file
 * This file contains the implementation of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Implementation of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "RbFunction_sqrt.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbNames.h"
#include <cmath>

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_sqrt = SymbolTable::globalTable().add("sqrt", new RbFunction_sqrt());


/** Default constructor, allocate workspace */
RbFunction_sqrt::RbFunction_sqrt(void)
    : RbFunction() {

    value = new RbDouble(0.0);
} 

/** Copy constructor */
RbFunction_sqrt::RbFunction_sqrt(const RbFunction_sqrt& s)
    : RbFunction(s) {
    
    value = new RbDouble(0.0);
}

/** Destructor, delete workspace */
RbFunction_sqrt::~RbFunction_sqrt() {

    delete value;
}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* RbFunction_sqrt::clone(void) const {

    RbObject *x = new RbFunction_sqrt( *this );
    return x;
}


RbFunction_sqrt& RbFunction_sqrt::operator=(const RbFunction_sqrt& obj) {
    (*value) = (*obj.value);
    return (*this);
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void RbFunction_sqrt::print(std::ostream &c) const {

    c << "RbFunction_sqrt" << std::endl;
}

void RbFunction_sqrt::printValue(std::ostream &o) const {

    o << value << std::endl;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbFunction_sqrt::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_sqrt not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void RbFunction_sqrt::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_sqrt not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_sqrt::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value->getValue());
	std::string tempStr = temp;
    return "Value = " + tempStr;
}

const std::string RbFunction_sqrt::getReturnType(void) const {

    const static std::string returnType = RbNames::Double::name;
    return returnType;
}

/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_sqrt::equals(const RbObject* o) const {

    return false;
}


/** Get string showing inheritance */
const StringVector& RbFunction_sqrt::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::Sqrt::name) + RbFunction::getClass();
    return rbClass;
}

/** Get the argument rules */
const ArgumentRule** RbFunction_sqrt::getArgumentRules(void) const {

    const static ArgumentRule* argRules[] = {
        new ArgumentRule("x", "double"),
        NULL };
    return argRules;
}

/** Execute function */
const RbObject* RbFunction_sqrt::executeOperation(const std::vector<RbObjectWrapper*>& arguments) const {

    /* Get actual argument */
    RbDouble *arg = (RbDouble*) arguments[0]->getValue();

    /* Compute result */
    if ( arg->getValue() < 0.0 )
        value->setValue(1E-100);
    else
        value->setValue(std::sqrt(arg->getValue()));

    return value;
}

RbObject* RbFunction_sqrt::convertTo(const std::string& type) const {

    return NULL;
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbFunction_sqrt::isConvertibleTo(const std::string& type) const {

    return false;
}
