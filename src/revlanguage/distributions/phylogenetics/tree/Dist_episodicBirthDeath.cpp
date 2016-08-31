#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "EpisodicBirthDeathProcess.h"
#include "Dist_episodicBirthDeath.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_episodicBirthDeath::Dist_episodicBirthDeath() : BirthDeathProcess()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_episodicBirthDeath* Dist_episodicBirthDeath::clone( void ) const
{
    return new Dist_episodicBirthDeath(*this);
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::EpisodicBirthDeathProcess* Dist_episodicBirthDeath::createDistribution( void ) const
{
    
    // get the parameters
    
    // the root age
    RevBayesCore::TypedDagNode<double>* ra = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
    
    // speciation rates
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* sr         = static_cast<const ModelVector<RealPos> &>( lambda_rates->getRevObject() ).getDagNode();
    // speciation times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* st         = static_cast<const ModelVector<RealPos> &>( lambda_times->getRevObject() ).getDagNode();
    // extinction rates
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er         = static_cast<const ModelVector<RealPos> &>( mu_rates->getRevObject() ).getDagNode();
    // extinction times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* et         = static_cast<const ModelVector<RealPos> &>( mu_times->getRevObject() ).getDagNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r                                   = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // sampling strategy
    const std::string &strategy                                             = static_cast<const RlString &>( samplingStrategy->getRevObject() ).getValue();
    // incompletely sampled clades
    std::vector<RevBayesCore::Clade> inc_clades;
    if ( incomplete_clades->getRevObject() != RevNullObject::getInstance() )
    {
       inc_clades = static_cast<const ModelVector<Clade> &>( incomplete_clades->getRevObject() ).getValue();
    }
    // condition
    const std::string& cond                                                 = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
    std::vector<RevBayesCore::Taxon> t                                      = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    
    
    
    // create the internal distribution object
    RevBayesCore::EpisodicBirthDeathProcess* d = new RevBayesCore::EpisodicBirthDeathProcess(ra, sr, st, er, et, r, strategy, inc_clades, cond, t);
    
    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_episodicBirthDeath::getClassType( void )
{
    
    static std::string revType = "Dist_episodicBirthDeath";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_episodicBirthDeath::getClassTypeSpec( void )
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_episodicBirthDeath::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "EBDP" );
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_episodicBirthDeath::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "EpisodicBirthDeath";
    
    return d_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the constant-rate birth-death process are:
 * (1) the speciation rate lambda which must be a positive real.
 * (2) the extinction rate mu that must be a positive real.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& Dist_episodicBirthDeath::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        distMemberRules.push_back( new ArgumentRule( "lambdaRates", ModelVector<RealPos>::getClassTypeSpec(), "The piecewise-constant speciation rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "lambdaTimes", ModelVector<RealPos>::getClassTypeSpec(), "The speciation rate change times.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        distMemberRules.push_back( new ArgumentRule( "muRates"    , ModelVector<RealPos>::getClassTypeSpec(), "The piecewise-constant extinction rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "muTimes"    , ModelVector<RealPos>::getClassTypeSpec(), "The constant extinction rate change times.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = BirthDeathProcess::getParameterRules();
        distMemberRules.insert(distMemberRules.end(), parentRules.begin(), parentRules.end());
        
        rules_set = true;
    }
    
    return distMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_episodicBirthDeath::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_episodicBirthDeath::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "lambdaRates" )
    {
        lambda_rates = var;
    }
    else if ( name == "muRates" )
    {
        mu_rates = var;
    }
    else if ( name == "lambdaTimes" )
    {
        lambda_times = var;
    }
    else if ( name == "muTimes" )
    {
        mu_times = var;
    }
    else
    {
        BirthDeathProcess::setConstParameter(name, var);
    }
    
}
