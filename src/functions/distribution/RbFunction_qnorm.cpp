/**
 * @file
 * This file contains the implementation of RbFunction_qnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_qnorm
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
#include "RbFunction_qnorm.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_qnorm::RbFunction_qnorm(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_qnorm::RbFunction_qnorm(const RbFunction_qnorm& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_qnorm::~RbFunction_qnorm(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_qnorm::clone(void) const {

    RbObject *x = new RbFunction_qnorm( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_qnorm::executeOperation(const std::vector<RbObjectWrapper*>& args) const {

    RbDouble *x     = (RbDouble*) args[0]->getValue();
    RbDouble *mu    = (RbDouble*) args[1]->getValue();
    RbDouble *sigma = (RbDouble*) args[2]->getValue();
    value->setValue( RbStatistics::Normal::quantile(*mu, *sigma, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_qnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"    , RbDouble_name                                                      ), 
		new ArgumentRule( "mu"   , RbDouble_name                                                      ),
		new ArgumentRule( "sigma", RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_qnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_qnorm_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_qnorm::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_qnorm::toString(void) const {

	const std::vector<RbObjectWrapper*>& args = getProcessedArguments();
    RbDouble *x     = (RbDouble*) args[0]->getValue();
    RbDouble *mu    = (RbDouble*) args[1]->getValue();
	RbDouble *sigma = (RbDouble*) args[2]->getValue();

    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Normal: q( " << x->getValue() << " | " << mu->getValue() << ", " << sigma->getValue() << " ) = " << value->getValue();
    return o.str();
}

