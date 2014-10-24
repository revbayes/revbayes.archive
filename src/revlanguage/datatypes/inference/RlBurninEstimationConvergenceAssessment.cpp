#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BurninEstimatorContinuous.h"
#include "ConvergenceDiagnosticContinuous.h"
#include "EssMax.h"
#include "EssTest.h"
#include "GelmanRubinTest.h"
#include "GewekeTest.h"
#include "HeidelbergerWelchTest.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RlBurninEstimationConvergenceAssessment.h"
#include "RlString.h"
#include "RlUserInterface.h"
#include "SemMin.h"
#include "StationarityTest.h"
#include "StringUtilities.h"
#include "TypeSpec.h"
#include "ModelVector.h"


using namespace RevLanguage;

BurninEstimationConvergenceAssessment::BurninEstimationConvergenceAssessment() : WorkspaceObject(),
    delimiter( "\t" ),
    filenames(),
    burninMethod( "ESS" ),
    verbose( false )
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
    
    std::vector<std::string> options;
    options.push_back( "ESS" );
    options.push_back( "SEM" );
    
    ArgumentRules* burninMethodArgRules = new ArgumentRules();
    burninMethodArgRules->push_back( new OptionRule("method", options ) );
    methods.addFunction("setBurninMethod", new MemberProcedure( RlUtils::Void, burninMethodArgRules) );
    
    ArgumentRules* verboseArgRules = new ArgumentRules();
    verboseArgRules->push_back( new ArgumentRule("x", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("verbose", new MemberProcedure( RlUtils::Void, verboseArgRules) );

    
}


/** Clone object */
BurninEstimationConvergenceAssessment* BurninEstimationConvergenceAssessment::clone(void) const
{
    
	return new BurninEstimationConvergenceAssessment(*this);
}



/* Map calls to member methods */
RevPtr<Variable> BurninEstimationConvergenceAssessment::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    RevPtr<Variable> retVar;
    
    if (name == "run")
    {
        bool passed = true;
        
        RBOUT("\n\t*********************************************");
        RBOUT("\tBurn-in Estimation and Convergence Assessment");
        RBOUT("\t*********************************************\n\n");
        
        RevBayesCore::BurninEstimatorContinuous *burninEst = NULL;
        
        if ( burninMethod == "ESS" )
        {
            burninEst = new RevBayesCore::EssMax();
        }
        else if ( burninMethod == "SEM" )
        {
            burninEst = new RevBayesCore::SemMin();
        }
        else
        {
            throw RbException("Unknown burnin estimation method");
        }
        
        RevBayesCore::ConvergenceDiagnosticContinuous *essTest = new RevBayesCore::EssTest();
        RevBayesCore::ConvergenceDiagnosticContinuous *gewekeTest = new RevBayesCore::GewekeTest();
        RevBayesCore::ConvergenceDiagnosticContinuous *gelmanRubinTest = new RevBayesCore::GelmanRubinTest();
        RevBayesCore::ConvergenceDiagnosticContinuous *heidelbergerTest = new RevBayesCore::HeidelbergerWelchTest();
        RevBayesCore::ConvergenceDiagnosticContinuous *stationarityTest = new RevBayesCore::StationarityTest();
        
        // read the traces
        
        // set up a vector of strings containing the name or names of the files to be read
        std::vector<std::string> vectorOfFileNames;
        
        for (std::set<std::string>::const_iterator it=filenames.begin(); it!=filenames.end(); ++it)
        {
            const std::string &fn = *it;
            // check that the file/path name has been correctly specified
            RevBayesCore::RbFileManager myFileManager( fn );
        
            if ( myFileManager.isFile() )
            {
                vectorOfFileNames.push_back( myFileManager.getFullFileName() );
            }
            else
            {
                myFileManager.setStringWithNamesOfFilesInDirectory( vectorOfFileNames );
            }
        }
        
        
        std::stringstream tmp;
        tmp << "Processing " << vectorOfFileNames.size() << ( vectorOfFileNames.size() > 1 ? " files ..." : " file ...");
        RBOUT( tmp.str() );
        
        
        
        RBOUT("\n\t-----------------------------------");
        RBOUT("\tSingle Chain Convergence Assessment");
        RBOUT("\t-----------------------------------\n\n");
        
        std::vector< std::vector<RevBayesCore::Trace> > runs;
        std::vector< size_t > burnins;
        
        for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
        {
            
            std::vector<RevBayesCore::Trace> d;
            const std::string &fn = *p;
            
            RBOUT("\tProcessing file '" + fn + "'");
            
            // read in the traces from this file
            readTrace(fn, d);
            
            // add the traces to our runs
            runs.push_back( d );
            std::vector<RevBayesCore::Trace> &data = runs[runs.size()-1];
            
            size_t maxBurnin = 0;
            
            for ( size_t i = 0; i < data.size(); ++i)
            {
                RevBayesCore::Trace &t = data[i];
                const std::vector<double> &v = t.getValues();
                size_t b = burninEst->estimateBurnin( v );
                if ( maxBurnin < b )
                {
                    maxBurnin = b;
                }
            }
            
            burnins.push_back( maxBurnin );
            
            bool failed = false;
            size_t numFailedParams = 0;
            for ( size_t i = 0; i < data.size(); ++i)
            {
                RevBayesCore::Trace &t = data[i];
                const std::vector<double> &v = t.getValues();
                t.setBurnin( maxBurnin );
                t.computeStatistics();
                
                const std::string &traceName = t.getParameterName();
                
                
                bool gewekeStat = gewekeTest->assessConvergenceSingleChain( v, maxBurnin );
                bool essStat = essTest->assessConvergenceSingleChain( v, maxBurnin );
//                bool gelmanStat = gelmanRubinTest->assessConvergenceSingleChain( v, maxBurnin );
                bool stationarityStat = stationarityTest->assessConvergenceSingleChain( v, maxBurnin );
                bool heidelbergerStat = heidelbergerTest->assessConvergenceSingleChain( v, maxBurnin );
                bool failedParam = !gewekeStat || !stationarityStat || !heidelbergerStat || !essStat;
                
                if ( failedParam == true )
                {
                    numFailedParams++;
                }
                
                failed |= failedParam;
                
                if ( verbose == true )
                {
                    RBOUT("\t\tResults for parameter '" + traceName + "'\n" );
                    std::stringstream ss("");
                    ss << "\t\t\tESS = " << t.getEss();
                    RBOUT( ss.str() );
                    std::string p = (gewekeStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Geweke test:\t\t\t\t" + p);
                    p = (essStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed ESS test:\t\t\t\t" + p);
//                    p = (gelmanStat ? "TRUE" : "FALSE");
//                    RBOUT("\t\t\tPassed Gelman-Rubin test:\t\t\t" + p);
                    p = (stationarityStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Stationarity test:\t\t\t" + p);
                    p = (heidelbergerStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Heideberger-Welch test:\t\t" + p);
                }
                
            }
            
            if ( failed )
            {
                std::stringstream ss("");
                ss << "\tConvergence assessment detected " << numFailedParams << " possible issues in file '" + fn + "'.\n\n";
                RBOUT( ss.str() );
                
            }
            else
            {
                RBOUT("No failure to convergence could be detected in file '"+ fn +"'.\n\n");
            }
            
            passed &= !failed;
        }
        
        
        // now, compare the different runs
        if ( runs.size() > 1 )
        {
            RBOUT("\n\t----------------------------------");
            RBOUT("\tMulti Chain Convergence Assessment");
            RBOUT("\t----------------------------------\n\n");
            
            std::vector<RevBayesCore::Trace> &run = runs[0];
            
            bool failed = false;
            size_t numFailedParams = 0;
            
            for (size_t j=0; j<run.size(); ++j)
            {
                
                RevBayesCore::Trace &t = run[j];
                const std::string &traceName = t.getParameterName();
                std::vector< std::vector<double> > v;
                v.push_back( t.getValues() );
                
                for (size_t i=1; i<runs.size(); ++i)
                {
                    
                    size_t index = runs[i].size();
                    for (size_t k=0; k<runs[i].size(); ++k)
                    {
                        if ( runs[i][k].getParameterName() == traceName )
                        {
                            index = k;
                            break;
                        }
                    }
                    
                    if ( index == runs[i].size() )
                    {
                        throw RbException("Could not find a trace for parameter '" + traceName + "' in file '" + runs[i][0].getFileName() + "'.");
                    }
                    RevBayesCore::Trace &nextTrace = runs[i][index];
                    v.push_back( nextTrace.getValues() );
            
                }
                
                
//                bool gewekeStat = gewekeTest->assessConvergenceMultipleChains( v, burnins );
//                bool essStat = essTest->assessConvergenceMultipleChains( v, burnins );
                bool gelmanStat = gelmanRubinTest->assessConvergenceMultipleChains( v, burnins );
                bool stationarityStat = stationarityTest->assessConvergenceMultipleChains( v, burnins );
//                bool heidelbergerStat = heidelbergerTest->assessConvergenceMultipleChains( v, burnins );
                bool failedParam =  !gelmanStat || !stationarityStat;
                
                if ( failedParam == true )
                {
                    numFailedParams++;
                }
                
                failed |= failedParam;
                
                if ( verbose == true )
                {
                    RBOUT("\t\tResults for parameter '" + traceName + "'\n" );
                    std::string p = (gelmanStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Gelman-Rubin test:\t\t\t" + p);
                    p = (stationarityStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Stationarity test:\t\t\t" + p);
                }
            
            }
            
            
            
            if ( failed )
            {
                std::stringstream ss("");
                ss << "\tConvergence assessment detected " << numFailedParams << " possible issues.\n\n";
                RBOUT( ss.str() );
                
            }
            else
            {
                RBOUT("No failure to convergence could be detected.\n\n");
            }
            
        }
        
        RBOUT("\n");
        
        retVar = new Variable( new RlBoolean( passed ) );
        
    }
    else if (name == "setBurninMethod")
    {
        // get the member with give index
        burninMethod = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
        
    }
    else if (name == "verbose")
    {
        // get the member with give index
        verbose = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getValue();
        
    }
    else
    {
        retVar = RevObject::executeMethod( name, args );
    }
    
    return retVar;
}


/** Get Rev type of object */
const std::string& BurninEstimationConvergenceAssessment::getClassType(void) {
    
    static std::string revType = "BurninEstimationConvergenceAssessment";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& BurninEstimationConvergenceAssessment::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& BurninEstimationConvergenceAssessment::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        std::vector<TypeSpec> filenameTypes;
        filenameTypes.push_back( RlString::getClassTypeSpec() );
        filenameTypes.push_back( ModelVector<RlString>::getClassTypeSpec() );
        memberRules.push_back( new ArgumentRule("filename", filenameTypes, ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("delimiter", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& BurninEstimationConvergenceAssessment::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void BurninEstimationConvergenceAssessment::printValue(std::ostream &o) const {
    
    o << "BurninEstimationConvergenceAssessment";
}



void BurninEstimationConvergenceAssessment::readTrace(const std::string &fn, std::vector<RevBayesCore::Trace> &data)
{
    
    bool hasHeaderBeenRead = false;
    
    /* Open file */
    std::ifstream inFile( fn.c_str() );
    
    if ( !inFile )
        throw RbException( "Could not open file \"" + fn + "\"" );
    
    /* Initialize */
    std::string commandLine;
    
    size_t startIndex = 0;
    
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
            
            // do not add the iteration number as a trace
            if ( columns[0] == "Iteration" )
            {
                startIndex = 1;
            }
            
            for (size_t j=startIndex; j<columns.size(); j++)
            {
                RevBayesCore::Trace t;
                
                std::string parmName = columns[j];
                t.setParameterName(parmName);
                t.setFileName(fn);
                
                data.push_back( t );
            }
            
            hasHeaderBeenRead = true;
            
            continue;
        }
        
        // adding values to the Tracess
        for (size_t j=startIndex; j<columns.size(); j++)
        {
            RevBayesCore::Trace& t = data[j-startIndex];
            std::string tmp = columns[j];
            double d = atof( tmp.c_str() );
            t.addObject(d);
        }
    }
    
}


/** Set a member variable */
void BurninEstimationConvergenceAssessment::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "filename")
    {
        // empty the files names
        if ( var->getRevObject().getTypeSpec().isDerivedOf( RlString::getClassTypeSpec() ) )
        {
            filenames.insert( static_cast<const RlString&>( var->getRevObject() ).getValue() );
        }
        else
        {
            const std::vector<std::string> &fn = static_cast<const ModelVector<RlString> &>( var->getRevObject() ).getValue();
            for (std::vector<std::string>::const_iterator it=fn.begin(); it!=fn.end(); ++it)
            {
                filenames.insert( *it );
            }
        }
        
    }
    else if ( name == "delimiter")
    {
        delimiter = static_cast<const RlString&>( var->getRevObject() ).getValue();
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
    
}
