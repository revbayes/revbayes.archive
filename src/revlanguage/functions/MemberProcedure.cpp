/**
 * @file
 * This file contains the implementation of SimpleMemberProcedure, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular Function objects
 * implementing the member functions.
 *
 * @brief Implementation of SimpleMemberProcedure
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: SimpleMemberProcedure.cpp 1544 2012-05-15 16:59:11Z hoehna $
 */

#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "MemberProcedure.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
MemberProcedure::MemberProcedure(const TypeSpec retType, ArgumentRules* argRules) : Function(),
    argumentRules(argRules),
    object(NULL),
    returnType(retType)
{
    
}


/** Clone the object */
MemberProcedure* MemberProcedure::clone(void) const
{
    
    return new MemberProcedure(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
RevPtr<Variable> MemberProcedure::execute( void )
{
    
    bool found = false;
    RevPtr<Variable> retValue = object->getRevObject().executeMethod( getName(), args, found );
    
    if ( found == false )
    {
        throw RbException("Couldn't find member procedure called '" + getName() + "'");
    }
    
    try
    {
        RevBayesCore::DagNode* theNode = object->getRevObject().getDagNode();
        
        if ( theNode != NULL )
        {
            theNode->touch();
        }
    }
    catch (RbException e)
    {
        // we do nothing ...
    }
    
    return retValue;
    
}


/** Get class name of object */
const std::string& MemberProcedure::getClassType(void) {
    
    static std::string revClassType = "MemberProcedure";
    
	return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& MemberProcedure::getClassTypeSpec(void) {
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}

/** Get type spec */
const TypeSpec& MemberProcedure::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& MemberProcedure::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get return type */
const TypeSpec& MemberProcedure::getReturnType(void) const {
    
    return returnType;
}


/** This is a procedure, so return true for this function call */
bool MemberProcedure::isProcedure( void ) const
{
    return true;
}


void MemberProcedure::setMemberObject( const RevPtr<Variable> &obj) {
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
}

