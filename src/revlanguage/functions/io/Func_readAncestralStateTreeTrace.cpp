/**
 * @file
 * This file contains the impementation of Func_readAncestralStateTreeTrace.
 * This class is differentiated from Func_readTreeTrace only in the sense that
 * the trees put into the TraceTree have not been rerooted; the nodes keep the
 * indexes already assigned to them, which is necessary for ancestral state
 * reconstruction.
 *
 * @brief Impementation of Func_readAncestralStateTreeTrace
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readAncestralStateTreeTrace.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TraceTree.h"
#include "TreeUtilities.h"

#include <map>
#include <set>
#include <sstream>


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_readAncestralStateTreeTrace* Func_readAncestralStateTreeTrace::clone( void ) const
{
    
    return new Func_readAncestralStateTreeTrace( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readAncestralStateTreeTrace::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string&  fn       = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string&  treetype = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string&  sep      = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn );
    
    if ( !myFileManager.testFile() || !myFileManager.testDirectory() )
    {
        std::string errorStr = "";
        myFileManager.formatError(errorStr);
        throw RbException(errorStr);
    }
    
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if ( myFileManager.isFile() )
    {
        vectorOfFileNames.push_back( myFileManager.getFullFileName() );
    }
    else
    {
        myFileManager.setStringWithNamesOfFilesInDirectory( vectorOfFileNames );
    }
    
    RevObject *rv;
    if ( treetype == "clock" )
    {
        rv = readTimeTrees(vectorOfFileNames, sep);
    }
    else if ( treetype == "non-clock" )
    {
        rv = readBranchLengthTrees(vectorOfFileNames, sep);
    }
    else
    {
        throw RbException("Unknown tree type to read.");
    }
    
    return new RevVariable( rv );
}


/** Get argument rules */
const ArgumentRules& Func_readAncestralStateTreeTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
		
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec(), "The name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        std::vector<std::string> options;
        options.push_back( "clock" );
        options.push_back( "non-clock" );
        argumentRules.push_back( new OptionRule( "treetype", new RlString("clock"), options, "The type of tree." ) );
        argumentRules.push_back( new ArgumentRule( "separator", RlString::getClassTypeSpec(), "The separater/delimiter between values.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readAncestralStateTreeTrace::getClassType(void)
{
    
    static std::string revType = "Func_readAncestralStateTreeTrace";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readAncestralStateTreeTrace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readAncestralStateTreeTrace::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readAncestralStateTreeTrace";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readAncestralStateTreeTrace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readAncestralStateTreeTrace::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TraceTree::getClassTypeSpec();
    return returnTypeSpec;
}


TraceTree* Func_readAncestralStateTreeTrace::readBranchLengthTrees(const std::vector<std::string> &vectorOfFileNames, const std::string &delimitter)
{
    
    
    std::vector<RevBayesCore::TraceTree> data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::const_iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        bool hasHeaderBeenRead = false;
        const std::string &fn = *p;
        
        /* Open file */
        std::ifstream inFile( fn.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fn + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cout << "Processing file \"" << fn << "\"" << std::endl;
        
        size_t index = 0;
        
        std::string outgroup = "";
        
        /* Command-processing loop */
        while ( inFile.good() )
        {
            
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
            if (line[0] == '#')
            {
                continue;
            }
            
            // splitting every line into its columns
            std::vector<std::string> columns;
            
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
                    
                    RevBayesCore::TraceTree t = RevBayesCore::TraceTree( false );
                    
                    t.setParameterName(parmName);
                    t.setFileName(fn);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Traces
            RevBayesCore::TraceTree& t = data[0];
            
            RevBayesCore::NewickConverter c;
            RevBayesCore::Tree *tau = c.convertFromNewickNoReIndexing( columns[index] );
			
            t.addObject( tau );
        }
    }	
    return new TraceTree( data[0] );
}


TraceTree* Func_readAncestralStateTreeTrace::readTimeTrees(const std::vector<std::string> &vectorOfFileNames, const std::string &delimitter) {
    
    
    std::vector<RevBayesCore::TraceTree> data;
    
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::const_iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        bool hasHeaderBeenRead = false;
        const std::string &fn = *p;
        
        /* Open file */
        std::ifstream inFile( fn.c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fn + "\"" );
        
        /* Initialize */
        std::string commandLine;
        std::cout << "Processing file \"" << fn << "\"" << std::endl;
        
        size_t index = 0;
        
        /* Command-processing loop */
        while ( inFile.good() )
        {
            
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
            
            // we should provide other delimiters too
            StringUtilities::stringSplit(line, delimitter, columns);
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead)
            {
                
                for (size_t j=1; j<columns.size(); j++)
                {
                    
                    std::string parmName = columns[j];
                    if ( parmName == "Posterior" || parmName == "Likelihood" || parmName == "Prior") {
                        continue;
                    }
                    index = j;
                    
                    RevBayesCore::TraceTree t = RevBayesCore::TraceTree( true );
                    
                    t.setParameterName(parmName);
                    t.setFileName(fn);
                    
                    data.push_back( t );
                }
                
                hasHeaderBeenRead = true;
                
                continue;
            }
            
            // adding values to the Traces
            RevBayesCore::TraceTree& t = data[0];
            
            RevBayesCore::NewickConverter c;
            RevBayesCore::Tree *blTree = c.convertFromNewickNoReIndexing( columns[index] );
            RevBayesCore::Tree *tau = RevBayesCore::TreeUtilities::convertTree( *blTree );
            
            t.addObject( tau );
			
        }
    }
    
    return new TraceTree( data[0] );
}


