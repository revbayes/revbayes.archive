/**
 * @file
 * This file contains the implementation of MemberFunction, which is used
 * for member functions (methods) of complex objects with methods.
 *
 * @brief Implementation of MemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "ContainerNode.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "MemberFunction.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
MemberFunction::MemberFunction(const TypeSpec retType, const ArgumentRules& argRules)
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


/** Execute operation: we rely on the object's own implementation, so we just throw an error here */
DAGNode* MemberFunction::executeFunction( void ) {

    throw RbException( "Unexpected call to void execute function in MemberFunction" );
}


/** Get class vector describing type of object */
const VectorString& MemberFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(MemberFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get argument rules */
const ArgumentRules& MemberFunction::getArgumentRules(void) const {

    return argumentRules;
}


/** Get return type */
const TypeSpec MemberFunction::getReturnType(void) const {

    return returnType;
}


/** Complete info about object */
std::string MemberFunction::richInfo(void) const {

    std::ostringstream o;
    o << "MemberFunction: " << (*this) << std::endl;
    
    if ( argsProcessed )
        o << "Arguments processed; there are " << args.size() << " arguments." << std::endl;
    else
        o << "Arguments not processed" << std::endl;
    
    for( size_t i = 0; i < args.size(); i++ )
        o << " args[" << i << "] = " << args[i].getValue() << std::endl;

    return o.str();
}

