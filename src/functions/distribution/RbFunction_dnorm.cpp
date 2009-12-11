/**
 * @file
 * This file contains the implementation of RbFunction_dnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dnorm
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
#include "RbFunction_dnorm.h"
#include "RbObject.h"
#include "RbObjectWrapper.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "RbUndefined.h"
#include <iomanip>
#include <sstream>
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

    RbFunction_dnorm *x = new RbFunction_dnorm( *this );
    return (RbObject*)x;
}


/** Execute the function */
const RbObject* RbFunction_dnorm::executeOperation(const std::vector<RbObjectWrapper*>& args) const {

    RbDouble *x     = (RbDouble*) args[0]->getValue();
    RbDouble *mu    = (RbDouble*) args[1]->getValue();
    RbDouble *sigma = (RbDouble*) args[2]->getValue();
    value->setValue( RbStatistics::Normal::pdf(*mu, *sigma, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_dnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"    , RbDouble_name                                                      ), 
		new ArgumentRule( "mu"   , RbDouble_name                                                      ),
		new ArgumentRule( "sigma", RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined() ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_dnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dnorm_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_dnorm::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_dnorm::toString(void) const {

    RbDouble *x     = (RbDouble*) processedArguments[0]->getValue();
    RbDouble *mu    = (RbDouble*) processedArguments[1]->getValue();
	RbDouble *sigma = (RbDouble*) processedArguments[2]->getValue();

    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Normal: f( " << x->getValue() << " | " << mu->getValue() << ", " << sigma->getValue() << " ) = " << value->getValue();
    return o.str();
}




