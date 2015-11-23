
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "MaxIterationStoppingRule.h"
#include "MonteCarloAnalysis.h"
#include "Model.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlMonteCarloAnalysis.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlStoppingRule.h"
#include "RlString.h"
#include "StoppingRule.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

MonteCarloAnalysis::MonteCarloAnalysis(RevBayesCore::MonteCarloAnalysis *m) : WorkspaceToCoreWrapperObject<RevBayesCore::MonteCarloAnalysis>( m )
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), "The number of generations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    runArgRules->push_back( new ArgumentRule("rules", WorkspaceVector<StoppingRule>::getClassTypeSpec(), "The rules when to automatically stop the run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new WorkspaceVector<StoppingRule>() ) );
    runArgRules->push_back( new ArgumentRule("underPrior" , RlBoolean::getClassTypeSpec(), "Should we run this analysis under the prior only?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );

    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), "The number of generation to run this burnin simulation.", ArgumentRule::BY_VALUE, ArgumentRule::ANY  ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), "The interval when to update the tuning parameters of the moves.", ArgumentRule::BY_VALUE, ArgumentRule::ANY  ) );
    methods.addFunction( new MemberProcedure( "burnin", RlUtils::Void, burninArgRules) );
    
    ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "operatorSummary", RlUtils::Void, operatorSummaryArgRules) );
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
MonteCarloAnalysis* MonteCarloAnalysis::clone(void) const
{
    
    return new MonteCarloAnalysis(*this);
}


//void MonteCarloAnalysis::constructInternalObject( void )
//{
//    // we free the memory first
//    delete value;
//    
//    // now allocate a new MonteCarloAnalysis object
//    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
//    const RevBayesCore::RbVector<RevBayesCore::Move>&       mvs     = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() ).getVectorRbPointer();
//    const RevBayesCore::RbVector<RevBayesCore::Monitor>&    mntr    = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() ).getVectorRbPointer();
//    const std::string &                                     sched   = static_cast<const RlString &>( moveSchedule->getRevObject() ).getValue();
//    value = new RevBayesCore::MonteCarloAnalysis(mdl, mvs, mntr);
//    value->setScheduleType( sched );
//}


/* Map calls to member methods */
RevPtr<RevVariable> MonteCarloAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        found = true;
        
        // get the member with give index
        const WorkspaceVector<StoppingRule>& ws_vec = static_cast<const WorkspaceVector<StoppingRule> &>( args[1].getVariable()->getRevObject() );
        RevBayesCore::RbVector<RevBayesCore::StoppingRule> rules;
        for ( size_t i = 0; i < ws_vec.size(); ++i )
        {
            rules.push_back( ws_vec[i].getValue() );
        }

        int currentGen = int(value->getCurrentGeneration());
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue() + currentGen;
        rules.push_back( RevBayesCore::MaxIterationStoppingRule(gen) );
        
        bool prior = static_cast<const RlBoolean &>( args[2].getVariable()->getRevObject() ).getValue();
        if ( prior == true )
        {
            value->runPriorSampler( gen, rules );
        }
        else
        {
            value->run( gen, rules );
        }
        
        return NULL;
    }
    else if (name == "burnin")
    {
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( gen, tuningInterval );
        
        return NULL;
    }
    else if ( name == "operatorSummary")
    {
        found = true;
        
        value->printPerformanceSummary();
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& MonteCarloAnalysis::getClassType(void)
{
    
    static std::string revType = "MonteCarloAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& MonteCarloAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::MonteCarloAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



///** Return member rules (no members) */
//const MemberRules& MonteCarloAnalysis::getParameterRules(void) const {
//    
//    static MemberRules memberRules;
//    static bool rulesSet = false;
//    
//    if ( !rulesSet )
//    {
//        
//        memberRules.push_back( new ArgumentRule("model"   , Model::getClassTypeSpec()                   , ArgumentRule::BY_VALUE ) );
//        memberRules.push_back( new ArgumentRule("monitors", WorkspaceVector<Monitor>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
//        memberRules.push_back( new ArgumentRule("moves"   , WorkspaceVector<Move>::getClassTypeSpec()   , ArgumentRule::BY_VALUE ) );
//        
//        std::vector<std::string> options;
//        options.push_back( "sequential" );
//        options.push_back( "random" );
//        options.push_back( "single" );
//        
//        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options ) );
//        
//        rulesSet = true;
//    }
//    
//    return memberRules;
//}

/** Get type spec */
const TypeSpec& MonteCarloAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void MonteCarloAnalysis::printValue(std::ostream &o) const
{
    
    o << "MonteCarloAnalysis";
}


///** Set a member variable */
//void MonteCarloAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
//{
//    
//    RevObject::setConstParameter(name, var);
//    
//}
