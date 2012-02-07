/**
 * @file
 * This file contains the implementation of Func_readTraces.
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

#include "Func_readTraces.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbUtil.h"
#include "StringUtilities.h"
#include "Trace.h"
#include "UserInterface.h"
#include "ValueRule.h"
#include "Vector.h"
#include "VectorString.h"

#include <map>
#include <set>
#include <sstream>



// Definition of the static type spec member
const TypeSpec Func_readTraces::typeSpec(Func_readTraces_name);
const TypeSpec Func_readTraces::returnTypeSpec(Vector_name, new TypeSpec(Trace_name) );

/** Clone object */
Func_readTraces* Func_readTraces::clone( void ) const {
    
    return new Func_readTraces( *this );
}


/** Execute function */
RbLanguageObject* Func_readTraces::executeFunction( void ) {
    // get the information from the arguments for reading the file
    RbString* fn = static_cast<RbString*>( (*args)[0]->getValue() );
    
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
    
    Vector* data = new Vector(Trace_name);
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool hasHeaderBeenRead = false;
        
        /* Open file */
        std::string filename = static_cast<const RbString*>( (const RbObject*)(*args)[0]->getValue() )->getValue();
        std::ifstream inFile( filename.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + filename + "\"" );
        
        /* Initialize */
        std::string commandLine;
        RBOUT("Processing file \"" + filename + "\"");
        
        /* Command-processing loop */
        while ( inFile.good() ) {
            
            // Read a line
            std::string line;
            getline( inFile, line );
            
            // skip empty lines
            //line = stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
            if (line.length() == 0) 
            {
                continue;
            }
            
            
            // removing comments
            if (line[0] == '#') {
                continue;
            }
            
            // splitting every line into its columns
            std::vector<std::string> columns;
            // first, getting the file delimmiter
            std::string delimitter = ";";
            
            // we should provide other delimiters too
            //if ([delimitter isEqualToString:@"Tab Delimitted (txt)"] == YES) 
            //{
            //    columns = [line componentsSeparatedByString:@"\t"]; 
            //}
            //else if ([delimitter isEqualToString:@"Comma Delimmited (csv)"] == YES) 
            //{
            //    columns = [line componentsSeparatedByString:@","]; 
            //}
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
                
                for (size_t j=0; j<columns.size(); j++) {
                    Trace* t = new Trace();
                    
                    std::string parmName = columns[j];
                    t->setParameterName(parmName);
                    t->setFileName(filename);
                    
                    data->push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Tracess
            for (size_t j=0; j<columns.size(); j++) {
                Trace* t = static_cast<Trace*>( (*data)[j] );
                std::string tmp = columns[j];
                double d = 1.0;
                t->addObject(d);
            }
        }
    }
    
    
    return NULL;
    

}


/** Format the error exception string for problems specifying the file/path name */
void Func_readTraces::formatError(RbFileManager& fm, std::string& errorStr) {
    
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
const ArgumentRules* Func_readTraces::getArgumentRules( void ) const {
    
    static ArgumentRules* argumentRules = new ArgumentRules();
    static bool          rulesSet = false;
    
    if (!rulesSet) 
    {
        argumentRules->push_back( new ValueRule( "file", RbString_name ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_readTraces::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_readTraces_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_readTraces::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_readTraces::getTypeSpec(void) const {
    return typeSpec;
}




