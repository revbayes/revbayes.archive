/**
 * @file
 * This file contains the implementation of RbFunction_qexp, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_qexp
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
#include "RbFunction_qexp.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_qexp::RbFunction_qexp(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_qexp::RbFunction_qexp(const RbFunction_qexp& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_qexp::~RbFunction_qexp(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_qexp::clone(void) const {

    RbObject *x = new RbFunction_qexp( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_qexp::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x    = (RbDouble*) arguments[0]->getValue();
    RbDouble *rate = (RbDouble*) arguments[1]->getValue();
    value->setValue( RbStatistics::Exponential::quantile(*rate, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_qexp::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"   , RbDouble_name, new RbUndefined  , new RbDouble(0.0), new RbUndefined ),
		new ArgumentRule( "rate", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_qexp::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_qexp_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_qexp::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_qexp::toString(void) const {

    RbDouble *x    = (RbDouble*) arguments[0]->getValue();
    RbDouble *rate = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Exponential: q( " << x->getValue() << " | " << rate->getValue() << " ) = " << value->getValue();
    return o.str();
}

