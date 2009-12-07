/**
 * @file
 * This file contains the implementation of RbFunction_dnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dnorm
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
#include "RbException.h"
#include "RbFunction_dnorm.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <cmath>
#include <string>



/** Default constructor */
RbFunction_dnorm::RbFunction_dnorm(void) : RbFunction() {

	value = new RbDouble(0.0);
} 

/** Copy constructor */
RbFunction_dnorm::RbFunction_dnorm(const RbFunction_dnorm& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}

/** Destructor */
RbFunction_dnorm::~RbFunction_dnorm(void) {

    delete value;
}

/** Clone */
RbObject* RbFunction_dnorm::clone(void) const {

    RbObject *x = new RbFunction_dnorm( *this );
    return x;
}

/** Equals: Check equality */
bool RbFunction_dnorm::equals(const RbObject* o) const {

    return false;
}

/** Get string showing inheritance */
const StringVector& RbFunction_dnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::Normal::dname) + RbFunction::getClass();
	return rbClass;
}

/** Print value */
void RbFunction_dnorm::printValue(std::ostream &o) const {

    o << value << std::endl;
}

/** Get string showing inheritance */
std::string RbFunction_dnorm::toString(void) const {

    std::ostringstream o;
    o << "Value = " << std::fixed << std::setprecision(6) << value->getValue();
    return o.str();
}

void RbFunction_dnorm::dump(std::ostream& c) {

    //TODO implement
    std::string message = "Dump function of RbFunction_dnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

void RbFunction_dnorm::resurrect(const RbDumpState& x) {

    //TODO implement
    std::string message = "Resurrect function of RbFunction_dnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

RbObject* RbFunction_dnorm::executeOperation(const std::vector<DAGNode*>& arguments) {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

    value->setValue(RbStatistics::Normal::pdf(*mu,*sigma,*x));

    return value;
}

const ArgumentRule** RbFunction_dnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule("x", "double"), 
		new ArgumentRule("mu", "double", new RbDouble(0.0)),
		new ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL),
		NULL };
	return argRules;
}

const int RbFunction_dnorm::getNumberOfRules(void) const {
    return 1;
}

const std::string RbFunction_dnorm::getReturnType(void) const {

	const static std::string returnType  = "double";
	return returnType;
}




