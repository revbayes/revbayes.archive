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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbDouble.h"
#include "RbFunction_pow.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbUndefined.h"
#include <cmath>
#include <string>

/** Default constructor */
RbFunction_pow::RbFunction_pow(void) :
    RbFunction() {

    value = new RbDouble(0.0);
}

/** Copy constructor */
RbFunction_pow::RbFunction_pow(const RbFunction_pow& s) :
    RbFunction(s) {

    value = new RbDouble(0.0); // ?!?
    *value = *s.value;
}

/** Destructor */
RbFunction_pow::~RbFunction_pow(void) {

    delete value;
}

/** Clone */
RbObject* RbFunction_pow::clone(void) const {

    RbObject *x = new RbFunction_pow(*this);
    return x;
}

/** Execute the function */
const RbObject* RbFunction_pow::executeOperation(
        const std::vector<RbObjectWrapper*>& arguments) const {

    /* Get actual argument */
    RbDouble *arg1 = (RbDouble*) arguments[0]->getValue();
    RbDouble *arg2 = (RbDouble*) arguments[1]->getValue();

    /* Compute result */
    value->setValue(std::pow(arg1->getValue(), arg2->getValue()));

    return value;

}

/** Get the argument rules */
const ArgumentRule** RbFunction_pow::getArgumentRules(void) const {

    const static ArgumentRule* argRules[] = { new ArgumentRule("x",
            RbNames::Double::name, new RbDouble(1.0), new RbDouble(0.0),
            new RbUndefined), new ArgumentRule("y", RbNames::Double::name,
            new RbDouble(1.0), new RbDouble(0.0), new RbUndefined), NULL };
    return argRules;
}

/** Get string showing inheritance */
const StringVector& RbFunction_pow::getClass(void) const {

    static StringVector rbClass = StringVector(RbFunction_pow_name)
            + RbFunction::getClass();
    return rbClass;
}

/** Get the return type */
const std::string RbFunction_pow::getReturnType(void) const {

    const static std::string returnType = RbNames::Double::name;
    return returnType;
}

/** Get string showing value */
std::string RbFunction_pow::toString(void) const {

    char v[30];
    sprintf(v, "%1.6lf", value->getValue());
    std::string vStr = v;

    RbDouble *x = (RbDouble*) processedArguments[0]->getValue();
    char arg[30];
    sprintf(arg, "%1.6lf", x->getValue());
    std::string xStr = arg;
    RbDouble *y = (RbDouble*) processedArguments[1]->getValue();
    char argY[30];
    sprintf(argY, "%1.6lf", x->getValue());
    std::string yStr = argY;
    std::string str = xStr + "^" + yStr +" = " + vStr;
    return str;
}

