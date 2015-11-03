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
MemberProcedure::MemberProcedure(const TypeSpec retType, ArgumentRules* argRules) : Procedure(),
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
RevPtr<RevVariable> MemberProcedure::execute( void )
{
    
    bool found = false;
    RevPtr<RevVariable> retValue = object->getRevObject().executeMethod( getName(), args, found );
    
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


/** Get the help entry for this class */
RevBayesCore::RbHelpFunction* MemberProcedure::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpFunction *help = new RevBayesCore::RbHelpFunction();
    RevBayesCore::RbHelpFunction &helpEntry = *help;
    
    std::string entry = "";
    
    // name
    helpEntry.setName( "name" );
    
    // aliases
    std::vector<std::string> aliases = std::vector<std::string>();
//    aliases.push_back( "alias" );
    helpEntry.setAliases( aliases );
    
    // title
    helpEntry.setTitle( "title" );
    
    // description
    std::vector<std::string> desc = std::vector<std::string>();
    desc.push_back("");
    helpEntry.setDescription( desc );
    
    // usage
    helpEntry.setUsage( "usage" );
    
    // arguments
    std::vector<RevBayesCore::RbHelpArgument> arguments = std::vector<RevBayesCore::RbHelpArgument>();
    RevBayesCore::RbHelpArgument argument = RevBayesCore::RbHelpArgument();
    argument.setLabel(                  "label"                 );
    argument.setDescription(            "description"           );
    argument.setArgumentDagNodeType(    "argumentNodeType"      );
    argument.setArgumentPassingMethod(  "argumentPassingMethod" );
    argument.setValueType(              "valueType"             );
    argument.setDefaultValue(           "defaultValue"          );
        
    // loop options
    std::vector<std::string> options = std::vector<std::string>();
    std::string option = std::string( "o" );
    options.push_back( option );
    argument.setOptions( options );
        
    // add the argument to the argument list
    arguments.push_back( argument );
    
    helpEntry.setArguments( arguments );
    
    // return value
    helpEntry.setReturnType( "returnValue" );
    
    // details
    std::vector<std::string> details = std::vector<std::string>();
    details.push_back( "details" );
    
    helpEntry.setDetails( details );
    
    // example
    helpEntry.setExample( "example" );
    
    // reference
    std::vector<RevBayesCore::RbHelpReference> references = std::vector<RevBayesCore::RbHelpReference>();
    RevBayesCore::RbHelpReference ref = RevBayesCore::RbHelpReference();
    ref.setCitation( "citation" );
    ref.setDoi( "doi" );
    ref.setUrl( "url" );
    references.push_back( ref );
    
    helpEntry.setReferences( references );
    
    // author
    helpEntry.setAuthor( "author" );
    
    // see also
    std::vector<std::string> seeAlso = std::vector<std::string>();
    seeAlso.push_back( "see also" );
    
    helpEntry.setSeeAlso(seeAlso);
    
    return help;

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

