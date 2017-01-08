#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "Ellipsis.h"
#include "Func_readTreeTrace.h"
#include "ModelVector.h"
#include "NclReader.h"
#include "NewickConverter.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTraceTree.h"
#include "RlUserInterface.h"
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
Func_readTreeTrace* Func_readTreeTrace::clone( void ) const
{
    
    return new Func_readTreeTrace( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_readTreeTrace::execute( void )
{
    
    // get the information from the arguments for reading the file
    
    const std::string&  treetype = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string&  sep      = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    
    std::vector<std::string> vectorOfFileNames;
    
    if ( args[0].getVariable()->getRevObject().isType( ModelVector<RlString>::getClassTypeSpec() ) )
    {
        const ModelVector<RlString>& fn = static_cast<const ModelVector<RlString> &>( args[0].getVariable()->getRevObject() ).getValue();
        std::vector<std::string> tmp;
        for (size_t i = 0; i < fn.size(); i++)
        {
            RevBayesCore::RbFileManager myFileManager( fn[i] );
            
            if ( !myFileManager.testFile() && !(myFileManager.testDirectory() && myFileManager.getFileName() == "" ) )
            {
                std::string errorStr = "Could not find filename: " + myFileManager.getFileName() + "\n";
                myFileManager.formatError(errorStr);
                throw RbException(errorStr);
            }
            
            if ( myFileManager.isFile() )
            {
                tmp.push_back( myFileManager.getFullFileName() );
            }
            else
            {
                myFileManager.setStringWithNamesOfFilesInDirectory( tmp );
            }
        }
        for (size_t i = 0; i < tmp.size(); i++)
        {
            vectorOfFileNames.push_back(tmp[i]);
        }
        std::set<std::string> s( vectorOfFileNames.begin(), vectorOfFileNames.end() );
        vectorOfFileNames.assign( s.begin(), s.end() );
    }
    else
    {
        // check that the file/path name has been correctly specified
        const std::string&  fn       = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::RbFileManager myFileManager( fn );
        
        if ( !myFileManager.testFile() && !(myFileManager.testDirectory() && myFileManager.getFileName() == "" ) )
        {
            std::string errorStr = "Could not find filename: " + myFileManager.getFileName() + "\n";
            myFileManager.formatError(errorStr);
            throw RbException(errorStr);
        }
        
        // set up a vector of strings containing the name or names of the files to be read
        if ( myFileManager.isFile() )
        {
            vectorOfFileNames.push_back( myFileManager.getFullFileName() );
        }
        else
        {
            myFileManager.setStringWithNamesOfFilesInDirectory( vectorOfFileNames );
        }
    }
    
    TraceTree *rv;
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
    
    int burnin = 0;

    RevObject& b = args[3].getVariable()->getRevObject();
    if ( b.isType( Integer::getClassTypeSpec() ) )
    {
        burnin = static_cast<const Integer &>(b).getValue();
    }
    else
    {
        double burninFrac = static_cast<const Probability &>(b).getValue();
        burnin = int( floor( rv->getValue().size()*burninFrac ) );
    }

    rv->getValue().setBurnin(burnin);

    return new RevVariable( rv );
}


/** Get argument rules */
const ArgumentRules& Func_readTreeTrace::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if (!rules_set)
    {
        //        std::vector<TypeSpec> branchRateTypeentRule( "branchRates", branchRateTypes, "The global or branch-specific rate multipliers.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<TypeSpec> fileTypes;
        fileTypes.push_back( RlString::getClassTypeSpec() );
        fileTypes.push_back( ModelVector<RlString>::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "file"     , fileTypes, "The name of the tree trace file(s).", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> treeOptions;
        treeOptions.push_back( "clock" );
        treeOptions.push_back( "non-clock" );
        argumentRules.push_back( new OptionRule( "treetype", new RlString("clock"), treeOptions, "The type of trees." ) );
        argumentRules.push_back( new ArgumentRule( "separator", RlString::getClassTypeSpec(), "The separator/delimiter between values in the file.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );

        std::vector<TypeSpec> burninTypes;
        burninTypes.push_back( Probability::getClassTypeSpec() );
        burninTypes.push_back( Integer::getClassTypeSpec() );
        argumentRules.push_back( new ArgumentRule( "burnin"   , burninTypes     , "The fraction/number of samples to discard as burnin.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.25) ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readTreeTrace::getClassType(void)
{
    
    static std::string rev_type = "Func_readTreeTrace";
    
    return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readTreeTrace::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readTreeTrace::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readTreeTrace";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readTreeTrace::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_readTreeTrace::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = TraceTree::getClassTypeSpec();
    return returnTypeSpec;
}


TraceTree* Func_readTreeTrace::readBranchLengthTrees(const std::vector<std::string> &vectorOfFileNames, const std::string &delimitter)
{
    
    
    std::vector<RevBayesCore::TraceTree > data;
    
    
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
        RBOUT( "Processing file \"" + fn + "\"");
        
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
            if (line[0] == '#') {
                continue;
            }
            
            // splitting every line into its columns
            std::vector<std::string> columns;
            
            // we should provide other delimiters too
            StringUtilities::stringSplit(line, delimitter, columns);
            
            // we assume a header at the first line of the file
            if (!hasHeaderBeenRead) {
                
                for (size_t j=1; j<columns.size(); j++)
                {
                    
                    std::string parmName = columns[j];
                    if ( parmName == "Posterior" || parmName == "Likelihood" || parmName == "Prior")
                    {
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
            RevBayesCore::Tree *tau = c.convertFromNewick( columns[index] );
            
            // moved the reroot functionality below into
            // RevBayesCore::TreeSummary<BranchLengthTree>::summarizeTrees()
            // so that tree traces retain their original topology
            // will freyman 12/12/14
            
            //            if ( outgroup == "" )
            //            {
            //                RevBayesCore::BranchLengthTree& referenceTree = *tau;
            //                outgroup = referenceTree.getTipNode(0).getName();
            //            }
            //            // re-root the tree so that we can compare the the trees
            //            tau->reroot( outgroup );
            
            t.addObject( tau );
        }
    }
    
    return new TraceTree( data[0] );
}


TraceTree* Func_readTreeTrace::readTimeTrees(const std::vector<std::string> &vectorOfFileNames, const std::string &delimitter) {
    
    
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
        RBOUT( "Processing file \"" + fn + "\"");
        
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
            if (line[0] == '#')
            {
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
                    if ( parmName == "Posterior" || parmName == "Likelihood" || parmName == "Prior")
                    {
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
            //            for (size_t j=1; j<columns.size(); j++) {
            RevBayesCore::TraceTree& t = data[0];
            
            RevBayesCore::NewickConverter c;
            RevBayesCore::Tree *blTree = c.convertFromNewick( columns[index] );
            RevBayesCore::Tree *tau = RevBayesCore::TreeUtilities::convertTree( *blTree );
            
            t.addObject( tau );
        }
    }
    
    return new TraceTree( data[0] );
}


