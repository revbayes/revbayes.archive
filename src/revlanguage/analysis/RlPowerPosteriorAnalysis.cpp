
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "DistributionBeta.h"
#include "Mcmc.h"
#include "ModelVector.h"
#include "Model.h"
#include "Natural.h"
#include "PowerPosteriorAnalysis.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevNullObject.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlPowerPosteriorAnalysis.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

PowerPosteriorAnalysis::PowerPosteriorAnalysis() : WorkspaceToCoreWrapperObject<RevBayesCore::PowerPosteriorAnalysis>()
{

    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), "The number of generations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );

    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), "The number of generations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), "The frequency when the moves are tuned (usually between 50 and 1000).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
    methods.addFunction( new MemberProcedure( "burnin", RlUtils::Void, burninArgRules) );

}


/** Clone object */
PowerPosteriorAnalysis* PowerPosteriorAnalysis::clone(void) const
{

	return new PowerPosteriorAnalysis(*this);
}


void PowerPosteriorAnalysis::constructInternalObject( void )
{
    // we free the memory first
    delete value;

    // now allocate a new sliding move
    const RevBayesCore::Model&                      mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const WorkspaceVector<Move>&                    rlmvs   = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() );
    const WorkspaceVector<Monitor>&                 rlmntr  = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() );
    RevBayesCore::RbVector<RevBayesCore::Monitor>   mntr;
    for ( size_t i = 0; i < rlmntr.size(); ++i )
    {
        mntr.push_back( rlmntr[i].getValue() );
    }
    RevBayesCore::RbVector<RevBayesCore::Move>      mvs;
    for ( size_t i = 0; i < rlmvs.size(); ++i )
    {
        mvs.push_back( rlmvs[i].getValue() );
    }
    const std::string&                              fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const double                                    alpha   = static_cast<const RealPos &>( alphaVal->getRevObject() ).getValue();
    const int                                       sf      = static_cast<const Natural &>( sampFreq->getRevObject() ).getValue();

    RevBayesCore::Mcmc *m = new RevBayesCore::Mcmc(mdl, mvs, mntr);
    m->setScheduleType( "random" );

    value = new RevBayesCore::PowerPosteriorAnalysis( m, fn );

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
RevPtr<RevVariable> PowerPosteriorAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{

    if (name == "run")
    {

        found = true;

        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->runAll( size_t(gen) );

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
const std::string& PowerPosteriorAnalysis::getClassType(void)
{

    static std::string revType = "PowerPosteriorAnalysis";

	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& PowerPosteriorAnalysis::getClassTypeSpec(void)
{

    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PowerPosteriorAnalysis>::getClassTypeSpec() ) );

	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& PowerPosteriorAnalysis::getParameterRules(void) const
{

    static MemberRules memberRules;
    static bool rulesSet = false;

    if ( !rulesSet )
    {

        memberRules.push_back( new ArgumentRule("model"      , Model::getClassTypeSpec()                   , "The model graph.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("moves"      , WorkspaceVector<Move>::getClassTypeSpec()   , "The vector moves to use.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("monitors"   , WorkspaceVector<Monitor>::getClassTypeSpec(), "The monitors to call. Do not provide a screen monitor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("filename"   , RlString::getClassTypeSpec()                , "The name of the file for the likelihood samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule("powers"     , ModelVector<RealPos>::getClassTypeSpec()    , "A vector of powers.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RevNullObject() ) );
        memberRules.push_back( new ArgumentRule("cats"       , Natural::getClassTypeSpec()                 , "The number of categories if no powers are specified.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100) ) );
        memberRules.push_back( new ArgumentRule("alpha"      , RealPos::getClassTypeSpec()                 , "The alpha parameter of the beta distribution if no powers are specified.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos(0.2) ) );
        memberRules.push_back( new ArgumentRule("sampleFreq" , Natural::getClassTypeSpec()                 , "The sampling frequency of the likelihood values.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(100) ) );

        rulesSet = true;
    }

    return memberRules;
}


/** Get type spec */
const TypeSpec& PowerPosteriorAnalysis::getTypeSpec( void ) const
{

    static TypeSpec typeSpec = getClassTypeSpec();

    return typeSpec;
}


/** Get type spec */
void PowerPosteriorAnalysis::printValue(std::ostream &o) const
{

    o << "PowerPosterior";
}


/** Set a member variable */
void PowerPosteriorAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "model")
    {
        model = var;
    }
    else if ( name == "moves")
    {
        moves = var;
    }
    else if ( name == "monitors")
    {
        monitors = var;
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
    else if ( name == "sampleFreq")
    {
        sampFreq = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
    
}
