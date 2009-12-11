/**
 * @file
 * This file contains the implementation of RbFunction_qchisq, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_qchisq
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
#include "RbFunction_qchisq.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_qchisq::RbFunction_qchisq(void) : RbFunction() {

	value = new RbDouble(0.0);
} 


/** Copy constructor */
RbFunction_qchisq::RbFunction_qchisq(const RbFunction_qchisq& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_qchisq::~RbFunction_qchisq(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_qchisq::clone(void) const {

    RbObject *x = new RbFunction_qchisq( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_qchisq::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x  = (RbDouble*) arguments[0]->getValue();
    RbInt    *df = (RbDouble*) arguments[1]->getValue();
    value->setValue( RbStatistics::ChiSquare::quantile(*df, *x) );
	return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_qchisq::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x" , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ), 
		new ArgumentRule( "df", RbInt_name   , new RbUndefined, new RbInt(1)     , new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_qchisq::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_qchisq_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_qchisq::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_qchisq::toString(void) const {

    RbDouble *x  = (RbDouble*) arguments[0]->getValue();
    RbInt    *df = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Chi-Square: q( " << x->getValue() << " | " << df->getValue() << " ) = " << value->getValue();
    return o.str();
}

