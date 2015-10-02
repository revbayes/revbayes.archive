#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTrees.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "Tree.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readTrees* Func_readTrees::clone( void ) const
{
    
    return new Func_readTrees( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readTrees::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader reader = RevBayesCore::NclReader();
    
    ModelVector<TimeTree> *trees = new ModelVector<TimeTree>();
    std::vector<RevBayesCore::Tree*> tmp = reader.readTimeTrees( fn.getValue() );
    for (std::vector<RevBayesCore::Tree*>::iterator t = tmp.begin(); t != tmp.end(); ++t) 
    {
        trees->push_back( TimeTree(*t) );
    }
    
    return new RevVariable( trees );
}


/** Get argument rules */
const ArgumentRules& Func_readTrees::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readTrees::getClassType(void)
{
    
    static std::string revType = "Func_readTrees";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readTrees::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_readTrees::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readTrees::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = ModelVector<TimeTree>::getClassTypeSpec();
    return returnTypeSpec;
}




