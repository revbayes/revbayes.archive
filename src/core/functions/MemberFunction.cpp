/**
 * @file
 * This file contains the implementation of MemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the member functions. Note that the first argument passed
 * in a member function call is a pointer to the MemberNode of the calling
 * object (like a this pointer).
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
#include "ContainerNode.h"
#include "DAGNode.h"
#include "Ellipsis.h"
#include "MemberFunction.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
MemberFunction::MemberFunction(const TypeSpec retType, const ArgumentRules& argRules)
    : RbFunction(), returnType(retType), argumentRules(argRules) {

    if ( argumentRules[0]->getArgTypeSpec() != TypeSpec( MemberObject_name, 0, true ) )
        throw RbException( "First argument rule to a member function must be a reference to the member object" );
    
    if ( argumentRules[0]->hasDefault() )
        throw RbException( "First argument rule to a member function is not allowed to have a default" );
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


/** Execute function: call the object's internal implementation through executeOperation */
DAGNode* MemberFunction::execute( void ) {

    MemberNode* memberNode = static_cast<MemberNode*>( args[0].getReference() );

    return memberNode->getMemberObject()->executeOperation( funcName, args );
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

