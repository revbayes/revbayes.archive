/**
 * @file
 * This file contains the implementation of RbFunction_dchisq, the
 * dchisq() function.
 *
 * @brief Implementation of RbFunction_dchisq
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

#include "DAGNode.h"
#include "RbFunction_dchisq.h"
#include "RbDouble.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbException.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include <cmath>

const StringVector RbFunction_dchisq::rbClass = StringVector("dchisq") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_dchisq = SymbolTable::globalTable().add("dchisq", new RbFunction_dchisq());


/** Default constructor, allocate workspace */
RbFunction_dchisq::RbFunction_dchisq(void)
    : RbFunction(), value(new RbDouble(0)) {
    RbUndefined undef;

    RbDouble x_min = RbDouble(0.0);
    RbInt df_def = RbInt(1);
    RbInt df_min = RbInt(1);

	argRules.push_back( ArgumentRule("x", RbNames::Double::name, undef, x_min, undef) );
	argRules.push_back( ArgumentRule("df", RbNames::Int::name, df_def, df_min, undef) );
	returnType = RbNames::Double::name;
} 

/** Copy constructor */
RbFunction_dchisq::RbFunction_dchisq(const RbFunction_dchisq& s)
    : RbFunction(s), value(new RbDouble(0)) {
    RbUndefined undef;

    RbDouble x_min = RbDouble(0.0);
    RbInt df_def = RbInt(1);
    RbInt df_min = RbInt(1);

    argRules.push_back( ArgumentRule("x", RbNames::Double::name, undef, x_min, undef) );
    argRules.push_back( ArgumentRule("df", RbNames::Int::name, df_def, df_min, undef) );
    returnType = RbNames::Double::name;
}

/** Destructor, delete workspace */
RbFunction_dchisq::~RbFunction_dchisq() {

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
RbObject* RbFunction_dchisq::clone(void) const {

    RbObject *x = new RbFunction_dchisq( *this );
    return x;
}

RbObject& RbFunction_dchisq::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_dchisq& x = dynamic_cast<const RbFunction_dchisq&> (obj);

        RbFunction_dchisq& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_dchisq& x = dynamic_cast<const RbFunction_dchisq&> (*(obj.convertTo("dchisq")));

            RbFunction_dchisq& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to dchisq");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_dchisq& RbFunction_dchisq::operator=(const RbFunction_dchisq& obj) {

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
void RbFunction_dchisq::printValue(std::ostream &o) const {

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
void RbFunction_dchisq::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_dchisq not fully implemented!";
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
void RbFunction_dchisq::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_dchisq not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_dchisq::toString(void) const {

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
bool RbFunction_dchisq::equals(const RbObject* o) const {

    return false;
}

/** Execute function */
RbObject* RbFunction_dchisq::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x      = (RbDouble*) arguments[0]->getValue();
    RbInt    *df     = (RbInt*) arguments[1]->getValue();

    value->setValue(RbStatistics::ChiSquare::pdf(*df,*x));

    return value;
}

RbObject* RbFunction_dchisq::convertTo(const std::string& type) const {

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
bool RbFunction_dchisq::isConvertibleTo(const std::string& type) const {

    return false;
}
