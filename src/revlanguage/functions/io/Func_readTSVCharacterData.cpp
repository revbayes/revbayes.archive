/**
 * @file
 * This file contains the implementation of Func_readTSVCharacterData which
 * reads in a file with tab-separated character data.
 *
 * @brief Implementation of Func_readTSVCharacterData
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */


#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTSVCharacterData.h"
#include "ModelVector.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "NaturalNumberTSVDataReader.h"
#include "ChromosomesState.h"
#include "NaturalNumbersState.h"
#include "RlChromosomesState.h"
#include "RlNaturalNumbersState.h"
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
Func_readTSVCharacterData* Func_readTSVCharacterData::clone( void ) const {
    
    return new Func_readTSVCharacterData( *this );
}


/** Execute function */
RevPtr<Variable> Func_readTSVCharacterData::execute( void ) {
	
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	
	const RlString& dt = static_cast<const RlString&>( args[1].getVariable()->getRevObject() );
	
	if (dt.getValue() == "Chromosomes") {
	
		// setup a matrix of chromosome states
		RevBayesCore::DiscreteCharacterData<RevBayesCore::ChromosomesState> *coreChromoStates = new RevBayesCore::DiscreteCharacterData<RevBayesCore::ChromosomesState>();
		
		// get data from file
		RevBayesCore::NaturalNumberTSVDataReader* tsv_data = new RevBayesCore::NaturalNumberTSVDataReader(fn.getValue());
		
		// loop through data and get each chromosome count
		int i = 0;
		while (i < tsv_data->getData().size()) {
			
			// get count from data
			std::string data = boost::lexical_cast<std::string>(tsv_data->getData()[i]);
			
			// make the core state
			RevBayesCore::ChromosomesState coreState = RevBayesCore::ChromosomesState( data );
			
			// now put core state into DiscreteTaxonData		
			RevBayesCore::DiscreteTaxonData<RevBayesCore::ChromosomesState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::ChromosomesState>(tsv_data->getNames()[i]);
			coreSeq.addCharacter( coreState );

			// add DiscreteTaxonData to the matrix of chromosome states
			coreChromoStates->addTaxonData( coreSeq );
			
			i++;
		}
		
		// put coreChromoStates matrix into rev language level matrix
		DiscreteCharacterData<RevLanguage::ChromosomesState> *chromoStates = new DiscreteCharacterData<RevLanguage::ChromosomesState>( coreChromoStates );

		return new Variable( chromoStates );
	
	} else if (dt.getValue() == "NaturalNumbers") {

		// setup a matrix of chromosome states
		RevBayesCore::DiscreteCharacterData<RevBayesCore::NaturalNumbersState> *coreStates = new RevBayesCore::DiscreteCharacterData<RevBayesCore::NaturalNumbersState>();
		
		// get data from file
		RevBayesCore::NaturalNumberTSVDataReader* tsv_data = new RevBayesCore::NaturalNumberTSVDataReader(fn.getValue());
		
		// loop through data and get each chromosome count
		int i = 0;
		while (i < tsv_data->getData().size()) {
			
			// get count from data
			std::string data = boost::lexical_cast<std::string>(tsv_data->getData()[i]);
			
			// make the core state
			RevBayesCore::NaturalNumbersState coreState = RevBayesCore::NaturalNumbersState( data );
			
			// now put core state into DiscreteTaxonData		
			RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState>(tsv_data->getNames()[i]);
			coreSeq.addCharacter( coreState );
			
			// add DiscreteTaxonData to the matrix of chromosome states
			coreStates->addTaxonData( coreSeq );
			
			i++;
		}
		
		// put coreChromoStates matrix into rev language level matrix
		DiscreteCharacterData<RevLanguage::NaturalNumbersState> *rlCoreStates = new DiscreteCharacterData<RevLanguage::NaturalNumbersState>( coreStates );
		
		return new Variable( rlCoreStates );
		
	} else {
		
		throw RbException( "Invalid data type. Valid data types are: NaturalNumbers, Chromosomes" );
	
	}
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readTSVCharacterData::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
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
const ArgumentRules& Func_readTSVCharacterData::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
		
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
		argumentRules.push_back( new ArgumentRule( "type", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
		
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readTSVCharacterData::getClassType(void) {
    
    static std::string revType = "Func_readTSVCharacterData";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readTSVCharacterData::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readTSVCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readTSVCharacterData::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ChromosomesState::getClassTypeSpec();
    return returnTypeSpec;
}




