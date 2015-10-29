#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_norm.h"
#include "NormalDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_norm::Dist_norm() : ContinuousDistribution()
{
    
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
RevBayesCore::NormalDistribution* Dist_norm::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<double>* m = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::NormalDistribution*   d = new RevBayesCore::NormalDistribution(m, s);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
Dist_norm* Dist_norm::clone( void ) const
{
    
    return new Dist_norm(*this);
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_norm::getClassType(void)
{ 
    
    static std::string revType = "Dist_norm";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_norm::getClassTypeSpec(void)
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_norm::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "Normal";
    
    return d_name;
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the branch rate jump process are:
 * (1) the mean of the distribution.
 * (2) the standard deviation.
 *
 * \return The member rules.
 */
const MemberRules& Dist_norm::getParameterRules(void) const
{
    
    static MemberRules distNormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distNormMemberRules.push_back( new ArgumentRule( "mean", Real::getClassTypeSpec()   , "The mean parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Real(0.0) ) );
        distNormMemberRules.push_back( new ArgumentRule( "sd"  , RealPos::getClassTypeSpec(), "The standard deviation parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
    
        rulesSet = true;
    }
    
    return distNormMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_norm::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_norm::printValue(std::ostream& o) const {
    
    o << " norm(mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL ) {
        o << sd->getName();
    } else {
        o << "?";
    }
    o << ")";
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
void Dist_norm::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "sd" ) 
    {
        sd = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
