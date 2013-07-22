/**
 * @file
 * This file contains the implementation of Func_readAlignment.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-22 11:45:25 +0200 (Wed, 22 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_readCharacterData.cpp 1765 2012-08-22 09:45:25Z hoehna $
 */

#include "ArgumentRule.h"
#include "CharacterData.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readCharacterData.h"
#include "NclReader.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbNullObject.h"
#include "RlBoolean.h"
#include "RlCharacterData.h"
#include "RlAminoAcidState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlStandardState.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"
#include "VectorRlPointer.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readCharacterData* Func_readCharacterData::clone( void ) const {
    
    return new Func_readCharacterData( *this );
}


/** Execute function */
RbLanguageObject* Func_readCharacterData::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString& fn = static_cast<const RlString&>( args[0].getVariable()->getValue() );
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn.getValue() );
    if ( myFileManager.getFileName() == "" && myFileManager.getFilePath() == "" )
    {
        std::string errorStr = "";
        formatError(myFileManager, errorStr);
        throw RbException("Could not find file or path with name \"" + fn.getValue() + "\"");
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = false;
    if ( myFileManager.getFilePath() != "" && myFileManager.getFileName() == "")
        readingDirectory = true;
    if (readingDirectory == true)
        RBOUT("Recursively reading the contents of a directory");
    else
        RBOUT("Attempting to read the contents of file \"" + myFileManager.getFileName() + "\"");
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true)
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    else 
    {
#       if defined (WIN32)
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "\\" + myFileManager.getFileName() );
#       else
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
#       endif
    }
    if (readingDirectory == true)
    {
        std::stringstream o1;
        o1 << "Found " << vectorOfFileNames.size() << " files in directory";
        RBOUT(o1.str());
    }
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    RevBayesCore::NclReader& reader = RevBayesCore::NclReader::getInstance();
    reader.clearWarnings();
    
    // the return value
    VectorRlPointer<RbLanguageObject> *m = new VectorRlPointer<RbLanguageObject>();
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool isInterleaved = false;
        std::string myFileType = "unknown", dType = "unknown";
        if (reader.isNexusFile(*p, dType) == true)
            myFileType = "nexus";
        else if (reader.isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (reader.isFastaFile(*p, dType) == true)
            myFileType = "fasta";
        
        if (myFileType != "unknown") {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" ) {
                if (isInterleaved == true)
                    suffix += "|interleaved";
                else
                    suffix += "|noninterleaved";
            }
            else if ( myFileType == "fasta" )
                suffix += "|noninterleaved";
            else
                suffix += "|unknown";
            myFileType += suffix;
            
            // read the content of the file now
            std::vector<RevBayesCore::AbstractCharacterData*> m_i = reader.readMatrices( *p, myFileType );
            for (std::vector<RevBayesCore::AbstractCharacterData*>::iterator it = m_i.begin(); it != m_i.end(); it++) {

                if ( dType == "dna" ) {
                    RevBayesCore::CharacterData<RevBayesCore::DnaState> *coreM = static_cast<RevBayesCore::CharacterData<RevBayesCore::DnaState> *>( *it );
                    CharacterData<DnaState> *mDNA = new CharacterData<DnaState>( coreM );
                    m->push_back( mDNA );
                }
                else if ( dType == "rna" ) {
                    RevBayesCore::CharacterData<RevBayesCore::RnaState> *coreM = static_cast<RevBayesCore::CharacterData<RevBayesCore::RnaState> *>( *it );
                    CharacterData<RnaState> *mRNA = new CharacterData<RnaState>( coreM );
                    m->push_back( mRNA );
                }
                else if ( dType == "protein" ) {
                    RevBayesCore::CharacterData<RevBayesCore::AminoAcidState> *coreM = static_cast<RevBayesCore::CharacterData<RevBayesCore::AminoAcidState> *>( *it );
                    CharacterData<AminoAcidState> *mAA = new CharacterData<AminoAcidState>( coreM );
                    m->push_back( mAA );
                }
                else if ( dType == "standard" ) {
                    RevBayesCore::CharacterData<RevBayesCore::StandardState> *coreM = static_cast<RevBayesCore::CharacterData<RevBayesCore::StandardState> *>( *it );
                    CharacterData<StandardState> *mSS = new CharacterData<StandardState>( coreM );
                    m->push_back( mSS );
                }
                else {
                    throw RbException("Unknown data type type \"" + dType + "\".");
                }
            }
        }
        else {
            reader.addWarning("Unknown file type");
        }
        
    }
    
    
    // print summary of results of file reading to the user
    if (readingDirectory == true)
    {
        std::stringstream o2;
        if ( m->size() == 0 )
            o2 << "Failed to read any files";
        else if ( m->size() == 1 )
            o2 << "Successfully read one file";
        else
            o2 << "Successfully read " << m->size() << " files";
        RBOUT(o2.str());
        std::set<std::string> myWarnings = reader.getWarnings();
        if ( vectorOfFileNames.size() - m->size() > 0 && myWarnings.size() > 0 )
        {
            std::stringstream o3;
            if (vectorOfFileNames.size() - m->size() == 1)
                o3 << "Did not read a file for the following ";
            else
                o3 << "Did not read " << vectorOfFileNames.size() - m->size() << " files for the following ";
            if (myWarnings.size() == 1)
                o3 << "reason:";
            else
                o3 << "reasons:";
            RBOUT(o3.str());
            for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                RBOUT("* "+(*it));
        }
    }
    else
    {
        if (m->size() > 0)
            RBOUT("Successfully read file");
        else
        {
            std::set<std::string> myWarnings = reader.getWarnings();
            if ( myWarnings.size() > 0 )
            {
                std::stringstream o3;
                o3 << "Did not read the file for the following ";
                if (myWarnings.size() == 1)
                    o3 << "reason:";
                else
                    o3 << "reasons:";
                RBOUT(o3.str());
                for (std::set<std::string>::iterator it = myWarnings.begin(); it != myWarnings.end(); it++)
                    RBOUT("* "+(*it));
            }
        }
    }
    
    return m;
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readCharacterData::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
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
const ArgumentRules& Func_readCharacterData::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", true, RlString::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_readCharacterData::getClassName(void) { 
    
    static std::string rbClassName = "Read alignment function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readCharacterData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_readCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readCharacterData::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = VectorRlPointer<RbLanguageObject>::getClassTypeSpec();
    return returnTypeSpec;
}




