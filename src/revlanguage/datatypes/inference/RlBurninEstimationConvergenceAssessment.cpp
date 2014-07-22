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


using namespace RevLanguage;

BurninEstimationConvergenceAssessment::BurninEstimationConvergenceAssessment() : RevObject(),
    filename( "" ),
    burninMethod( "ESS" )
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
            std::vector<std::string> failedGeweke;
            std::vector<std::string> failedGelman;
            std::vector<std::string> failedStationary;
            std::vector<std::string> failedESS;
            std::vector<std::string> failedHeidelberger;
            for ( size_t i = 0; i < data.size(); ++i)
            {
                RevBayesCore::Trace &t = data[i];
                const std::vector<double> &v = t.getValues();
                t.setBurnin( maxBurnin );
                
                const std::string &traceName = t.getParameterName();
                
                bool gewekeStat = gewekeTest->assessConvergenceSingleChain( v, maxBurnin );
                if ( !gewekeStat )
                {
                    failedGeweke.push_back( traceName );
                }
                
                bool essStat = essTest->assessConvergenceSingleChain( v, maxBurnin );
                if ( !essStat )
                {
                    failedESS.push_back( traceName );
                }
                
                bool gelmanStat = gelmanRubinTest->assessConvergenceSingleChain( v, maxBurnin );
                if ( !gelmanStat )
                {
                    failedGelman.push_back( traceName );
                }
                
                bool stationarityStat = stationarityTest->assessConvergenceSingleChain( v, maxBurnin );
                if ( !stationarityStat )
                {
                    failedStationary.push_back( traceName );
                }
                
                bool heidelbergerStat = heidelbergerTest->assessConvergenceSingleChain( v, maxBurnin );
                if ( !heidelbergerStat )
                {
                    failedHeidelberger.push_back( traceName );
                }
                
                failed = !gewekeStat || !gelmanStat || !stationarityStat || !heidelbergerStat || !essStat;
            }
            
            if ( failed )
            {
                RBOUT("\tConvergence assessment detected possible issues in file " + fn + ":");
                
                // printing failures of convergence of the Geweke test
                for (std::vector<std::string>::iterator it = failedGeweke.begin(); it != failedGeweke.end(); ++it)
                {
                    RBOUT("\t\tThe parameter with name '" + *it + " failed the Geweke test.");
                }
                
                // printing failures of convergence of the Gelman-Rubin test
                for (std::vector<std::string>::iterator it = failedGelman.begin(); it != failedGelman.end(); ++it)
                {
                    RBOUT("\t\tThe parameter with name '" + *it + " failed the Gelman-Rubin test.");
                }
                
                // printing failures of convergence of the stationarity test
                for (std::vector<std::string>::iterator it = failedStationary.begin(); it != failedStationary.end(); ++it)
                {
                    RBOUT("\t\tThe parameter with name '" + *it + " failed the stationarity test.");
                }
                
                // printing failures of convergence of the ESS test
                for (std::vector<std::string>::iterator it = failedESS.begin(); it != failedESS.end(); ++it)
                {
                    RBOUT("\t\tThe parameter with name '" + *it + " failed the ESS test.");
                }
                
                // printing failures of convergence of the Heidelberger-Welch test
                for (std::vector<std::string>::iterator it = failedHeidelberger.begin(); it != failedHeidelberger.end(); ++it)
                {
                    RBOUT("\t\tThe parameter with name '" + *it + " failed the Heidelberger-Welch test.");
                }
                
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
const TypeSpec& BurninEstimationConvergenceAssessment::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& BurninEstimationConvergenceAssessment::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule("filename", true, RlString::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("delimitter", true, RlString::getClassTypeSpec(), new RlString("\t") ) );
        
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
        StringUtilities::stringSplit(line, delimitter, columns);
            
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
    else if ( name == "delimitter")
    {
        delimitter = static_cast<const RlString&>( var->getRevObject() ).getValue();
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
    
}
