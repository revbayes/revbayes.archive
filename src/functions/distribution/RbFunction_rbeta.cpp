/**
 * @file
 * This file contains the implementation of RbFunction_rbeta, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_rbeta
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
#include "RbFunction_rbeta.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_rbeta::RbFunction_rbeta(void) : RbFunction() {

	value = new RbDouble(0.5);
} 


/** Copy constructor */
RbFunction_rbeta::RbFunction_rbeta(const RbFunction_rbeta& s) : RbFunction(s) {
    
	value = new RbDouble(0.5);
	*value = *s.value;
}


/** Destructor */
RbFunction_rbeta::~RbFunction_rbeta(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_rbeta::clone(void) const {

    RbObject *x = new RbFunction_rbeta( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_rbeta::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *alpha = (RbDouble*) arguments[0]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[1]->getValue();
	RandomNumberFactory& rnf = RandomNumberFactory::randomNumberFactoryInstance();
	RandomNumberGenerator* rng = rnf.getGlobalRandomNumberGenerator();
    value->setValue( RbStatistics::Beta::rv(*alpha, *beta, rng) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_rbeta::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "alpha", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined   ),
		new ArgumentRule( "beta" , RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined   ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_rbeta::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_rbeta_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_rbeta::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_rbeta::toString(void) const {

    RbDouble *alpha = (RbDouble*) arguments[0]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << value->getValue() << " ~ Beta( " << alpha->getValue() << ", " << beta->getValue() << " )";
    return o.str();
}

