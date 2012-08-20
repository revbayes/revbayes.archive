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

#include "ArgumentRule.h"
#include "CharacterData.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readCharacterData.h"
#include "NclReader.h"
#include "RbBoolean.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RlCharacterData.h"
#include "RlVector.h"
#include "StringUtilities.h"
#include "UserInterface.h"

#include <map>
#include <set>
#include <sstream>


/** Clone object */
Func_readCharacterData* Func_readCharacterData::clone( void ) const {
    
    return new Func_readCharacterData( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_readCharacterData::executeFunction( const std::vector<const RbObject*>& args ) {

    // get the information from the arguments for reading the file
    const RbString& fn = static_cast<const RbString&>( *args[0] );
    
    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn.getValue() );
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
        std::cout << "myFileType = " << myFileType << std::endl;
            
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
    std::vector<CharacterData*> m = reader.readMatrices( fileMap );
    
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
    
    RlVector<RlCharacterData> *retList;
    // return either a list of character matrices or a single character matrix wrapped up in a DAG node
    if ( m.size() > 1 )
        {
        retList = new RlVector<RlCharacterData>( m.size() );
        size_t index = 0;
        for (std::vector<CharacterData*>::iterator it = m.begin(); it != m.end(); it++)
            {
            std::string eName = "Data from file \"" + StringUtilities::getLastPathComponent( (*it)->getFileName() ) + "\"";
            retList->setElement( index, new RlCharacterData( *(*it) ) );
            index++;
            }
        return retList;
        throw RbException("Wanted to create a List of Alignment but List does not exist anymore. See Func_readAlignment");
        }
    else if ( m.size() == 1 ) 
        {
        return RbPtr<RbLanguageObject>( new RlCharacterData(*m[0]) );
        }
    else
        {
        // Return null object
            return NULL;
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
const ArgumentRules& Func_readCharacterData::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) 
        {
        argumentRules.push_back( new ArgumentRule( "file", true, RbString::getClassTypeSpec() ) );
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
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_readCharacterData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readCharacterData::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlCharacterData::getClassTypeSpec();
    return returnTypeSpec;
}




