#include "ArgumentRule.h"
#include "BranchLengthTree.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTreeTrace.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTreeTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeTrace.h"
#include "TreeUtilities.h"
#include "UserInterface.h"
#include "Vector.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/** Clone object */
Func_readTreeTrace* Func_readTreeTrace::clone( void ) const {
    
    return new Func_readTreeTrace( *this );
}


/** Execute function */
RbLanguageObject* Func_readTreeTrace::execute( void ) {
    
    // get the information from the arguments for reading the file
    const RlString&     fn       = static_cast<const RlString&>( args[0].getVariable()->getValue() );
    const std::string&  treetype = static_cast<const RlString&>( args[1].getVariable()->getValue() ).getValue();
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn.getValue() );
    if ( (myFileManager.isFileNamePresent() == false && myFileManager.testDirectory() == false) ||
        (myFileManager.isFileNamePresent() == true  && (myFileManager.testFile() == false || myFileManager.testDirectory() == false)) ){
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException(errorStr);
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = myFileManager.isDirectory( fn.getValue() );
    if (readingDirectory == true)
        std::cerr << "Recursively reading the contents of a directory" << std::endl;
    else
        std::cerr << "Attempting to read the contents of file \"" << myFileManager.getFileName() << "\"" << std::endl;
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if (readingDirectory == true) {
        myFileManager.setStringWithNamesOfFilesInDirectory(vectorOfFileNames);
    }
    else {
#       if defined (WIN32)
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "\\" + myFileManager.getFileName() );
#       else
        vectorOfFileNames.push_back( myFileManager.getFilePath() + "/" + myFileManager.getFileName() );
#       endif
    }
    if (readingDirectory == true) {
        std::cerr << "Found " << vectorOfFileNames.size() << " files in directory" << std::endl;
    }
    
    RbLanguageObject *rv;
    if ( treetype == "clock" ) {
        rv = readTimeTrees(vectorOfFileNames);
    } else if ( treetype == "non-clock" ) {
        rv = readBranchLengthTrees(vectorOfFileNames);
    } else {
        throw RbException("Unknown tree type to read.");
    }
    
    return rv;
}


/** Format the error exception string for problems specifying the file/path name */
void Func_readTreeTrace::formatError(RevBayesCore::RbFileManager& fm, std::string& errorStr) {
    
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
const ArgumentRules& Func_readTreeTrace::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) {
        argumentRules.push_back( new ArgumentRule( "file", true, RlString::getClassTypeSpec() ) );
        Vector<RlString> options;
        options.push_back( RlString("clock") );
        options.push_back( RlString("non-clock") );
        argumentRules.push_back( new OptionRule( "treetype", new RlString("clock"), options ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_readTreeTrace::getClassName(void) { 
    
    static std::string rbClassName = "Read tree trace function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_readTreeTrace::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_readTreeTrace::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readTreeTrace::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = TreeTrace<BranchLengthTree>::getClassTypeSpec();
    return returnTypeSpec;
}


TreeTrace<BranchLengthTree>* Func_readTreeTrace::readBranchLengthTrees(const std::vector<std::string> &vectorOfFileNames) {
    
    
    std::vector<RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree> > data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::const_iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool hasHeaderBeenRead = false;
        const std::string &fn = *p;
        
        /* Open file */
        std::ifstream inFile( fn.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fn + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cerr << "Processing file \"" << fn << "\"" << std::endl;
        
        size_t index = 0;
        
        /* Command-processing loop */
        while ( inFile.good() ) {
            
            // Read a line
            std::string line;
            getline( inFile, line );
            
            // skip empty lines
            //line = StringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
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
            std::string delimitter = "\t";
            
            // we should provide other delimiters too
            StringUtilities::stringSplit(line, delimitter, columns);
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
                
                for (size_t j=1; j<columns.size(); j++) {
                    
                    std::string parmName = columns[j];
                    if ( parmName == "Posterior" || parmName == "Likelihood" || parmName == "Prior") {
                        continue;
                    }
                    index = j;
                    
                    RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree> t = RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree>();
                    
                    t.setParameterName(parmName);
                    t.setFileName(fn);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Traces
            //            for (size_t j=1; j<columns.size(); j++) {
            RevBayesCore::TreeTrace<RevBayesCore::BranchLengthTree>& t = data[0];
            
            RevBayesCore::NewickConverter c;
            RevBayesCore::BranchLengthTree *tau = c.convertFromNewick( columns[index] );
            
            t.addObject( *tau );
            
            delete tau;
            //            }
        }
    }
    
    return new TreeTrace<BranchLengthTree>( data[0] );
}


TreeTrace<TimeTree>* Func_readTreeTrace::readTimeTrees(const std::vector<std::string> &vectorOfFileNames) {
    
    
    std::vector<RevBayesCore::TreeTrace<RevBayesCore::TimeTree> > data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::const_iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool hasHeaderBeenRead = false;
        const std::string &fn = *p;
        
        /* Open file */
        std::ifstream inFile( fn.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fn + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cerr << "Processing file \"" << fn << "\"" << std::endl;
        
        size_t index = 0;
        
        /* Command-processing loop */
        while ( inFile.good() ) {
            
            // Read a line
            std::string line;
            getline( inFile, line );
            
            // skip empty lines
            //line = StringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
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
            std::string delimitter = "\t";
            
            // we should provide other delimiters too
            StringUtilities::stringSplit(line, delimitter, columns);
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
                
                for (size_t j=1; j<columns.size(); j++) {
                    
                    std::string parmName = columns[j];
                    if ( parmName == "Posterior" || parmName == "Likelihood" || parmName == "Prior") {
                        continue;
                    }
                    index = j;
                    
                    RevBayesCore::TreeTrace<RevBayesCore::TimeTree> t = RevBayesCore::TreeTrace<RevBayesCore::TimeTree>();
                    
                    t.setParameterName(parmName);
                    t.setFileName(fn);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Traces
            //            for (size_t j=1; j<columns.size(); j++) {
            RevBayesCore::TreeTrace<RevBayesCore::TimeTree>& t = data[0];
            
            RevBayesCore::NewickConverter c;
            RevBayesCore::BranchLengthTree *blTree = c.convertFromNewick( columns[index] );
            RevBayesCore::TimeTree *tau = RevBayesCore::TreeUtilities::convertTree( *blTree );
            
            t.addObject( *tau );
            
            delete tau;
            //            }
        }
    }
    
    return new TreeTrace<TimeTree>( data[0] );
}


