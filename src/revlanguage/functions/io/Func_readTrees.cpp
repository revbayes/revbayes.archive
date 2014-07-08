#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTrees.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TimeTree.h"
#include "RlUserInterface.h"
#include "Vector.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readTrees* Func_readTrees::clone( void ) const {
    
    return new Func_readTrees( *this );
}


/** Execute function */
RevPtr<Variable> Func_readTrees::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader& reader = RevBayesCore::NclReader::getInstance();
    reader.clearWarnings();
    
    Vector<TimeTree> *trees = new Vector<TimeTree>();
    std::vector<RevBayesCore::TimeTree*> tmp = reader.readTimeTrees( fn.getValue() );
    for (std::vector<RevBayesCore::TimeTree*>::iterator t = tmp.begin(); t != tmp.end(); ++t) 
    {
        trees->push_back( TimeTree(*t) );
    }
    
    return new Variable( trees );
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readTrees::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
    bool fileNameProvided    = fm.isFileNamePresent();
    bool isFileNameGood      = fm.testFile();
    bool isDirectoryNameGood = fm.testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + fm.getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false)) {
        errorStr += "Could not read file named \"" + fm.getFileName() + "\" in directory named \"" + fm.getFilePath() + "\" ";
        if (isFileNameGood == false && isDirectoryNameGood == true)
            errorStr += "because the file does not exist";
        else if (isFileNameGood == true && isDirectoryNameGood == false)
            errorStr += "because the directory does not exist";
        else
            errorStr += "because neither the directory nor the file exist";
    }
}


/** Get argument rules */
const ArgumentRules& Func_readTrees::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", true, RlString::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_readTrees::getClassName(void) { 
    
    static std::string rbClassName = "Func_readTrees";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readTrees::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_readTrees::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readTrees::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Vector<TimeTree>::getClassTypeSpec();
    return returnTypeSpec;
}




