#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTrace.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlString.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TraceNumeric.h"
#include "RlTrace.h"
#include "RlUserInterface.h"
#include "WorkspaceVector.h"

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
Func_readTrace* Func_readTrace::clone( void ) const
{
    
    return new Func_readTrace( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readTrace::execute( void )
{

    // get the information from the arguments for reading the file
    const RlString&     fn       = static_cast<const RlString&>( args[0].getVariable()->getRevObject() );
    // get the column delimiter
    const std::string& delimiter = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    
    
    // check that the file/path name has been correctly specified
    RevBayesCore::RbFileManager myFileManager( fn.getValue() );
    if ( !myFileManager.testFile() || !myFileManager.testDirectory() ) {
        std::string errorStr = "";
        myFileManager.formatError( errorStr );
        throw( RbException(errorStr) );
    }
        
    // set up a vector of strings containing the name or names of the files to be read
    std::vector<std::string> vectorOfFileNames;
    if ( myFileManager.isFile() )
    {
        vectorOfFileNames.push_back( myFileManager.getFileName() );
    }
    else 
    {
        myFileManager.setStringWithNamesOfFilesInDirectory( vectorOfFileNames );
    }
        
    std::vector<RevBayesCore::TraceNumeric> data;
        
    
    // Set up a map with the file name to be read as the key and the file type as the value. Note that we may not
    // read all of the files in the string called "vectorOfFileNames" because some of them may not be in a format
    // that can be read.
    std::map<std::string,std::string> fileMap;
    for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
    {
        bool hasHeaderBeenRead = false;
            
        /* Open file */
        std::ifstream inFile( fn.getValue().c_str() );
        
        if ( !inFile )
            throw RbException( "Could not open file \"" + fn.getValue() + "\"" );
            
        /* Initialize */
        std::string commandLine;
        RBOUT("Processing file \"" + fn.getValue() + "\"");
            
        /* Command-processing loop */
        while ( inFile.good() )
        {
                
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
            if (line[0] == '#')
            {
                continue;
            }
                
            // splitting every line into its columns
            std::vector<std::string> columns;
            StringUtilities::stringSplit(line, delimiter, columns);
                
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead)
            {
                    
                for (size_t j=0; j<columns.size(); j++)
                {
                    RevBayesCore::TraceNumeric t;
                        
                    std::string parmName = columns[j];
                    t.setParameterName(parmName);
                    t.setFileName(fn.getValue());
                        
                    data.push_back( t );
                }
                    
                hasHeaderBeenRead = true;
                    
                continue;
            }
                
            // adding values to the Tracess
            for (size_t j=0; j<columns.size(); j++)
            {
                RevBayesCore::TraceNumeric& t = static_cast<RevBayesCore::TraceNumeric&>( data[j] );
                std::string tmp = columns[j];
                double d = atof( tmp.c_str() );
                t.addObject(d);
            }
        }
    }
    
    WorkspaceVector<Trace> *rv = new WorkspaceVector<Trace>();
    for (std::vector<RevBayesCore::TraceNumeric>::iterator it = data.begin(); it != data.end(); ++it)
    {
        it->computeStatistics();
        rv->push_back( Trace( *it ) );
    }
    
    // return the vector of traces
    return new RevVariable( rv );
}


/** Get argument rules */
const ArgumentRules& Func_readTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        
        argumentRules.push_back( new ArgumentRule( "file"     , RlString::getClassTypeSpec(), "Name of the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), "The delimiter between columns (e.g., the iteration number and the trees).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readTrace::getClassType(void)
{
    
    static std::string revType = "Func_readTrace";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readTrace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readTrace::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readTrace";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readTrace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readTrace::getReturnType( void ) const
{
    static TypeSpec returnTypeSpec = WorkspaceVector<Trace>::getClassTypeSpec();
    return returnTypeSpec;
}


