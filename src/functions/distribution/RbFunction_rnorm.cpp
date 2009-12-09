/**
 * @file
 * This file contains the implementation of RbFunction_rnorm, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_rnorm
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
#include "RbFunction_rnorm.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_rnorm::RbFunction_rnorm(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_rnorm::RbFunction_rnorm(const RbFunction_rnorm& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_rnorm::~RbFunction_rnorm(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_rnorm::clone(void) const {

    RbObject *x = new RbFunction_rnorm( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_rnorm::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *mu    = (RbDouble*) arguments[0]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[1]->getValue();
	RandomNumberFactory& rnf = RandomNumberFactory::randomNumberFactoryInstance();
	RandomNumberGenerator* rng = rnf.getGlobalRandomNumberGenerator();
    value->setValue( RbStatistics::Normal::rv(*mu, *sigma, rng) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_rnorm::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "mu"   , RbDouble_name, new RbDouble(0.0)                                     ),
		new ArgumentRule( "sigma", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_rnorm::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dnorm_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_rnorm::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_rnorm::toString(void) const {

    RbDouble *mu    = (RbDouble*) arguments[0]->getValue();
    RbDouble *sigma = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << value->getValue() << " ~ Normal( " << mu->getValue() << ", " << sigma->getValue() << " )";
    return o.str();
}




