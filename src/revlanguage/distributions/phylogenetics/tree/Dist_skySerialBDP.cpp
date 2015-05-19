#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_skySerialBDP.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "PiecewiseConstantSerialSampledBirthDeathProcess.h"
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
Dist_skySerialBDP::Dist_skySerialBDP() : TypedDistribution<TimeTree>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Dist_skySerialBDP* Dist_skySerialBDP::clone( void ) const 
{
    return new Dist_skySerialBDP(*this);
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
RevBayesCore::PiecewiseConstantSerialSampledBirthDeathProcess* Dist_skySerialBDP::createDistribution( void ) const 
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
    // speciation rates
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* s     = static_cast<const ModelVector<RealPos> &>( lambda->getRevObject() ).getDagNode();
    // speciation rate change times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* st    = static_cast<const ModelVector<RealPos> &>( lambdaTimes->getRevObject() ).getDagNode();
    // extinction rates
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* e     = static_cast<const ModelVector<RealPos> &>( mu->getRevObject() ).getDagNode();
    // extinction rate change times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* et    = static_cast<const ModelVector<RealPos> &>( muTimes->getRevObject() ).getDagNode();
    // sampling rates
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* p     = static_cast<const ModelVector<RealPos> &>( psi->getRevObject() ).getDagNode();
    // sampling rate change times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* pt    = static_cast<const ModelVector<RealPos> &>( psiTimes->getRevObject() ).getDagNode();
    // sampling probabilities
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* r     = static_cast<const ModelVector<RealPos> &>( rho->getRevObject() ).getDagNode();
    // sampling times
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rt    = static_cast<const ModelVector<RealPos> &>( rhoTimes->getRevObject() ).getDagNode();
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
    RevBayesCore::PiecewiseConstantSerialSampledBirthDeathProcess*   d = new RevBayesCore::PiecewiseConstantSerialSampledBirthDeathProcess(o, ra, s, st, e, et, p, pt, r, rt, tLastSample, cond, taxa, c);
    
    return d;
}



/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_skySerialBDP::getClassType( void ) 
{ 
    
    static std::string revType = "Dist_skySerialBDP";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_skySerialBDP::getClassTypeSpec( void ) 
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
const MemberRules& Dist_skySerialBDP::getParameterRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin"                , RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rootAge"               , RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda"                , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambdaTimes"           , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "mu"                    , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>( std::vector<double>(0.0) ) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "muTimes"               , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "psi"                   , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>( std::vector<double>(0.0) ) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "psiTimes"              , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"                   , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>(std::vector<double>(1.0)) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rhoTimes"              , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new ModelVector<RealPos>() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "timeSinceLastSample"   , RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        optionsCondition.push_back( "nTaxa" );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("survival"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , ModelVector<RlString>::getClassTypeSpec()              , ArgumentRule::BY_VALUE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"          , ModelVector<Clade>::getClassTypeSpec()   , ArgumentRule::BY_VALUE             , ArgumentRule::ANY, new ModelVector<Clade>() ) );
        
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
const TypeSpec& Dist_skySerialBDP::getTypeSpec( void ) const 
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
void Dist_skySerialBDP::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "lambdaTimes" ) 
    {
        lambdaTimes = var;
    }
    else if ( name == "mu" ) 
    {
        mu = var;
    }
    else if ( name == "muTimes" ) 
    {
        muTimes = var;
    }
    else if ( name == "psi" ) 
    {
        psi = var;
    }
    else if ( name == "psiTimes" ) 
    {
        psiTimes = var;
    }
    else if ( name == "rho" ) 
    {
        rho = var;
    }
    else if ( name == "rhoTimes" ) 
    {
        rhoTimes = var;
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
    else 
    {
        TypedDistribution<TimeTree>::setConstParameter(name, var);
    }
    
}
