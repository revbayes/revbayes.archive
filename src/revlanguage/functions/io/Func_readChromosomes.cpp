/*
 *  Func_readChromosomes.cpp
 *  revbayes-proj
 *
 *  Created by Will Freyman on 9/8/14.
 *
 */


#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readChromosomes.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "ChromosomesDataReader.h"
#include "ChromosomesState.h"
#include "RlChromosomesState.h"
#include "DiscreteCharacterData.h"
#include "RlDiscreteCharacterData.h"
#include "WorkspaceVector.h"
#include <boost/lexical_cast.hpp>

#include <map>
#include <set>
#include <sstream>
#include<iostream>


using namespace RevLanguage;

/** Clone object */
Func_readChromosomes* Func_readChromosomes::clone( void ) const {
    
    return new Func_readChromosomes( *this );
}


/** Execute function */
RevPtr<Variable> Func_readChromosomes::execute( void ) {
	
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	
	// setup the vector of chromosome states;
	//WorkspaceVector<AbstractCharacterData> *chromoStates = new WorkspaceVector<AbstractCharacterData>();
	WorkspaceVector<RevLanguage::ChromosomesState> *chromoStates = new WorkspaceVector<RevLanguage::ChromosomesState>();
	
	// get data from file
    RevBayesCore::ChromosomesDataReader* data = new RevBayesCore::ChromosomesDataReader(fn.getValue());
	
	// loop through data and get each chromosome count
	int i = 0;
	while (i < data->getCounts().size()) {
		
		// get count from data
		std::string count = boost::lexical_cast<std::string>(data->getCounts()[i]);
		
		// make the core state
		RevBayesCore::ChromosomesState *coreState = new RevBayesCore::ChromosomesState( count );
		
		// now put core state into RL state object
		//DiscreteCharacterData<RevLanguage::ChromosomesState>* thisState = new RevBayesCore::DiscreteCharacterData<DiscreteCharacterData<RevLanguage::ChromosomesState>( *coreState );
		//AbstractCharacterData<RevLanguage::ChromosomesState>* thisState = new AbstractCharacterData<RevLanguage::ChromosomesState>( *coreState );
		RevLanguage::ChromosomesState* thisState = new RevLanguage::ChromosomesState( *coreState );
		
		// add chromosome state to matrix
		//chromoStates->push_back( *thisState );	
		i++;
	}
    return new Variable( chromoStates );
}
 


//RevBayesCore::DiscreteCharacterData<RevBayesCore::DnaState> *coreM = static_cast<RevBayesCore::DiscreteCharacterData<RevBayesCore::DnaState> *>( *it );
//DiscreteCharacterData<DnaState>* mDNA = new DiscreteCharacterData<DnaState>( coreM );
//m->push_back( mDNA );


/** Format the error exception string for problems specifying the file/path name */
void Func_readChromosomes::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
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
const ArgumentRules& Func_readChromosomes::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
		
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
		
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readChromosomes::getClassType(void) {
    
    static std::string revType = "Func_readChromosomes";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readChromosomes::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readChromosomes::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readChromosomes::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ChromosomesState::getClassTypeSpec();
    return returnTypeSpec;
}




