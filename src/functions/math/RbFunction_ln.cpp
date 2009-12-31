/**
 * @file
 * This file contains the implementation of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Implementation of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "RbDouble.h"
#include "RbFunction_ln.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbUndefined.h"
#include <cmath>
#include <string>



/** Default constructor */
RbFunction_ln::RbFunction_ln(void) : RbFunction() {

	value = new RbDouble(0.0);
} 

/** Copy constructor */
RbFunction_ln::RbFunction_ln(const RbFunction_ln& s) : RbFunction(s) {
    
	value = new RbDouble(0.0);
	*value = *s.value;
}

/** Destructor */
RbFunction_ln::~RbFunction_ln(void) {

    delete value;
}

/** Clone */
RbObject* RbFunction_ln::clone(void) const {

    RbObject *x = new RbFunction_ln( *this );
    return x;
}

/** Execute the function */
const RbObject* RbFunction_ln::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbDouble *arg = (RbDouble*) arguments[0]->getValue();
    if ( arg->getValue() < 0.0 )
        value->setValue(1E-100);
    else
        value->setValue( std::log(arg->getValue()) );
    return value;
}

/** Get the argument rules */
const ArgumentRule** RbFunction_ln::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "x", RbNames::Double::name, new RbUndefined, new RbDouble(0.0), new RbUndefined ),
		NULL };
	return argRules;
}

/** Get string showing inheritance */
const StringVector& RbFunction_ln::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_ln_name) + RbFunction::getClass();
	return rbClass;
}

/** Get the return type */
const std::string RbFunction_ln::getReturnType(void) const {

	const static std::string returnType = RbNames::Double::name;
	return returnType;
}

/** Get string showing value */
std::string RbFunction_ln::toString(void) const {

    char v[30];
    sprintf(v, "%1.6lf", value->getValue());
    std::string vStr = v;

    RbDouble *x = (RbDouble*) processedArguments[0]->getValue();
    char arg[30];
    sprintf(arg, "%1.6lf", x->getValue());
    std::string xStr = arg;
    std::string str = "Log_e( " + xStr + " ) = " + vStr;
    return str;
}


