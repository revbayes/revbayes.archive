/**
 * @file
 * This file contains the implementation of RbFunction_pnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_pnorm
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
#include "RbFunction_pnorm.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <cmath>
#include <string>



RbFunction_pnorm::RbFunction_pnorm(void) : RbFunction() {

	value = new RbDouble(0.0);
} 

RbFunction_pnorm::RbFunction_pnorm(const RbFunction_pnorm& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}

RbFunction_pnorm::~RbFunction_pnorm(void) {

    delete value;
}

RbObject* RbFunction_pnorm::clone(void) const {

    RbObject *x = new RbFunction_pnorm( *this );
    return x;
}

bool RbFunction_pnorm::equals(const RbObject* o) const {

    return false;
}

const StringVector& RbFunction_pnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbNames::Normal::pname) + RbFunction::getClass();
	return rbClass;
}

void RbFunction_pnorm::printValue(std::ostream &o) const {

    o << value << std::endl;
}

std::string RbFunction_pnorm::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value->getValue());
	std::string tempStr = temp;
    return "Value = " + tempStr;
}

void RbFunction_pnorm::dump(std::ostream& c) {

    //TODO implement
    std::string message = "Dump function of RbFunction_pnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

void RbFunction_pnorm::resurrect(const RbDumpState& x) {

    //TODO implement
    std::string message = "Resurrect function of RbFunction_pnorm not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

RbObject* RbFunction_pnorm::executeOperation(const std::vector<DAGNode*>& arguments) {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();

    value->setValue(RbStatistics::Normal::pdf(*mu, *sigma, *x));

    return value;
}

const ArgumentRule** RbFunction_pnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule("q", "double"), 
		new ArgumentRule("mu", "double", new RbDouble(0.0)),
		new ArgumentRule("sigma", "double", new RbDouble(1.0), new RbDouble(0.0), NULL),
		NULL };
	return argRules;
}

const int RbFunction_pnorm::getNumberOfRules(void) const {
    return 1;
}

const std::string RbFunction_pnorm::getReturnType(void) const {

	const static std::string returnType  = "double";
	return returnType;
}

