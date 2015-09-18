#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ContinuousCharacterData.h"
#include "Func_readContinuousCharacterData.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlAminoAcidState.h"
#include "RlContinuousCharacterData.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readContinuousCharacterData* Func_readContinuousCharacterData::clone( void ) const {
    
    return new Func_readContinuousCharacterData( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readContinuousCharacterData::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    bool returnAsVector = static_cast<const RlBoolean&>( args[1].getVariable()->getRevObject() ).getValue();
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn.getValue() );
    if ( !myFileManager.testFile() && !myFileManager.testDirectory() )
    {
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException("Could not find file or path with name \"" + fn.getValue() + "\"");
    }
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if ( myFileManager.isDirectory() )
    {
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    }
    else
    {
        vectorOfFileNames.push_back( myFileManager.getFullFileName() );
    }
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader reader = RevBayesCore::NclReader();
    
    // the vector of matrices;
    ModelVector<ContinuousCharacterData> *m = new ModelVector<ContinuousCharacterData>();
    
    // the return value
    RevObject* retVal = NULL;
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    size_t numFilesRead = 0;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        bool isInterleaved = false;
        std::string myFileType = "unknown";
        std::string dType = "unknown";
        if (reader.isNexusFile(*p) == true)
        {
            myFileType = "nexus";
        }
        else if (reader.isPhylipFile(*p, dType, isInterleaved) == true)
        {
            myFileType = "phylip";
        }
        else if (reader.isFastaFile(*p, dType) == true)
        {
            myFileType = "fasta";
        }
        
        int numMatricesReadForThisFile=0;
        if (myFileType != "unknown")
        {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" )
            {
                if (isInterleaved == true)
                {
                    suffix += "|interleaved";
                }
                else
                {
                    suffix += "|noninterleaved";
                }
            }
            else if ( myFileType == "fasta" )
            {
                suffix += "|noninterleaved";
            }
            else
            {
                suffix += "|unknown";
            }
            myFileType += suffix;
            
            // read the content of the file now
            std::vector<RevBayesCore::AbstractCharacterData*> m_i = reader.readMatrices( *p, myFileType );
            for (std::vector<RevBayesCore::AbstractCharacterData*>::iterator it = m_i.begin(); it != m_i.end(); it++)
            {
                
                dType = (*it)->getDataType();
                
                // Assume success; correct below if failure
                numMatricesReadForThisFile++;
                
                if ( dType == "Continuous" )
                {
                    RevBayesCore::ContinuousCharacterData *coreM = static_cast<RevBayesCore::ContinuousCharacterData *>( *it );
                    ContinuousCharacterData mCC = ContinuousCharacterData (coreM );
                    m->push_back( mCC );
                }
                else
                {
                    numMatricesReadForThisFile--;
                    throw RbException("Unknown data type \"" + dType + "\".");
                }
            }
        }
        else
        {
            reader.addWarning("Unknown file type");
        }
        
        if (numMatricesReadForThisFile > 0)
        {
            numFilesRead++;
        }
    }
    
    
    // print summary of results of file reading to the user
    if (myFileManager.isDirectory() == true)
    {
        std::stringstream o2;
        if ( numFilesRead == 0 )
        {
            o2 << "Failed to read any files from directory '" << fn.getValue() << "'";
        }
        else if ( numFilesRead == 1 )
        {
            if ( m->size() == 1 )
            {
                o2 << "Successfully read one file with one character matrix from directory '" << fn.getValue() << "'";
            }
            else
            {
                o2 << "Successfully read one file with " << m->size() << " character matrices from directory '" << fn.getValue() << "'";
            }
        }
        else
        {
            o2 << "Successfully read " << numFilesRead << " files with " << m->size() << " character matrices from directory '" << fn.getValue() << "'";
        }
        RBOUT(o2.str());
        std::set<std::string> myWarnings = reader.getWarnings();
        if ( vectorOfFileNames.size() - numFilesRead > 0 && myWarnings.size() > 0 )
        {
            std::stringstream o3;
            if (vectorOfFileNames.size() - numFilesRead == 1)
            {
                o3 << "Did not read a file for the following ";
            }
            else
            {
                o3 << "Did not read " << vectorOfFileNames.size() - numFilesRead << " files for the following ";
            }
            
            if (myWarnings.size() == 1)
            {
                o3 << "reason:";
            }
            else
            {
                o3 << "reasons:";
            }
            RBOUT(o3.str());
            for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
            {
                RBOUT("* "+(*it));
            }
            
        }
    }
    else
    {
        if (m->size() == 1)
        {
            RBOUT("Successfully read one character matrix from file '" + fn.getValue() + "'");
            
            // set the return value
            if ( returnAsVector == false )
            {
                retVal = new ContinuousCharacterData( (*m)[0] );
                delete m;
            }
            else
            {
                retVal = m;
            }
        }
        else if (m->size() > 1)
        {
            std::stringstream o3;
            o3 << "Successfully read " << m->size() << " character matrices from file '" << fn.getValue() << "'";
            RBOUT(o3.str());
            
            // set the return value
            retVal = m;
        }
        else
        {
            std::set<std::string> myWarnings = reader.getWarnings();
            if ( myWarnings.size() > 0 )
            {
                std::stringstream o3;
                o3 << "Error reading file '" << fn.getValue() << "'";
                RBOUT(o3.str());
                for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                {
                    RBOUT("Error:   " + (*it));
                }
            }
        }
    }
    
    return new RevVariable( retVal );
}


/** Get argument rules */
const ArgumentRules& Func_readContinuousCharacterData::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "file", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new ArgumentRule( "alwaysReturnAsVector", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readContinuousCharacterData::getClassType(void) {
    
    static std::string revType = "Func_readContinuousCharacterData";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readContinuousCharacterData::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_readContinuousCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readContinuousCharacterData::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = ModelVector<ContinuousCharacterData>::getClassTypeSpec();
    return returnTypeSpec;
}

