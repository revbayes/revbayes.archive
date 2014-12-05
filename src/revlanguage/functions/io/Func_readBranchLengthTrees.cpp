#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readBranchLengthTrees.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlBranchLengthTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "BranchLengthTree.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readBranchLengthTrees* Func_readBranchLengthTrees::clone( void ) const {
    
    return new Func_readBranchLengthTrees( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readBranchLengthTrees::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader reader = RevBayesCore::NclReader();
    
    ModelVector<BranchLengthTree> *trees = new ModelVector<BranchLengthTree>();
    std::vector<RevBayesCore::BranchLengthTree*> tmp = *(reader.readBranchLengthTrees( fn.getValue() ));
    for (std::vector<RevBayesCore::BranchLengthTree*>::iterator t = tmp.begin(); t != tmp.end(); ++t) 
    {
        trees->push_back( BranchLengthTree(*t) );
    }
    
    return new RevVariable( trees );
}


/** Get argument rules */
const ArgumentRules& Func_readBranchLengthTrees::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readBranchLengthTrees::getClassType(void) { 
    
    static std::string revType = "Func_readBranchLengthTrees";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readBranchLengthTrees::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_readBranchLengthTrees::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readBranchLengthTrees::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<BranchLengthTree>::getClassTypeSpec();
    return returnTypeSpec;
}




