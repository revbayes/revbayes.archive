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

#include "Alignment.h"
#include "ConstantNode.h"
#include "Func_readAlignment.h"
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
const TypeSpec Func_readAlignment::typeSpec(Func_readAlignment_name);
const TypeSpec Func_readAlignment::returnTypeSpec(Vector_name, RbPtr<TypeSpec>(new TypeSpec(Alignment_name) ) );

/** Clone object */
Func_readAlignment* Func_readAlignment::clone( void ) const {
    
    return new Func_readAlignment( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_readAlignment::execute( void ) {

    // get the information from the arguments for reading the file
    const RbPtr<RbString> fn( static_cast<RbString*>( args[0]->getValue().get() ) );

    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fn->getValue() );
    if ( (myFileManager.isFileNamePresent() == false && myFileManager.testDirectory() == false) ||
         (myFileManager.isFileNamePresent() == true  && (myFileManager.testFile() == false || myFileManager.testDirectory() == false)) )
        {
        std::string errorStr = "";
        formatError(myFileManager, errorStr);
        //throw( RbException(errorStr) );
        }

    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = myFileManager.isDirectory(fn->getValue());
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
    std::vector<RbPtr<Alignment> > m = reader.readMatrices( fileMap );
    
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
        for (std::vector<RbPtr<Alignment> >::iterator it = m.begin(); it != m.end(); it++)
            {
            std::string eName = "Data from file \"" + StringUtilities::getLastPathComponent( (*it)->getFileName() ) + "\"";
            retList->setElement( index, new Variable(RbPtr<DAGNode>( new ConstantNode(*it) ) ) );
            index++;
            }
        return retList;
            throw RbException("Wanted to create a List of Alignment but List does not exist anymore. See Func_readAlignment");
        }
    else if ( m.size() == 1 ) 
        {
        return m[0];
        }
    else
        {
        // Return null object
        return NULL;
        }
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readAlignment::formatError(RbFileManager& fm, std::string& errorStr) {

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
const ArgumentRules& Func_readAlignment::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules;
    static bool          rulesSet = false;
    
    if (!rulesSet) 
        {
        argumentRules.push_back( new ValueRule( "file", RbString_name ) );
        rulesSet = true;
        }
            
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_readAlignment::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_readAlignment_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_readAlignment::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_readAlignment::getTypeSpec(void) const {
    return typeSpec;
}




