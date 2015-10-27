#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readAtlas.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TimeAtlas.h"
#include "TimeAtlasDataReader.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readAtlas* Func_readAtlas::clone( void ) const
{
    
    return new Func_readAtlas( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readAtlas::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    RevBayesCore::TimeAtlasDataReader* tadr = new RevBayesCore::TimeAtlasDataReader( fn.getValue() );
    RevBayesCore::TimeAtlas* atlas = new RevBayesCore::TimeAtlas(tadr);

    return new RevVariable( new RlAtlas(atlas) );
}


/** Get argument rules */
const ArgumentRules& Func_readAtlas::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
    
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), "The name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readAtlas::getClassType(void)
{
    
    static std::string revType = "Func_readAtlas";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readAtlas::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readAtlas::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readAtlas::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlAtlas::getClassTypeSpec();
    return returnTypeSpec;
}




