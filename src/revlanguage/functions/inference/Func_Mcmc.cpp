#include "Func_Mcmc.h"
#include "Mcmc.h"
#include "MonteCarloAnalysis.h"
#include "OptionRule.h"
#include "RlModel.h"
#include "RlMove.h"
#include "RlMonitor.h"
#include "RlMonteCarloAnalysis.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "WorkspaceVector.h"

using namespace RevLanguage;

/** default constructor */
Func_Mcmc::Func_Mcmc( void ) : Procedure()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_Mcmc* Func_Mcmc::clone( void ) const
{
    
    return new Func_Mcmc( *this );
}


RevPtr<RevVariable> Func_Mcmc::execute( void )
{
    
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( args[0].getVariable()->getRevObject() ).getValue();
    const WorkspaceVector<Monitor> &                        ws_vec_mntr = static_cast<const WorkspaceVector<Monitor> &>( args[1].getVariable()->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Monitor>           mntr;
    for ( size_t i = 0; i < ws_vec_mntr.size(); ++i )
    {
        mntr.push_back( ws_vec_mntr[i].getValue() );
    }
    const WorkspaceVector<Move>&                            ws_vec_mvs = static_cast<const WorkspaceVector<Move> &>( args[2].getVariable()->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Move>              mvs;
    for ( size_t i = 0; i < ws_vec_mvs.size(); ++i )
    {
        mvs.push_back( ws_vec_mvs[i].getValue() );
    }
    const std::string &                                     sched   = static_cast<const RlString &>( args[3].getVariable()->getRevObject() ).getValue();
    int                                                     nreps   = static_cast<const Natural &>( args[4].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Mcmc *m = new RevBayesCore::Mcmc(mdl, mvs, mntr);
    m->setScheduleType( sched );
    RevBayesCore::MonteCarloAnalysis *a = new RevBayesCore::MonteCarloAnalysis(m,nreps);
    
    return new RevVariable( new MonteCarloAnalysis( a ) );
}



/* Get argument rules */
const ArgumentRules& Func_Mcmc::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule("model"   , Model::getClassTypeSpec()                   , "The model graph.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule("monitors", WorkspaceVector<Monitor>::getClassTypeSpec(), "The monitors used for this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule("moves"   , WorkspaceVector<Move>::getClassTypeSpec()   , "The moves used for this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> options;
        options.push_back( "sequential" );
        options.push_back( "random" );
        options.push_back( "single" );
        
        argumentRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options, "The strategy how the moves are used." ) );
        argumentRules.push_back( new ArgumentRule("nruns"   , Natural::getClassTypeSpec()                   , "The number of replicate analyses.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_Mcmc::getClassType(void)
{
    
    static std::string revType = "Func_Mcmc";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_Mcmc::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_Mcmc::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "mcmc";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_Mcmc::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_Mcmc::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    
    std::string description1 = "";
    description1 += "The MCMC analysis object keeps a model and the associated moves and monitors.";
    description1 += "The object is used to run Markov chain Monte Carlo (MCMC) simulation on";
    description1 += "the model, using the provided moves, to obtain a sample of the posterior probability";
    description1 += "distribution. During the analysis, the monitors are responsible for sampling model parameters of interest.";
    descriptions.push_back( description1 );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_Mcmc::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    std::string details1 = "";
    details1 += "The MCMC analysis object produced by a call to this function keeps copies of the model";
    details1 += "and the associated moves and monitors. The MCMC analysis object is used to run Markov";
    details1 += "chain Monte Carlo (MCMC) simulation on the model, using the provided moves,";
    details1 += "to obtain a sample of the posterior probability distribution. During the analysis,";
    details1 += "the monitors are responsible for sampling model parameters of interest.";
    details.push_back( details1 );
    
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_Mcmc::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "# Create a simple model (unclamped)\n";
    example += "a ~ exponential(1)\n";
    example += "mymodel = model(a)\n";
    example += "\n";
    example += "# Create a move vector and a monitor vector\n";
    example += "moves[1] = mvScale(a, lambda=1.0, weight=1.0)\n";
    example += "monitors[1] = mnFile(a,\"output/out.log\")\n";
    example += "\n";
    example += "# Create an mcmc object\n";
    example += "mymcmcObject = mcmc( mymodel, monitors, moves)\n";
    example += "\n";
    example += "# Run a short analysis\n";
    example += "mymcmcObject.burnin( generations = 400, tuningInterval = 100)\n";
    example += "mymcmcObject.run( generations = 400)\n";
    example += "\n";
    example += "# print the summary of the operators (now tuned)\n";
    example += "mymcmcObject.operatorSummary()\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_Mcmc::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    RevBayesCore::RbHelpReference ref1;
    ref1.setCitation("Metropolis N, AW Rosenbluth, MN Rosenbluth, AH Teller, E Teller (1953). Equation of state calculations by fast computing machines. Journal of Chemical Physics, 21:1087-1092.");
    references.push_back( ref1 );
    
    RevBayesCore::RbHelpReference ref2;
    ref2.setCitation("Hastings WK (1970) Monte Carlo sampling methods using Markov chains and their applications. Biometrika, 57:97-109.");
    references.push_back( ref2 );

    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_Mcmc::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "mcmcmc" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_Mcmc::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Create an MCMC analysis object";
    
    return title;
}


const TypeSpec& Func_Mcmc::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_Mcmc::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = MonteCarloAnalysis::getClassTypeSpec();
    return returnTypeSpec;
}
