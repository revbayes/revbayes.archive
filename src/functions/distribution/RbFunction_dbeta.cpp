/**
 * @file
 * This file contains the implementation of RbFunction_dbeta, the
 * dbeta() function.
 *
 * @brief Implementation of RbFunction_dbeta
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

#include "RbFunction_dbeta.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include <cmath>

const StringVector RbFunction_dbeta::rbClass = StringVector("dbeta") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_dbeta = SymbolTable::globalTable().add("dbeta", new RbFunction_dbeta());


/** Default constructor, allocate workspace */
RbFunction_dbeta::RbFunction_dbeta(void)
    : RbFunction() {

    RbUndefined undef;
    RbDouble x_min = RbDouble(0.0);
    RbDouble x_max = RbDouble(1.0);
    RbDouble a_def = RbDouble(1.0);
    RbDouble a_min = RbDouble(0.0);
    RbDouble b_def = RbDouble(1.0);
    RbDouble b_min = RbDouble(0.0);

	argRules.push_back( ArgumentRule("x", RbNames::Double::name, undef, x_min, x_max) );
	argRules.push_back( ArgumentRule("alpha", RbNames::Double::name, a_def, a_min, undef) );
	argRules.push_back( ArgumentRule("beta", RbNames::Double::name, b_def, b_min, undef) );
	returnType = RbNames::Double::name;
} 

/** Copy constructor */
RbFunction_dbeta::RbFunction_dbeta(const RbFunction_dbeta& s)
    : RbFunction(s) {

    RbUndefined undef;
    RbDouble x_min = RbDouble(0.0);
    RbDouble x_max = RbDouble(1.0);
    RbDouble a_def = RbDouble(1.0);
    RbDouble a_min = RbDouble(0.0);
    RbDouble b_def = RbDouble(1.0);
    RbDouble b_min = RbDouble(0.0);

    argRules.push_back( ArgumentRule("x", RbNames::Double::name, undef, x_min, x_max) );
    argRules.push_back( ArgumentRule("alpha", RbNames::Double::name, a_def, a_min, undef) );
    argRules.push_back( ArgumentRule("beta", RbNames::Double::name, b_def, b_min, undef) );
    returnType = RbNames::Double::name;
}

/** Destructor, delete workspace */
RbFunction_dbeta::~RbFunction_dbeta() {

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
RbObject* RbFunction_dbeta::clone(void) const {

    RbObject *x = new RbFunction_dbeta( *this );
    return x;
}

RbObject& RbFunction_dbeta::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_dbeta& x = dynamic_cast<const RbFunction_dbeta&> (obj);

        RbFunction_dbeta& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_dbeta& x = dynamic_cast<const RbFunction_dbeta&> (*(obj.convertTo("dbeta")));

            RbFunction_dbeta& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to dbeta");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_dbeta& RbFunction_dbeta::operator=(const RbFunction_dbeta& obj) {

    argRules = obj.argRules;
    returnType = obj.returnType;
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
void RbFunction_dbeta::print(std::ostream &c) const {

    c << "RbFunction_dbeta" << std::endl;
}

void RbFunction_dbeta::printValue(std::ostream &o) const {

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
void RbFunction_dbeta::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_dbeta not fully implemented!";
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
void RbFunction_dbeta::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_dbeta not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_dbeta::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value->getValue());
	std::string tempStr = temp;
    return "Value = " + tempStr;
}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_dbeta::equals(const RbObject* o) const {

    return false;
}

/** Execute function */
RbObject* RbFunction_dbeta::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

    value->setValue(RbStatistics::Beta::pdf(*mu,*sigma,*x));

    return value;
}

RbObject* RbFunction_dbeta::convertTo(const std::string& type) const {

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
bool RbFunction_dbeta::isConvertibleTo(const std::string& type) const {

    return false;
}
