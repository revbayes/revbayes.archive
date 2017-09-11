#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_CharacterDependentBirthDeathProcess.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RateGenerator.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlDistributionMemberFunction.h"
#include "RlRateGenerator.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "StochasticNode.h"

using namespace RevLanguage;


Dist_CharacterDependentBirthDeathProcess::Dist_CharacterDependentBirthDeathProcess() : TypedDistribution<TimeTree>()
{
    
}


Dist_CharacterDependentBirthDeathProcess::~Dist_CharacterDependentBirthDeathProcess()
{
    
}



Dist_CharacterDependentBirthDeathProcess* Dist_CharacterDependentBirthDeathProcess::clone( void ) const
{
    
    return new Dist_CharacterDependentBirthDeathProcess( *this );
}


RevBayesCore::TypedDistribution<RevBayesCore::Tree>* Dist_CharacterDependentBirthDeathProcess::createDistribution( void ) const
{
    
    // Get the parameters
    RevBayesCore::TypedDagNode<double>* ra   = static_cast<const RealPos &>( root_age->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<long>*    rs   = static_cast<const Natural &>( root_state->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* sp  = static_cast<const ModelVector<RealPos> &>( speciation_rates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ex  = static_cast<const ModelVector<RealPos> &>( extinction_rates->getRevObject() ).getDagNode();

    // rate matrix
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* q      = NULL;
    if( event_rate_matrix->getRevObject() != RevNullObject::getInstance() )
    {
        q = static_cast<const RateGenerator &>( event_rate_matrix->getRevObject() ).getDagNode();
    }

    RevBayesCore::TypedDagNode<double>* r = NULL;
    if( event_rate->getRevObject() != RevNullObject::getInstance() )
    {
        r = static_cast<const RealPos &>( event_rate->getRevObject() ).getDagNode();
    }

    RevBayesCore::TypedDagNode<RevBayesCore::Simplex >* bf = NULL;
    if( root_frequencies->getRevObject() != RevNullObject::getInstance() )
    {
        bf = static_cast<const Simplex &>( root_frequencies->getRevObject() ).getDagNode();
    }

    RevBayesCore::TypedDagNode<double>* rh   = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    
    RevBayesCore::StateDependentSpeciationExtinctionProcess*   d = new RevBayesCore::StateDependentSpeciationExtinctionProcess( ra, ex, q, r, bf, rh, cond, t );
    d->setSpeciationRates( sp );
    
    // set the number of time slices for the numeric ODE
    double n = static_cast<const RealPos &>( num_time_slices->getRevObject() ).getValue();
    d->setNumberOfTimeSlices( n );
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* ps = NULL;
    if( psi->getRevObject() != RevNullObject::getInstance() )
    {
        ps  = static_cast<const ModelVector<RealPos> &>( psi->getRevObject() ).getDagNode();

        d->setSerialSamplingRates( ps );
    }

    return d;
}



/* Get Rev type of object */
const std::string& Dist_CharacterDependentBirthDeathProcess::getClassType(void)
{
    
    static std::string rev_type = "Dist_CharacterDependentBirthDeathProcess";
    
    return rev_type;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& Dist_CharacterDependentBirthDeathProcess::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_CharacterDependentBirthDeathProcess::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "CDBDP";
    
    return d_name;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_CharacterDependentBirthDeathProcess::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "CDSSBDP" );
    a_names.push_back( "CDFBDP" );
    a_names.push_back( "BirthDeathMultiRate" );

    return a_names;
}


MethodTable Dist_CharacterDependentBirthDeathProcess::getDistributionMethods( void ) const
{
    
    MethodTable methods = TypedDistribution<TimeTree>::getDistributionMethods();
    
    
    ArgumentRules* clampCharDataArgRules = new ArgumentRules();
    clampCharDataArgRules->push_back( new ArgumentRule( "value", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    //    methods.addFunction("clampAt", new DistributionMemberFunction<TimeTree,RealPos>(this, clampAtArgRules   ) );
    methods.addFunction( new MemberProcedure( "clampCharData", RlUtils::Void, clampCharDataArgRules ) );

    
    return methods;
}



/* Return member rules */
const MemberRules& Dist_CharacterDependentBirthDeathProcess::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        memberRules.push_back( new ArgumentRule( "rootAge"   , RealPos::getClassTypeSpec()              , "The age of the root."                        , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        std::vector<std::string> slabels;
        slabels.push_back("speciationRates");
        slabels.push_back("lambda");
        memberRules.push_back( new ArgumentRule( slabels     , ModelVector<RealPos>::getClassTypeSpec() , "The vector of speciation rates."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        std::vector<std::string> elabels;
        elabels.push_back("extinctionRates");
        elabels.push_back("mu");
        memberRules.push_back( new ArgumentRule( elabels     , ModelVector<RealPos>::getClassTypeSpec() , "The vector of extinction rates."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "psi"       , ModelVector<RealPos>::getClassTypeSpec() , "The vector of serial sampling rates."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "Q"         , RateGenerator::getClassTypeSpec()        , "The rate matrix of jumping between rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "delta"     , RealPos::getClassTypeSpec()              , "The rate-factor of jumping between rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY, new RealPos(1.0) ) );
        memberRules.push_back( new ArgumentRule( "pi"        , Simplex::getClassTypeSpec()              , "State frequencies at the root."              , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "rho"       , Probability::getClassTypeSpec()          , "The taxon sampling probability."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        memberRules.push_back( new OptionRule( "condition"    , new RlString("time"), optionsCondition, "The condition of the birth-death process." ) );
        
        memberRules.push_back( new ArgumentRule( "taxa"      , ModelVector<Taxon>::getClassTypeSpec()   , "The taxa used for simulation."               , ArgumentRule::BY_VALUE                , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "nTimeSlices",       RealPos::getClassTypeSpec(),              "The number of time slices for the numeric ODE.",           ArgumentRule::BY_VALUE                , ArgumentRule::ANY, new RealPos(500.0) ) );

        rules_set = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_CharacterDependentBirthDeathProcess::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void Dist_CharacterDependentBirthDeathProcess::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "rootAge" )
    {
        root_age = var;
    }
    else if ( name == "pi" )
    {
        root_frequencies = var;
    }
    else if ( name == "speciationRates" || name == "lambda" )
    {
        speciation_rates = var;
    }
    else if ( name == "extinctionRates" || name == "mu" )
    {
        extinction_rates = var;
    }
    else if ( name == "psi" )
    {
        psi = var;
    }
    else if ( name == "Q" )
    {
        event_rate_matrix = var;
    }
    else if ( name == "delta" )
    {
        event_rate = var;
    }
    else if ( name == "rho" )
    {
        rho = var;
    }
    else if ( name == "condition" )
    {
        condition = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else if ( name == "nTimeSlices" )
    {
        num_time_slices = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}

