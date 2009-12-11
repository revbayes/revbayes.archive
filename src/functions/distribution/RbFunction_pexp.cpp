/**
 * @file
 * This file contains the implementation of RbFunction_pexp, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_pexp
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
#include "RbDouble.h"
#include "RbFunction_pexp.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_pexp::RbFunction_pexp(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_pexp::RbFunction_pexp(const RbFunction_pexp& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_pexp::~RbFunction_pexp(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_pexp::clone(void) const {

    RbObject *x = new RbFunction_pexp( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_pexp::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x    = (RbDouble*) arguments[0]->getValue();
    RbDouble *rate = (RbDouble*) arguments[1]->getValue();
    value->setValue( RbStatistics::Exponential::cdf(*rate, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_pexp::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"   , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		new ArgumentRule( "rate", RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_pexp::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_pexp_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_pexp::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_pexp::toString(void) const {

    RbDouble *x    = (RbDouble*) arguments[0]->getValue();
    RbDouble *rate = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Exponential: F( " << x->getValue() << " | " << rate->getValue() << " ) = " << value->getValue();
    return o.str();
}


