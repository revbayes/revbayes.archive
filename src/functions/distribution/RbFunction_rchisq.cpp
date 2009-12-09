/**
 * @file
 * This file contains the implementation of RbFunction_rchisq, the
 * dnorm() function.
 *
 * @brief Implementation of RbFunction_rchisq
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
#include "RbFunction_rchisq.h"
#include "RbInt.h"
#include "RbObject.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include <iomanip>
#include <sstream>
#include <string>



/** Default constructor */
RbFunction_rchisq::RbFunction_rchisq(void) : RbFunction() {

	value = new RbDouble(1.0);
} 


/** Copy constructor */
RbFunction_rchisq::RbFunction_rchisq(const RbFunction_rchisq& s) : RbFunction(s) {
    
	value = new RbDouble(1.0);
	*value = *s.value;
}


/** Destructor */
RbFunction_rchisq::~RbFunction_rchisq(void) {

    delete value;
}


/** Clone */
RbObject* RbFunction_rchisq::clone(void) const {

    RbObject *x = new RbFunction_rchisq( *this );
    return x;
}


/** Execute the function */
const RbObject* RbFunction_rchisq::executeOperation(const std::vector<DAGNode*>& arguments) const {

    RbInt    *df = (RbDouble*) arguments[0]->getValue();
	RandomNumberFactory& rnf = RandomNumberFactory::randomNumberFactoryInstance();
	RandomNumberGenerator* rng = rnf.getGlobalRandomNumberGenerator();
    value->setValue( RbStatistics::ChiSquare::rv(*df, rng) );
    return value;
}


/** Get the argument rules */
const ArgumentRule** RbFunction_rchisq::getArgumentRules(void) const {

	const static ArgumentRule* argRules[] = { 
		new ArgumentRule( "df", RbInt_name   , new RbInt(1)   , new RbInt(1)     , new RbUndefined ),
		NULL };
	return argRules;
}


/** Get string showing inheritance */
const StringVector& RbFunction_rchisq::getClass(void) const { 

    static StringVector rbClass = StringVector(RbFunction_dchisq_name) + RbFunction::getClass();
	return rbClass;
}


/** Get the return type */
const std::string RbFunction_rchisq::getReturnType(void) const {

	const static std::string returnType = RbDouble_name;
	return returnType;
}


/** Get string showing value */
std::string RbFunction_rchisq::toString(void) const {

    RbInt    *df = (RbDouble*) arguments[0]->getValue();
    std::ostringstream o;
	o << std::fixed << std::setprecision(6);
	o << value->getValue() << " ~ Chi-Square( " << df->getValue() << " )";
    return o.str();
}

