#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ModelVector.h"
#include "Model.h"
#include "Natural.h"
#include "Mcmcmc.h"
#include "OptionRule.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevNullObject.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlMcmcmc.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

Mcmcmc::Mcmcmc() : MonteCarloAnalysis()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Mcmcmc* Mcmcmc::clone(void) const
{
    
    return new Mcmcmc(*this);
}


void Mcmcmc::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const WorkspaceVector<Monitor> &                        ws_vec_mntr = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Monitor>           mntr;
    for ( size_t i = 0; i < ws_vec_mntr.size(); ++i )
    {
        mntr.push_back( ws_vec_mntr[i].getValue() );
    }
    const WorkspaceVector<Move>&                            ws_vec_mvs = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Move>              mvs;
    for ( size_t i = 0; i < ws_vec_mvs.size(); ++i )
    {
        mvs.push_back( ws_vec_mvs[i].getValue() );
    }
    const std::string &                                     sched   = static_cast<const RlString &>( moveschedule->getRevObject() ).getValue();
    int                                                     nchains = static_cast<const Natural &>( num_chains->getRevObject() ).getValue();
    int                                                     si      = static_cast<const Natural &>( swap_interval->getRevObject() ).getValue();
    double                                                  delta   = static_cast<const RealPos &>( delta_heat->getRevObject() ).getValue();
    int                                                     nreps   = static_cast<const Natural &>( num_runs->getRevObject() ).getValue();
    RevBayesCore::Mcmcmc *m = new RevBayesCore::Mcmcmc(mdl, mvs, mntr, sched, nchains, si, delta);
    
    value = new RevBayesCore::MonteCarloAnalysis(m,nreps);
    
}


/**
 * Get Rev type of object
 */
const std::string& Mcmcmc::getClassType(void)
{
    
    static std::string rev_type = "Mcmcmc";
    
    return rev_type;
}

/**
 * Get class type spec describing type of object
 */
const TypeSpec& Mcmcmc::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( MonteCarloAnalysis::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Mcmcmc::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "mcmcmc";
    
    return c_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Mcmcmc::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Michael Landis" );
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Mcmcmc::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    
    std::string description1 = "";
    description1 += "The Mcmcmc analysis object keeps a model and the associated moves and monitors.";
    description1 += "The object is used to run Metropolis Couped Markov chain Monte Carlo (Mcmcmc) simulation on";
    description1 += "the model, using the provided moves, to obtain a sample of the posterior probability";
    description1 += "distribution. During the analysis, the monitors are responsible for sampling model parameters of interest.";
    descriptions.push_back( description1 );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Mcmcmc::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    std::string details1 = "";
    details1 += "The Mcmcmc analysis object produced by a call to this function keeps copies of the model";
    details1 += "and the associated moves and monitors. The Mcmcmc analysis object is used to run Markov";
    details1 += "chain Monte Carlo (Mcmcmc) simulation on the model, using the provided moves,";
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
std::string Mcmcmc::getHelpExample(void) const
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
    example += "# Create an mcmcmc object\n";
    example += "myMcmcmcObject = mcmcmc( mymodel, monitors, moves, nchains=4, deltaHeat=5)\n";
    example += "\n";
    example += "# Run a short analysis\n";
    example += "myMcmcmcObject.burnin( generations = 400, tuningInterval = 100)\n";
    example += "myMcmcmcObject.run( generations = 400)\n";
    example += "\n";
    example += "# print the summary of the operators (now tuned)\n";
    example += "myMcmcmcObject.operatorSummary()\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Mcmcmc::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    RevBayesCore::RbHelpReference ref1;
    ref1.setCitation("Geyer,C.J. (1991) Markov chain Monte Carlo maximum likelihood. In Keramidas (ed.), Computing Science and Statistics: Proceedings of the 23rd Symposium on the Interface. Interface Foundation, Fairfax Station, pp. 156–163.");
    references.push_back( ref1 );
    
    RevBayesCore::RbHelpReference ref2;
    ref2.setCitation("Gilks,W.R. and Roberts,G.O. (1996) Strategies for improving MCMC. In Gilks,W.R., Richardson,S. and Spiegelhalter (eds) Markov chain Monte Carlo in Practice. Chapman&Hall, London, 89–114.");
    references.push_back( ref2 );
    
    RevBayesCore::RbHelpReference ref3;
    ref3.setCitation("Altekar, G.; Dwarkadas, S.; Huelsenbeck, J. P. & Ronquist, F. Parallel metropolis coupled Markov chain Monte Carlo for Bayesian phylogenetic inference Bioinformatics, Oxford Univ Press, 2004, 20, 407-415.");
    references.push_back( ref3 );
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Mcmcmc::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "mcmc" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Mcmcmc::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Metropolis-Coupled MCMC analysis object";
    
    return title;
}


/** Return member rules (no members) */
const MemberRules& Mcmcmc::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        // add the rules from the base class
        const MemberRules &parentRules = MonteCarloAnalysis::getParameterRules();
        memberRules.insert(memberRules.end(), parentRules.begin(), parentRules.end());
        
        memberRules.push_back( new ArgumentRule("nchains"    , Natural::getClassTypeSpec()  , "The number of chains to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(4) ) );
        memberRules.push_back( new ArgumentRule("swapInterval" , Natural::getClassTypeSpec(), "The interval at which swaps will be attempted.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10)) );
        memberRules.push_back( new ArgumentRule("deltaHeat"    , RealPos::getClassTypeSpec(), "The delta parameter for the heat function.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0.2) ) );

        
        rules_set = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& Mcmcmc::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get type spec */
void Mcmcmc::printValue(std::ostream &o) const
{
    
    o << "MetropolisCoupledMarkovChainMonteCarloAnalysis";
}


/** Set a member variable */
void Mcmcmc::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "nchains" )
    {
        num_chains = var;
    }
    else if ( name == "swapInterval" )
    {
        swap_interval = var;
    }
    else if ( name == "deltaHeat" )
    {
        delta_heat = var;
    }
    else
    {
        MonteCarloAnalysis::setConstParameter(name, var);
    }
}
