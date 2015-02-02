
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
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE ) );
    runArgRules->push_back( new ArgumentRule("underPrior" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );

    ArgumentRules* run2ArgRules = new ArgumentRules();
    run2ArgRules->push_back( new ArgumentRule("rules", WorkspaceVector<StoppingRule>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    run2ArgRules->push_back( new ArgumentRule("underPrior" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, run2ArgRules) );

    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );
    
    ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
    methods.addFunction("operatorSummary", new MemberProcedure( RlUtils::Void, operatorSummaryArgRules) );
    
}


/** Clone object */
MonteCarloAnalysis* MonteCarloAnalysis::clone(void) const {
    
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
        if ( args[0].getVariable()->getRevObject().isType( Natural::getClassTypeSpec() ) )
        {
            int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
            bool prior = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
            if ( prior == true )
            {
                RevBayesCore::RbVector<RevBayesCore::StoppingRule> rules;
                rules.push_back( RevBayesCore::MaxIterationStoppingRule(gen) );
                value->runPriorSampler( rules );
            }
            else
            {
                RevBayesCore::RbVector<RevBayesCore::StoppingRule> rules;
                rules.push_back( RevBayesCore::MaxIterationStoppingRule(gen) );
                value->run( rules );
            }
        }
        else
        {
            RevBayesCore::RbVector<RevBayesCore::StoppingRule> rules = static_cast<const WorkspaceVector<StoppingRule> &>( args[0].getVariable()->getRevObject() ).getVectorRbPointer();
            value->run( rules );
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
