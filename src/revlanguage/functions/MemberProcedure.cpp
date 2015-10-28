#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "MemberProcedure.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
MemberProcedure::MemberProcedure(const std::string &n, const TypeSpec retType, ArgumentRules* argRules) : Procedure(),
    argumentRules( argRules ),
    proc_name( n ),
    object( NULL ),
    returnType( retType )
{
    
}


/** Clone the object */
MemberProcedure* MemberProcedure::clone(void) const
{
    
    return new MemberProcedure(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
RevPtr<RevVariable> MemberProcedure::execute( void )
{
    
    bool found = false;
    RevPtr<RevVariable> retValue = object->getRevObject().executeMethod( getFunctionName(), args, found );
    
    if ( found == false )
    {
        throw RbException("Couldn't find member procedure called '" + getFunctionName() + "'");
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
const std::string& MemberProcedure::getClassType(void)
{
    
    static std::string revClassType = "MemberProcedure";
    
	return revClassType;
}

/** Get class type spec describing type of object */
const TypeSpec& MemberProcedure::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revClassTypeSpec;
}


/** Get type spec */
const TypeSpec& MemberProcedure::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& MemberProcedure::getArgumentRules(void) const
{
    
    return *argumentRules;
}


/**
 * Get the name for this procedure.
 */
const std::string& MemberProcedure::getFunctionName( void ) const
{
    
    return proc_name;
}


/**
 * Get the name for this procedure.
 */
const std::vector<std::string>& MemberProcedure::getFunctionNameAliases( void ) const
{
    static std::vector<std::string> aliases;
    return aliases;
}


/** Get return type */
const TypeSpec& MemberProcedure::getReturnType(void) const
{
    
    return returnType;
}


/** This is a procedure, so return true for this function call */
bool MemberProcedure::isProcedure( void ) const
{
    return true;
}


void MemberProcedure::setMemberObject( const RevPtr<RevVariable> &obj)
{
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
}

