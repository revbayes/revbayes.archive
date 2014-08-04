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

BurninEstimationConvergenceAssessment::BurninEstimationConvergenceAssessment() : RevObject(),
    delimiter( "\t" ),
    filename( "" ),
    burninMethod( "ESS" ),
    verbose( false )
{
    
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
        // check that the file/path name has been correctly specified
        RevBayesCore::RbFileManager myFileManager( filename );
        
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
        
        
        std::stringstream tmp;
        tmp << "Processing " << vectorOfFileNames.size() << ( vectorOfFileNames.size() > 1 ? " files ..." : " file ...");
        RBOUT( tmp.str() );
        
        for (std::vector<std::string>::iterator p = vectorOfFileNames.begin(); p != vectorOfFileNames.end(); p++)
        {
            std::vector<RevBayesCore::Trace> data;
            const std::string &fn = *p;
            readTrace(fn, data);
            
            RBOUT("\tProcessing file '" + fn + "'");
            
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
            
            bool failed = false;
            size_t numFailedParams = 0;
            std::vector<std::string> errors;
            std::vector<std::string> output;
            for ( size_t i = 0; i < data.size(); ++i)
            {
                RevBayesCore::Trace &t = data[i];
                const std::vector<double> &v = t.getValues();
                t.setBurnin( maxBurnin );
                t.computeStatistics();
                
                const std::string &traceName = t.getParameterName();
                
                
                bool gewekeStat = gewekeTest->assessConvergenceSingleChain( v, maxBurnin );
                bool essStat = essTest->assessConvergenceSingleChain( v, maxBurnin );
                bool gelmanStat = gelmanRubinTest->assessConvergenceSingleChain( v, maxBurnin );
                bool stationarityStat = stationarityTest->assessConvergenceSingleChain( v, maxBurnin );
                bool heidelbergerStat = heidelbergerTest->assessConvergenceSingleChain( v, maxBurnin );
                failed = !gewekeStat || !gelmanStat || !stationarityStat || !heidelbergerStat || !essStat;
                
                if ( failed == true )
                {
                    numFailedParams++;
                }
                
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
                    p = (gelmanStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Gelman-Rubin test:\t\t\t" + p);
                    p = (stationarityStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Stationarity test:\t\t\t" + p);
                    p = (heidelbergerStat ? "TRUE" : "FALSE");
                    RBOUT("\t\t\tPassed Heideberger-Welch test:\t\t" + p);
                }
                
                if ( !gewekeStat )
                {
                    errors.push_back("The parameter with name '" + traceName + "' failed the Geweke test.");
                }
                
                if ( !essStat )
                {
                    errors.push_back("The parameter with name '" + traceName + "' failed the ESS test.");
                }
                
                if ( !gelmanStat )
                {
                    errors.push_back("The parameter with name '" + traceName + "' failed the Gelman-Rubin test.");
                }
                
                if ( !stationarityStat )
                {
                    errors.push_back("The parameter with name '" + traceName + "' failed the stationarity test.");
                }
                
                if ( !heidelbergerStat )
                {
                    errors.push_back("The parameter with name '" + traceName + "' failed the Heidelberger-Welch test.");
                }
                
            }
            
            if ( failed )
            {
                std::stringstream ss("");
                ss << "\tConvergence assessment detected " << numFailedParams << " possible issues in file " + fn + ":";
                RBOUT( ss.str() );
                
            }
            else
            {
                RBOUT("No failure to convergence could be detected.");
            }
            
            passed &= !failed;
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
const MemberRules& BurninEstimationConvergenceAssessment::getMemberRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        std::vector<TypeSpec> filenameTypes;
        filenameTypes.push_back( RlString::getClassTypeSpec() );
        filenameTypes.push_back( ModelVector<RlString>::getClassTypeSpec() );
        memberRules.push_back( new ArgumentRule("filename", true, filenameTypes ) );
        memberRules.push_back( new ArgumentRule("delimiter", true, RlString::getClassTypeSpec(), new RlString("\t") ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& BurninEstimationConvergenceAssessment::getMethods(void) const {
    
    static MethodTable   methods    = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false )
    {
        ArgumentRules* runArgRules = new ArgumentRules();
        methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
        
        std::vector<RlString> options;
        options.push_back( RlString("ESS") );
        options.push_back( RlString("SEM") );
        
        ArgumentRules* burninMethodArgRules = new ArgumentRules();
        burninMethodArgRules->push_back( new OptionRule("method", options ) );
        methods.addFunction("setBurninMethod", new MemberProcedure( RlUtils::Void, burninMethodArgRules) );
        
        ArgumentRules* verboseArgRules = new ArgumentRules();
        verboseArgRules->push_back( new ArgumentRule("x", true, RlBoolean::getClassTypeSpec() ) );
        methods.addFunction("verbose", new MemberProcedure( RlUtils::Void, verboseArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
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
void BurninEstimationConvergenceAssessment::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "filename")
    {
        filename = static_cast<const RlString&>( var->getRevObject() ).getValue();
    }
    else if ( name == "delimiter")
    {
        delimiter = static_cast<const RlString&>( var->getRevObject() ).getValue();
    }
    else
    {
        RevObject::setConstMemberVariable(name, var);
    }
    
}
