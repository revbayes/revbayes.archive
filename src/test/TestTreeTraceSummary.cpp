
#include "BranchLengthTree.h"
#include "NewickConverter.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "StringUtilities.h"
#include "TestTreeTraceSummary.h"
#include "TreeTrace.h"
#include "Tree.h"
#include "TreeSummary.h"

using namespace RevBayesCore;

TestTreeTraceSummary::TestTreeTraceSummary(const std::string &fn) : filename( fn ){
    
}

TestTreeTraceSummary::~TestTreeTraceSummary() {
    // nothing to do
}


TreeTrace<BranchLengthTree> TestTreeTraceSummary::readTreeTrace(const std::string &fname) {
    // check that the file/path name has been correctly specified
    RbFileManager myFileManager( fname );
    if ( (myFileManager.isFileNamePresent() == false && myFileManager.testDirectory() == false) ||
        (myFileManager.isFileNamePresent() == true  && (myFileManager.testFile() == false || myFileManager.testDirectory() == false)) ){
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException(errorStr);
    }
    
    // are we reading a single file or are we reading the contents of a directory?
    bool readingDirectory = myFileManager.isDirectory();
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
    
    std::vector<TreeTrace<BranchLengthTree> > data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++) {
        bool hasHeaderBeenRead = false;
        
        /* Open file */
        std::ifstream inFile( fname.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fname + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cerr << "Processing file \"" << fname << "\"" << std::endl;
        
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
                    TreeTrace<BranchLengthTree> t = TreeTrace<BranchLengthTree>();
                    
                    std::string parmName = columns[j];
                    t.setParameterName(parmName);
                    t.setFileName(fname);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Tracess
            for (size_t j=1; j<columns.size(); j++) {
                TreeTrace<BranchLengthTree>& t = data[j-1];
                //                std::istringstream stm;
                //                stm.str(columns[j]);
                //                double d;
                //                stm >> d;
                
                NewickConverter c;
                BranchLengthTree *tau = c.convertFromNewick( columns[j] );
                
                //                std::cerr << *tau << std::endl;
                
                t.addObject( *tau );
                
                delete tau;
            }
        }
    }
    
    return data[0];
}


bool TestTreeTraceSummary::run( void ) {
    
    // read in the tree trace
    std::cout << "Read tree trace '" << filename << "'" << std::endl;
    TreeTrace<BranchLengthTree> trace = readTreeTrace(filename);
    
    TreeSummary<BranchLengthTree> summary = TreeSummary<BranchLengthTree>(trace);
    summary.summarize();
    summary.printTreeSummary(std::cerr);
    
    return true;
}
