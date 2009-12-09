/**
 * @file
 * This file contains the implementation of RbFunction_rexp, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_rexp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbDouble.h"
#include "RbFunction_rexp.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_rexp::RbFunction_rexp(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_rexp::RbFunction_rexp(const RbFunction_rexp& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_rexp::~RbFunction_rexp(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_rexp::clone(void) const {

    RbObject *x = new RbFunction_rexp( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_rexp::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *rate = (RbDouble*) arguments[0]->getValue();
	RandomNumberFactory& rnf = RandomNumberFactory::randomNumberFactoryInstance();
	RandomNumberGenerator* rng = rnf.getGlobalRandomNumberGenerator();
    value->setValue( RbStatistics::Exponential::rv(*rate, rng) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_rexp::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "rate", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_rexp::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dexp_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_rexp::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_rexp::toString(void) const {

    RbDouble *rate = (RbDouble*) arguments[0]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << value->getValue() << " ~ Exponential( " << rate->getValue() << " )";
    return o.str();
}


