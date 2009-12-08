/**
 * @file
 * This file contains the implementation of RbFunction_dchisq, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_dchisq
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
#include "RbFunction_dchisq.h"
#include "RbInt.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_dchisq::RbFunction_dchisq(void) : RbFunction() {

	value = new RbDouble(1.0);
} 


/** Copy constructor */
RbFunction_dchisq::RbFunction_dchisq(const RbFunction_dchisq& s) : RbFunction(s) {
    
	value = new RbDouble(1.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_dchisq::~RbFunction_dchisq(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_dchisq::clone(void) const {

    RbObject *x = new RbFunction_dchisq( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_dchisq::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *x  = (RbDouble*) arguments[0]->getValue();
    RbInt    *df = (RbDouble*) arguments[1]->getValue();
    value->setValue( RbStatistics::ChiSquare::pdf(*df, *x) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_dchisq::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x" , RbDouble_name, new RbUndefined, new RbDouble(0.0), new RbUndefined ), 
		new ArgumentRule( "df", RbInt_name   , new RbInt(1)   , new RbInt(1)     , new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_dchisq::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dchisq_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_dchisq::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_dchisq::toString(void) const {

    RbDouble *x  = (RbDouble*) arguments[0]->getValue();
    RbInt    *df = (RbDouble*) arguments[1]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << "Chi-Square: f( " << x->getValue() << " | " << df->getValue() << " ) = " << value->getValue();
    return o.str();
}

