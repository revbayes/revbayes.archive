/**
 * @file
 * This file contains the implementation of MemberFunction, which is used
 * for member functions (methods) of complex objects with methods.
 *
 * @brief Implementation of MemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodeContainer.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "MemberFunction.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Constructor */
MemberFunction::MemberFunction(const std::string& retType, const ArgumentRules& argRules)
    : RbFunction(), returnType(retType), argumentRules(argRules) {
}


/** Brief info on the function */
std::string MemberFunction::briefInfo(void) const {

    std::ostringstream o;
    o << "MemberFunction: " << (*this);

    return o.str();
}


/** Clone the object */
MemberFunction* MemberFunction::clone(void) const {

    return new MemberFunction(*this);
}


/** Pointer-based equals comparison */
bool MemberFunction::equals(const RbObject* x) const {

    // TODO: Implement properly
    return false;
}


/** Execute operation: we rely on the object's own implementation, so we do nothing here */
RbObject* MemberFunction::executeOperation(const std::vector<DAGNode*>& args) {

    return NULL;
}


/** Get class vector describing type of object */
const StringVector& MemberFunction::getClass(void) const { 

    static StringVector rbClass = StringVector(MemberFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get argument rules */
const ArgumentRules& MemberFunction::getArgumentRules(void) const {

    return argumentRules;
}


/** Get return type */
const std::string& MemberFunction::getReturnType(void) const {

    return returnType;
}

/** Complete info about object */
std::string MemberFunction::toString(void) const {

    std::ostringstream o;
    o << "MemberFunction: " << (*this) << std::endl;
    
    if (argumentsProcessed)
        o << "Arguments processed; there are " << processedArguments.size() << " values." << std::endl;
    else
        o << "Arguments not processed; there are " << processedArguments.size() << " values." << std::endl;
    
    int index=1;
    for (std::vector<DAGNode*>::const_iterator i=processedArguments.begin(); i!=processedArguments.end(); i++, index++) {
        o << " processedArguments[" << index << "] = " << (*i)->getValue() << std::endl;
    }

    return o.str();
}

