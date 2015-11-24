#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_constFBDP.h"
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
Dist_constFBDP::Dist_constFBDP() : BirthDeathProcess()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_constFBDP* Dist_constFBDP::clone( void ) const
{
    return new Dist_constFBDP(*this);
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
RevBayesCore::ConstantRateFossilizedBirthDeathProcess* Dist_constFBDP::createDistribution( void ) const
{
    
    // get the parameters

    // the origin
    RevBayesCore::TypedDagNode<double>* o                   = NULL;
    if ( origin != NULL && origin->getRevObject() != RevNullObject::getInstance() )
    {
        o = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
    }
    // the root age
    RevBayesCore::TypedDagNode<double>* ra                   = NULL;
    if ( rootAge != NULL && rootAge->getRevObject() != RevNullObject::getInstance() )
    {
        ra = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
    }
    // speciation rate
    RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    // extinction rate
    RevBayesCore::TypedDagNode<double>* e       = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
    // fossilization rate
    RevBayesCore::TypedDagNode<double>* p       = static_cast<const RealPos &>( psi->getRevObject() ).getDagNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
    std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();

    // create the internal distribution object
    RevBayesCore::ConstantRateFossilizedBirthDeathProcess* d = new RevBayesCore::ConstantRateFossilizedBirthDeathProcess(o, ra, s, e, p, r, cond, t);
    
    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_constFBDP::getClassType( void )
{
    
    static std::string revType = "Dist_constFBDP";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_constFBDP::getClassTypeSpec( void )
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_constFBDP::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "FBDP" );
    
    return a_names;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_constFBDP::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "FossilBirthDeath";
    
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
const MemberRules& Dist_constFBDP::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), "The constant speciation rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "mu"    , RealPos::getClassTypeSpec(), "The constant extinction rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        distMemberRules.push_back( new ArgumentRule( "psi"   , RealPos::getClassTypeSpec(), "The constant fossilization rate.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = BirthDeathProcess::getParameterRules();
        distMemberRules.insert(distMemberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_constFBDP::getTypeSpec( void ) const
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
void Dist_constFBDP::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "mu" )
    {
        mu = var;
    }
    else if ( name == "psi" )
    {
        psi = var;
    }
    else
    {
        BirthDeathProcess::setConstParameter(name, var);
    }
    
}
