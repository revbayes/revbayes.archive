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


/** Clone object */
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
