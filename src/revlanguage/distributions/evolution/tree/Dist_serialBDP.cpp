#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateSerialSampledBirthDeathProcess.h"
#include "Dist_serialBDP.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Taxon.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_serialBDP::Dist_serialBDP() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Dist_serialBDP* Dist_serialBDP::clone( void ) const 
{
    return new Dist_serialBDP(*this);
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
RevBayesCore::ConstantRateSerialSampledBirthDeathProcess* Dist_serialBDP::createDistribution( void ) const
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
    // sampling rate
    RevBayesCore::TypedDagNode<double>* p       = static_cast<const RealPos &>( psi->getRevObject() ).getDagNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // time between now and most recent sample
    double tLastSample                          = static_cast<const RealPos &>( tLast->getRevObject() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    // taxon names
    const std::vector<std::string> &names       = static_cast<const ModelVector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
    // clade constraints
    const std::vector<RevBayesCore::Clade> &c   = static_cast<const ModelVector<Clade> &>( constraints->getRevObject() ).getValue();
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i) 
    {
        taxa.push_back( RevBayesCore::Taxon( names[i] ) );
    }
    
    // create the internal distribution object
    RevBayesCore::ConstantRateSerialSampledBirthDeathProcess*   d = new RevBayesCore::ConstantRateSerialSampledBirthDeathProcess(o, ra, s, e, p, r, tLastSample, cond, taxa, c);
    
    return d;
}



/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_serialBDP::getClassType( void ) 
{ 
    
    static std::string revType = "Dist_serialBDP";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_serialBDP::getClassTypeSpec( void ) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the diversity-dependent pure-birth process are:
 * (1) the initial speciation rate lambda which must be a positive real.
 * (2) the carrying capacity that must be a natural number.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& Dist_serialBDP::getParameterRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin" , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rootAge", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda" , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "mu"     , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "psi"    , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"    , Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "timeSinceLastSample", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        optionsCondition.push_back( "nTaxa" );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints", ModelVector<Clade>::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new ModelVector<Clade>() ) );
        
        // add the rules from the base class
        const MemberRules &parentRules = TypedDistribution<TimeTree>::getParameterRules();
        distcBirthDeathMemberRules.insert(distcBirthDeathMemberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_serialBDP::getTypeSpec( void ) const 
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
void Dist_serialBDP::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) 
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
    else if ( name == "rho" ) 
    {
        rho = var;
    }
    else if ( name == "timeSinceLastSample" ) 
    {
        tLast = var;
    }
    else if ( name == "origin" ) 
    {
        origin = var;
    }
    else if ( name == "rootAge" )
    {
        rootAge = var;
    }
    else if ( name == "names" ) 
    {
        taxonNames = var;
    }
    else if ( name == "constraints" ) 
    {
        constraints = var;
    }
    else if ( name == "condition" ) 
    {
        condition = var;
    }
    else {
        TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
    
}
