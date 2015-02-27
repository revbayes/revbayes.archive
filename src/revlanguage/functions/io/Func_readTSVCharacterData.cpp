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
#include "NaturalNumbersState.h"
#include "RlNaturalNumbersState.h"
#include "DiscreteCharacterData.h"
#include "RlDiscreteCharacterData.h"
#include "WorkspaceVector.h"
#include <boost/lexical_cast.hpp>

#include <map>
#include <set>
#include <sstream>
#include <iostream>


using namespace RevLanguage;

/** Clone object */
Func_readTSVCharacterData* Func_readTSVCharacterData::clone( void ) const {
    
    return new Func_readTSVCharacterData( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readTSVCharacterData::execute( void ) {
	
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
	
	const RlString& dt = static_cast<const RlString&>( args[1].getVariable()->getRevObject() );
	
	if (dt.getValue() == "NaturalNumbers") {

		// setup a matrix of NaturalNumbers states
		RevBayesCore::DiscreteCharacterData<RevBayesCore::NaturalNumbersState> *coreStates = new RevBayesCore::DiscreteCharacterData<RevBayesCore::NaturalNumbersState>();
		
		// get data from file
		RevBayesCore::NaturalNumberTSVDataReader* tsv_data = new RevBayesCore::NaturalNumberTSVDataReader(fn.getValue());
		
		// loop through data and get each NaturalNumbers value
		int i = 0;
		while (i < tsv_data->getData().size()) {
			
			// get count from data
			std::string data = boost::lexical_cast<std::string>(tsv_data->getData()[i]);
			
			// make the core state
			RevBayesCore::NaturalNumbersState coreState = RevBayesCore::NaturalNumbersState( data );
			
			// now put core state into DiscreteTaxonData		
			RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState>(tsv_data->getNames()[i]);
			coreSeq.addCharacter( coreState );
			
			// add DiscreteTaxonData to the matrix of NaturalNumbers states
			coreStates->addTaxonData( coreSeq );
			
			i++;
		}
		
		// put coreNaturalNumbers matrix into rev language level matrix
		DiscreteCharacterData<RevLanguage::NaturalNumbersState> *rlCoreStates = new DiscreteCharacterData<RevLanguage::NaturalNumbersState>( coreStates );
		
		return new RevVariable( rlCoreStates );
		
	} else {
		
		throw RbException( "Invalid data type. Valid data types are: NaturalNumbers" );
	
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
    
    static TypeSpec returnTypeSpec = NaturalNumbersState::getClassTypeSpec();
    return returnTypeSpec;
}




