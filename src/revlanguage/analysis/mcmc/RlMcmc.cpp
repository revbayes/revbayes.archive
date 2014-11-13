
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

Mcmc::Mcmc() : WorkspaceToCoreWrapperObject<RevBayesCore::Mcmc>()
{

    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec()  , ArgumentRule::BY_VALUE ) );
    runArgRules->push_back( new ArgumentRule("underPrior" , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
    
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );
    
    ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
    methods.addFunction("operatorSummary", new MemberProcedure( RlUtils::Void, operatorSummaryArgRules) );

}


/** Clone object */
Mcmc* Mcmc::clone(void) const {
    
	return new Mcmc(*this);
}


void Mcmc::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new MCMC object
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const RevBayesCore::RbVector<RevBayesCore::Move>&       mvs     = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() ).getVectorRbPointer();
    const RevBayesCore::RbVector<RevBayesCore::Monitor>&    mntr    = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() ).getVectorRbPointer();
    const std::string &                                     sched   = static_cast<const RlString &>( moveSchedule->getRevObject() ).getValue();
    value = new RevBayesCore::Mcmc(mdl, mvs, mntr);
    value->setScheduleType( sched );
}


/* Map calls to member methods */
RevPtr<RevVariable> Mcmc::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run") 
    {
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        bool prior = static_cast<const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();
        if ( prior == true )
        {
            value->runPriorSampler( gen );
        }
        else
        {
            value->run( gen );
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
        
        value->printOperatorSummary();
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Mcmc::getClassType(void) { 
    
    static std::string revType = "MCMC";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mcmc::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::Mcmc>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mcmc::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("model"   , Model::getClassTypeSpec()                   , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("monitors", WorkspaceVector<Monitor>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("moves"   , WorkspaceVector<Move>::getClassTypeSpec()   , ArgumentRule::BY_VALUE ) );

        std::vector<std::string> options;
        options.push_back( "sequential" );
        options.push_back( "random" );
        options.push_back( "single" );
        
        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Mcmc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mcmc::printValue(std::ostream &o) const {
    
    o << "Mcmc";
}


/** Set a member variable */
void Mcmc::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "model") {
        model = var;
    } 
    else if ( name == "moves") {
        moves = var;
    }
    else if ( name == "monitors") {
        monitors = var;
    }
    else if ( name == "moveschedule") {
        moveSchedule = var;
    }
    else {
        RevObject::setConstParameter(name, var);
    }
}
