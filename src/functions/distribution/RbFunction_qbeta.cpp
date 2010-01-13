/**
 * @file
 * This file contains the implementation of RbFunction_qbeta, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_qbeta. (Also implements ssRNA bacteriophage qBeta.)
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
#include "RbFunction_qbeta.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_qbeta::RbFunction_qbeta(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_qbeta::RbFunction_qbeta(const RbFunction_qbeta& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_qbeta::~RbFunction_qbeta(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_qbeta::clone(void) const {

    RbObject *x = new RbFunction_qbeta( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_qbeta::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *alpha = (RbDouble*) arguments[1]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[2]->getValue();
    value->setValue( RbStatistics::Beta::quantile(*alpha, *beta, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_qbeta::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x"    , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbDouble(1.0) ),
		new ArgumentRule( "alpha", RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined   ),
		new ArgumentRule( "beta" , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined   ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_qbeta::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_qbeta_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_qbeta::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_qbeta::toString(void) const {

    RbDouble *x     = (RbDouble*) arguments[0]->getValue();
    RbDouble *alpha = (RbDouble*) arguments[1]->getValue();
    RbDouble *beta  = (RbDouble*) arguments[2]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Beta: q( " << x->getValue() << " | " << alpha->getValue() << ", " << beta->getValue() << " ) = " << value->getValue();
    return o.str();
}


