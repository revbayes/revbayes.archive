#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DelimitedCharacterDataReader.h"
#include "HomologousDiscreteCharacterData.h"
#include "Ellipsis.h"
#include "Func_readCharacterDataDelimited.h"
#include "NaturalNumbersState.h"
#include "RbException.h"
#include "RlString.h"
#include "StringUtilities.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlHomologousDiscreteCharacterData.h"
#include "RlNaturalNumbersState.h"
#include "RlStandardState.h"
#include "StandardState.h"



using namespace RevLanguage;

/** Clone object */
Func_readCharacterDataDelimited* Func_readCharacterDataDelimited::clone( void ) const
{
    
    return new Func_readCharacterDataDelimited( *this );
}


std::string Func_readCharacterDataDelimited::bitToState(const std::string &s)
{
    
    std::stringstream ss;
    char* ptr;
    long parsed = strtol(s.c_str(), &ptr, 2);
    
    if (parsed > RbConstants::Integer::max)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " too large to store as NaturalNumber");
    }
    
    if (s.find_first_not_of("01") != std::string::npos)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " contains non-binary characters");
    }
    
    ss << parsed;
    
    return ss.str();
}


/** Execute function */
RevPtr<RevVariable> Func_readCharacterDataDelimited::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string& fn  = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string& dt  = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string& del = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    
    if (dt == "NaturalNumbers")
    {
        
        // setup a matrix of NaturalNumbers states
        RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::NaturalNumbersState> *coreStates = new RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::NaturalNumbersState>();
        
        // get data from file
        RevBayesCore::DelimitedCharacterDataReader* tsv_data = new RevBayesCore::DelimitedCharacterDataReader(fn, del[0]);
        
        // loop through data and get each NaturalNumbers value
        for (size_t i = 0; i < tsv_data->getData().size(); ++i)
        {
            
            // now put core state into DiscreteTaxonData
            RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState>(tsv_data->getNames()[i]);
            
            // get count from data
            const std::vector<std::string> &data = tsv_data->getData()[i];
            
            for (size_t j= 0; j < data.size(); ++j)
            {
                // make the core state
                RevBayesCore::NaturalNumbersState coreState = RevBayesCore::NaturalNumbersState( data[j] );
                
                coreSeq.addCharacter( coreState );
            }
            
            // add DiscreteTaxonData to the matrix of NaturalNumbers states
            coreStates->addTaxonData( coreSeq );
            
        }
        
        // put coreNaturalNumbers matrix into rev language level matrix
        //        HomologousDiscreteCharacterData<RevLanguage::NaturalNumbersState> *rlCoreStates = new HomologousDiscreteCharacterData<RevLanguage::NaturalNumbersState>( coreStates );
        AbstractHomologousDiscreteCharacterData *rlCoreStates = new AbstractHomologousDiscreteCharacterData( coreStates );
        
        return new RevVariable( rlCoreStates );
        
    }
    else if (dt == "Bitset")
    {
        
        // setup a matrix of NaturalNumbers states
        RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::NaturalNumbersState> *coreStates = new RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::NaturalNumbersState>();
        
        // get data from file
        RevBayesCore::DelimitedCharacterDataReader* tsv_data = new RevBayesCore::DelimitedCharacterDataReader(fn, del[0]);
        
        // loop through data and get each NaturalNumbers value
        for (size_t i = 0; i < tsv_data->getData().size(); ++i)
        {
            
            // now put core state into DiscreteTaxonData
            RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::NaturalNumbersState>(tsv_data->getNames()[i]);
            
            // get count from data
            const std::vector<std::string> &data = tsv_data->getData()[i];
            
            for (size_t j= 0; j < data.size(); ++j)
            {
                // make the core state
                std::string d = data[j];
                std::reverse(d.begin(), d.end());
                d = bitToState( d );
                RevBayesCore::NaturalNumbersState coreState = RevBayesCore::NaturalNumbersState( d );
                
                coreSeq.addCharacter( coreState );
            }
            
            // add DiscreteTaxonData to the matrix of NaturalNumbers states
            coreStates->addTaxonData( coreSeq );
            
        }
        
        // put coreNaturalNumbers matrix into rev language level matrix
        //        HomologousDiscreteCharacterData<RevLanguage::NaturalNumbersState> *rlCoreStates = new HomologousDiscreteCharacterData<RevLanguage::NaturalNumbersState>( coreStates );
        AbstractHomologousDiscreteCharacterData *rlCoreStates = new AbstractHomologousDiscreteCharacterData( coreStates );
        
        return new RevVariable( rlCoreStates );
        
    }
    else if (dt == "Standard")
    {
        
        // setup a matrix of NaturalNumbers states
        RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::StandardState> *coreStates = new RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::StandardState>();
        
        // get data from file
        RevBayesCore::DelimitedCharacterDataReader* tsv_data = new RevBayesCore::DelimitedCharacterDataReader(fn, del[0]);
        
        // loop through data and get each NaturalNumbers value
        for (size_t i = 0; i < tsv_data->getData().size(); ++i)
        {
            
            // now put core state into DiscreteTaxonData
            RevBayesCore::DiscreteTaxonData<RevBayesCore::StandardState> coreSeq = RevBayesCore::DiscreteTaxonData<RevBayesCore::StandardState>(tsv_data->getNames()[i]);
            
            // get count from data
            const std::vector<std::string> &data = tsv_data->getData()[i];
            
            for (size_t j= 0; j < data.size(); ++j)
            {
                // make the core state
                RevBayesCore::StandardState coreState = RevBayesCore::StandardState( data[j] );
                
                coreSeq.addCharacter( coreState );
            }
            
            // add DiscreteTaxonData to the matrix of NaturalNumbers states
            coreStates->addTaxonData( coreSeq );
            
        }
        
        // put coreNaturalNumbers matrix into rev language level matrix
        //        HomologousDiscreteCharacterData<RevLanguage::StandardState> *rlCoreStates = new HomologousDiscreteCharacterData<RevLanguage::StandardState>( coreStates );
        AbstractHomologousDiscreteCharacterData *rlCoreStates = new AbstractHomologousDiscreteCharacterData( coreStates );
        
        return new RevVariable( rlCoreStates );
        
    }
    else if (dt == "Continuous")
    {
        
        // setup a matrix of NaturalNumbers states
        RevBayesCore::ContinuousCharacterData *coreStates = new RevBayesCore::ContinuousCharacterData();
        
        // get data from file
        RevBayesCore::DelimitedCharacterDataReader* tsv_data = new RevBayesCore::DelimitedCharacterDataReader(fn, del[0], 1);
        
        // loop through data and get each NaturalNumbers value
        for (size_t i = 0; i < tsv_data->getData().size(); ++i)
        {
            
            // now put core state into DiscreteTaxonData
            RevBayesCore::ContinuousTaxonData coreSeq = RevBayesCore::ContinuousTaxonData(tsv_data->getNames()[i]);
            
            // get count from data
            const std::vector<std::string> &data = tsv_data->getData()[i];
            
            for (size_t j= 0; j < data.size(); ++j)
            {
                // make the core state
                double coreState = atof( data[j].c_str() );
                
                coreSeq.addCharacter( coreState );
            }
            
            // add DiscreteTaxonData to the matrix of NaturalNumbers states
            coreStates->addTaxonData( coreSeq );
            
        }
        
        // put coreNaturalNumbers matrix into rev language level matrix
        ContinuousCharacterData *rlCoreStates = new ContinuousCharacterData( coreStates );
        
        return new RevVariable( rlCoreStates );
        
    }
    else
    {
        
        throw RbException( "Invalid data type. Valid data types are: NaturalNumbers" );
        
    }
}


/** Get argument rules */
const ArgumentRules& Func_readCharacterDataDelimited::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "file",      RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "type",      RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString( "\t" ) ) );
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readCharacterDataDelimited::getClassType(void) {
    
    static std::string revType = "Func_readCharacterDataDelimited";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readCharacterDataDelimited::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readCharacterDataDelimited::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readCharacterDataDelimited::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = NaturalNumbersState::getClassTypeSpec();
    return returnTypeSpec;
}




