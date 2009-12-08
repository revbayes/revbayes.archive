/**
 * @file
 * This file contains the implementation of RbFunction_dbeta, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dbeta
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
#include "RbFunction_dbeta.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_dbeta::RbFunction_dbeta(void) : RbFunction() {

	value = new RbDouble(0.5);
} 


/** Copy constructor */
RbFunction_dbeta::RbFunction_dbeta(const RbFunction_dbeta& s) : RbFunction(s) {
    
	value = new RbDouble(0.5);
	*value = *s.value;
}


/** Destructor */
RbFunction_dbeta::~RbFunction_dbeta(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_dbeta::clone(void) const {

    RbObject *x = new RbFunction_dbeta( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_dbeta::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *alpha = (RbDouble*) arguments[1]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[2]->getValue();
    value->setValue( RbStatistics::Beta::pdf(*alpha, *beta, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_dbeta::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"    , RbDouble_name, new RbUndefined  , new RbDouble(0.0), new RbDouble(1.0) ),
		new ArgumentRule( "alpha", RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined   ),
		new ArgumentRule( "beta" , RbDouble_name, new RbDouble(1.0), new RbDouble(0.0), new RbUndefined   ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_dbeta::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dbeta_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_dbeta::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_dbeta::toString(void) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *alpha = (RbDouble*) arguments[1]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[2]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Beta: f( " << x->getValue() << " | " << alpha->getValue() << ", " << beta->getValue() << " ) = " << value->getValue();
    return o.str();
}

