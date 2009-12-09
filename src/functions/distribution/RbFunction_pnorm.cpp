/**
 * @file
 * This file contains the implementation of RbFunction_pnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_pnorm
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
#include "RbFunction_pnorm.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_pnorm::RbFunction_pnorm(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_pnorm::RbFunction_pnorm(const RbFunction_pnorm& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_pnorm::~RbFunction_dnorm(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_pnorm::clone(void) const {

    RbObject *x = new RbFunction_pnorm( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_pnorm::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();
    value->setValue( RbStatistics::Normal::cdf(*mu, *sigma, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_pnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"    , RbDouble_name                                                        ), 
		new ArgumentRule( "mu"   , RbDouble_name, new RbDouble(0.0)                                     ),
		new ArgumentRule( "sigma", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_pnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_pnorm_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_pnorm::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_pnorm::toString(void) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) arguments[1]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[2]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Normal: F( " << x->getValue() << " | " << mu->getValue() << ", " << sigma->getValue() << " ) = " << value->getValue();
    return o.str();
}

