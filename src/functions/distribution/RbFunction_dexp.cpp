/**
 * @file
 * This file contains the implementation of RbFunction_dexp, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dexp
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
#include "RbFunction_dexp.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_dexp::RbFunction_dexp(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_dexp::RbFunction_dexp(const RbFunction_dexp& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_dexp::~RbFunction_dexp(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_dexp::clone(void) const {

    RbObject *x = new RbFunction_dexp( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_dexp::executeOperation(const std::vector<RbObjectWrapper*>& arguments) const {

    RbDouble *x    = (RbDouble*) arguments[0]->getValue();
    RbDouble *rate = (RbDouble*) arguments[1]->getValue();
    value->setValue( RbStatistics::Exponential::pdf(*rate, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_dexp::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"   , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		new ArgumentRule( "rate", RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_dexp::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dexp_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_dexp::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_dexp::toString(void) const {

    RbDouble *x    = (RbDouble*) processedArguments[0]->getValue();
    RbDouble *rate = (RbDouble*) processedArguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Exponential: f( " << x->getValue() << " | " << rate->getValue() << " ) = " << value->getValue();
    return o.str();
}


