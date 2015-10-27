#include "Func_Mcmcmc.h"
#include "Mcmcmc.h"
#include "MonteCarloAnalysis.h"
#include "OptionRule.h"
#include "RealPos.h"
#include "RlModel.h"
#include "RlMove.h"
#include "RlMonitor.h"
#include "RlMonteCarloAnalysis.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "WorkspaceVector.h"

using namespace RevLanguage;

/** default constructor */
Func_Mcmcmc::Func_Mcmcmc( void ) : Procedure()
{
    
}


/** Clone object */
Func_Mcmcmc* Func_Mcmcmc::clone( void ) const
{
    
    return new Func_Mcmcmc( *this );
}


RevPtr<RevVariable> Func_Mcmcmc::execute( void )
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
    int                                                     nchains = static_cast<const Natural &>( args[4].getVariable()->getRevObject() ).getValue();
    int                                                     si      = static_cast<const Natural &>( args[5].getVariable()->getRevObject() ).getValue();
    double                                                  delta   = static_cast<const RealPos &>( args[6].getVariable()->getRevObject() ).getValue();
    int                                                     nreps   = static_cast<const Natural &>( args[7].getVariable()->getRevObject() ).getValue();
    RevBayesCore::Mcmcmc *m = new RevBayesCore::Mcmcmc(mdl, mvs, mntr, sched, nchains, si, delta);
//    m->setScheduleType( sched );

    RevBayesCore::MonteCarloAnalysis *a = new RevBayesCore::MonteCarloAnalysis(m,nreps);
    
    return new RevVariable( new MonteCarloAnalysis( a ) );
}



/* Get argument rules */
const ArgumentRules& Func_Mcmcmc::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule("model"   , Model::getClassTypeSpec()                   , "The model graph.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule("monitors", WorkspaceVector<Monitor>::getClassTypeSpec(), "A vector of monitors for this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule("moves"   , WorkspaceVector<Move>::getClassTypeSpec()   , "A vector of moves that will be used in this analysis.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        std::vector<std::string> options;
        options.push_back( "sequential" );
        options.push_back( "random" );
        options.push_back( "single" );
        
        argumentRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options, "The method how to pick new moves per iteration/generation." ) );
        argumentRules.push_back( new ArgumentRule("nchains"    , Natural::getClassTypeSpec()                , "The number of chains to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(4) ) );
        argumentRules.push_back( new ArgumentRule("swapInterval" , Natural::getClassTypeSpec()              , "The interval at which swaps will be attempted.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100)) );
        argumentRules.push_back( new ArgumentRule("deltaHeat"    , RealPos::getClassTypeSpec()              , "The delta parameter for the heat function.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Real(0.2) ) );
        argumentRules.push_back( new ArgumentRule("nruns"   , Natural::getClassTypeSpec()                   , "The number of replicated analyses.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_Mcmcmc::getClassType(void)
{
    
    static std::string revType = "Func_Mcmcmc";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_Mcmcmc::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_Mcmcmc::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_Mcmcmc::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = MonteCarloAnalysis::getClassTypeSpec();
    return returnTypeSpec;
}
