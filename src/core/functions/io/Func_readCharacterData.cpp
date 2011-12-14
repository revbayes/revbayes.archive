/**
 * @file
 * This file contains the implementation of Func_readAlignment.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "CharacterData.h"
#include "ConstantNode.h"
#include "Func_readCharacterData.h"
#include "DagNodeContainer.h"
#include "NclReader.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "RbString.h"
#include "StringUtilities.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <map>
#include <set>
#include <sstream>



// Definition of the static type spec member
const TypeSpec Func_readCharacterData::typeSpec(Func_readAlignment_name);
const TypeSpec Func_readCharacterData::returnTypeSpec(Vector_name, RbPtr<TypeSpec>(new TypeSpec(CharacterData_name) ) );

/** Clone object */
Func_readCharacterData* Func_readCharacterData::clone( void ) const {
    
    return new Func_readCharacterData( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_readCharacterData::executeFunction( void ) {

    // get the information from the arguments for reading the file
    RbPtr<RbString> fn( static_cast<RbString*>( (RbObject*)(*args)[0]->getValue() ) );

    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn->getValue() );
    if ( myFileManager.getFileName() == "" && myFileManager.getFilePath() == "" )
        {
        std::string errorStr = "";
        formatError(myFileManager, errorStr);
        throw RbException("Could not find file or path with name \"" + fn->getValue() + "\"");
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
    NclReader& reader = NclReader::getInstance();
    reader.clearWarnings();
            
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
        {
        bool isInterleaved = false;
        std::string myFileType = "unknown", dType = "unknown";
        if (reader.isNexusFile(*p) == true)
            myFileType = "nexus";
        else if (reader.isPhylipFile(*p, dType, isInterleaved) == true)
            myFileType = "phylip";
        else if (reader.isFastaFile(*p, dType) == true)
            myFileType = "fasta";
            
        if (myFileType != "unknown")
            {
            std::string suffix = "|" + dType;
            if ( myFileType == "phylip" )
                {
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
            fileMap.insert( std::make_pair(*p,myFileType) );
            }
        else
            {
            reader.addWarning("Unknown file type");
            }
        }
#   if 0
    std::cout << "File map (" << fileMap.size() << ")" << std::endl;
    for (std::map<std::string,std::string>::iterator it = fileMap.begin(); it != fileMap.end(); it++)
        std::cout << it->first << " -- " << it->second << std::endl;
#   endif
                
    // read the files in the map containing the file names with the output being a vector of pointers to
    // the character matrices that have been read
    std::vector<RbPtr<CharacterData> > m = reader.readMatrices( fileMap );
    
    // print summary of results of file reading to the user
    if (readingDirectory == true)
        {
        std::stringstream o2;
        if ( m.size() == 0 )
            o2 << "Failed to read any files";
        else if ( m.size() == 1 )
            o2 << "Successfully read one file";
        else
            o2 << "Successfully read " << m.size() << " files";
        RBOUT(o2.str());
        std::set<std::string> myWarnings = reader.getWarnings();
        if ( vectorOfFileNames.size() - m.size() > 0 && myWarnings.size() > 0 )
            {
            std::stringstream o3;
            if (vectorOfFileNames.size() - m.size() == 1)
                o3 << "Did not read a file for the following ";
            else
                o3 << "Did not read " << vectorOfFileNames.size() - m.size() << " files for the following ";
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
        if (m.size() > 0)
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
    
    // return either a list of character matrices or a single character matrix wrapped up in a DAG node
    if ( m.size() > 1 )
        {
        RbPtr<DagNodeContainer> retList( new DagNodeContainer(m.size()) );
        size_t index = 0;
        for (std::vector<RbPtr<CharacterData> >::iterator it = m.begin(); it != m.end(); it++)
            {
            std::string eName = "Data from file \"" + StringUtilities::getLastPathComponent( (*it)->getFileName() ) + "\"";
            retList->setElement( index, RbPtr<RbObject>( new Variable(RbPtr<DAGNode>( new ConstantNode(RbPtr<RbLanguageObject>( (*it) ) ) ) ) ) );
            index++;
            }
        return RbPtr<RbLanguageObject>( retList );
        throw RbException("Wanted to create a List of Alignment but List does not exist anymore. See Func_readAlignment");
        }
    else if ( m.size() == 1 ) 
        {
        return RbPtr<RbLanguageObject>( m[0] );
        }
    else
        {
        // Return null object
        return RbPtr<RbLanguageObject>::getNullPtr();
        }
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readCharacterData::formatError(RbFileManager& fm, std::string& errorStr) {

    bool fileNameProvided    = fm.isFileNamePresent();
    bool isFileNameGood      = fm.testFile();
    bool isDirectoryNameGood = fm.testDirectory();
    
    if ( fileNameProvided == false && isDirectoryNameGood == false )
        errorStr += "Could not read contents of directory \"" + fm.getFilePath() + "\" because the directory does not exist";
    else if (fileNameProvided == true && (isFileNameGood == false || isDirectoryNameGood == false))
        {
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
RbPtr<const ArgumentRules> Func_readCharacterData::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    static bool          rulesSet = false;
    
    if (!rulesSet) 
        {
        argumentRules->push_back( RbPtr<ArgumentRule>( new ValueRule( "file", RbString_name ) ) );
        rulesSet = true;
        }
            
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_readCharacterData::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_readAlignment_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_readCharacterData::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_readCharacterData::getTypeSpec(void) const {
    return typeSpec;
}




