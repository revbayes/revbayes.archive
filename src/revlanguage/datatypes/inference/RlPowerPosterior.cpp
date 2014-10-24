
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "DistributionBeta.h"
#include "ModelVector.h"
#include "Model.h"
#include "Natural.h"
#include "PowerPosteriorMcmc.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevNullObject.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlPowerPosterior.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

PowerPosterior::PowerPosterior() : WorkspaceToCoreWrapperObject<RevBayesCore::PowerPosteriorMcmc>()
{

    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
    
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );

}


/** Clone object */
PowerPosterior* PowerPosterior::clone(void) const {
    
	return new PowerPosterior(*this);
}


void PowerPosterior::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::Model&                  mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const WorkspaceVector<Move>&                rlmvs   = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Move>  mvs     = rlmvs.getVectorRbPointer();
    const std::string&                          fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const double                                alpha   = static_cast<const RealPos &>( alphaVal->getRevObject() ).getValue();
    const int                                   sf      = static_cast<const Natural &>( sampFreq->getRevObject() ).getValue();

    value = new RevBayesCore::PowerPosteriorMcmc(mdl, mvs, fn);
    
    std::vector<double> beta;
    if ( powers->getRevObject() != RevNullObject::getInstance() )
    {
        beta = static_cast<const ModelVector<RealPos> &>( powers->getRevObject() ).getValue();
    }
    else
    {
        int k = static_cast<const Natural &>( cats->getRevObject() ).getValue();
        for (int i = k; i >= 0; --i)
        {
            double b = RevBayesCore::RbStatistics::Beta::quantile(alpha,1.0,i / double(k));
            beta.push_back( b );
        }
    }
    
    value->setPowers( beta );
    value->setSampleFreq( sf );
}


/* Map calls to member methods */
RevPtr<Variable> PowerPosterior::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->run( size_t(gen) );
        
        return NULL;
    } 
    else if (name == "burnin")
    {
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( size_t(gen), size_t(tuningInterval) );
        
        return NULL;
    } 
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& PowerPosterior::getClassType(void) { 
    
    static std::string revType = "PowerPosterior";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& PowerPosterior::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PowerPosteriorMcmc>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& PowerPosterior::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("model"      , Model::getClassTypeSpec()                , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("moves"      , WorkspaceVector<Move>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("filename"   , RlString::getClassTypeSpec()             , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("powers"     , ModelVector<RealPos>::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("cats"       , Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100) ) );
        memberRules.push_back( new ArgumentRule("alpha"      , RealPos::getClassTypeSpec()              , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0.3) ) );
        memberRules.push_back( new ArgumentRule("sample_freq", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100) ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& PowerPosterior::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PowerPosterior::printValue(std::ostream &o) const {
    
    o << "PowerPosterior";
}


/** Set a member variable */
void PowerPosterior::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "model")
    {
        model = var;
    } 
    else if ( name == "moves")
    {
        moves = var;
    }
    else if ( name == "filename")
    {
        filename = var;
    }
    else if ( name == "cats")
    {
        cats = var;
    }
    else if ( name == "powers")
    {
        powers = var;
    }
    else if ( name == "alpha")
    {
        alphaVal = var;
    }
    else if ( name == "sample_freq")
    {
        sampFreq = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}
