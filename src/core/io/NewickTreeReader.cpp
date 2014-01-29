#include "BranchLengthTree.h"
#include "NewickConverter.h"
#include "NewickTreeReader.h"
#include "RbException.h"

#include <fstream>

using namespace RevBayesCore;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
NewickTreeReader::NewickTreeReader() 
{
    
}



/**
 *
 */
std::vector<BranchLengthTree*>* NewickTreeReader::readBranchLengthTrees(std::string const &fn) 
{
    /* Open file */
    std::ifstream inFile( fn.c_str() );
    
    if ( !inFile )
        throw RbException( "Could not open file \"" + fn + "\"" );
    
    /* Initialize */
    std::vector<BranchLengthTree*>* trees = new std::vector<BranchLengthTree*>();
    std::string commandLine;
    
    /* line-processing loop */
    while ( inFile.good() ) 
    {
        
        // Read a line
        std::string line;
        getline( inFile, line );
        
        // skip empty lines
        if (line.length() == 0) 
        {
            continue;
        }
                
        
        NewickConverter c;
        BranchLengthTree *blTree = c.convertFromNewick( line );
        
        trees->push_back( blTree );
    }
    
    return trees;
}
