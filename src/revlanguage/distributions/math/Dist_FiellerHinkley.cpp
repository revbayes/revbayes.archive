#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_FiellerHinkley.h"
#include "FiellerHinkleyDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_FiellerHinkley::Dist_FiellerHinkley() : ContinuousDistribution(){
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_FiellerHinkley* Dist_FiellerHinkley::clone( void ) const
{
    
    return new Dist_FiellerHinkley(*this);
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
RevBayesCore::FiellerHinkleyDistribution* Dist_FiellerHinkley::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<double>* m1 = static_cast<const Real &>( mu1->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s1 = static_cast<const RealPos &>( sd1->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* m2 = static_cast<const Real &>( mu2->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s2 = static_cast<const RealPos &>( sd2->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* r = static_cast<const Real &>( rho->getRevObject() ).getDagNode();
    RevBayesCore::FiellerHinkleyDistribution*   d = new RevBayesCore::FiellerHinkleyDistribution(m1, s1, m2, s2, r);
    
    return d;
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_FiellerHinkley::getClassType(void)
{ 
    
    static std::string rev_type = "Dist_FiellerHinkley";
    
	return rev_type; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_FiellerHinkley::getClassTypeSpec(void)
{ 
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
 std::vector<std::string> Dist_FiellerHinkley::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "normalRatio" );
    
    return a_names;
}

/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_FiellerHinkley::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "FiellerHinkley";
    
    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_FiellerHinkley::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Andrew Magee" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Dist_FiellerHinkley::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Fieller-Hinkley distribution describes the distribution of the ratio of two normal random variables." );
    
    return descriptions;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_FiellerHinkley::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_FiellerHinkley::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "dnNormal" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_FiellerHinkley::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Fieller-Hinkley Distribution";
    
    return title;
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the Fieller-Hinkley distribution are:
 * (1) the mean of the first distribution.
 * (2) the standard deviation of the first distribution.
 * (3) the mean of the second distribution.
 * (4) the standard deviation of the second distribution.
 * (5) the correlation between the distributions.
 *
 * \return The member rules.
 */
const MemberRules& Dist_FiellerHinkley::getParameterRules(void) const
{
    
    static MemberRules distFiellerHinkleyMemberRules;
    static bool rules_set = false;
    
    if ( !rules_set ) 
    {
        distFiellerHinkleyMemberRules.push_back( new ArgumentRule( "mu1", Real::getClassTypeSpec()   , "Mean of numerator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distFiellerHinkleyMemberRules.push_back( new ArgumentRule( "sd1"  , RealPos::getClassTypeSpec(), "Standard deviation of numerator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distFiellerHinkleyMemberRules.push_back( new ArgumentRule( "mu2", Real::getClassTypeSpec()   , "Mean of denominator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distFiellerHinkleyMemberRules.push_back( new ArgumentRule( "sd2"  , RealPos::getClassTypeSpec(), "Standard deviation of denominator.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distFiellerHinkleyMemberRules.push_back( new ArgumentRule( "rho", Real::getClassTypeSpec()   , "Correlation between distributions.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    
        rules_set = true;
    }
    
    return distFiellerHinkleyMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_FiellerHinkley::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_FiellerHinkley::printValue(std::ostream& o) const
{
    
    o << " FiellerHinkley(mu1=";
    if ( mu1 != NULL )
    {
        o << mu1->getName();
    }
    else
    {
        o << "?";
    }
    o << ", sd1=";
    if ( sd1 != NULL )
    {
        o << sd1->getName();
    }
    else
    {
        o << "?";
    }
    o << ", mu2=";
    if ( mu2 != NULL )
    {
        o << mu2->getName();
    }
    else
    {
        o << "?";
    }
    o << ", sd2=";
    if ( sd2 != NULL )
    {
        o << sd2->getName();
    }
    else
    {
        o << "?";
    }
    o << ", rho=";
    if ( rho != NULL )
    {
        o << rho->getName();
    }
    else
    {
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
void Dist_FiellerHinkley::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "mu1" )
    {
        mu1 = var;
    }
    else if ( name == "sd1" )
    {
        sd1 = var;
    }
    else if ( name == "mu2" )
    {
        mu2 = var;
    }
    else if ( name == "sd2" )
    {
        sd2 = var;
    }
    else if ( name == "rho" )
    {
        rho = var;
    }
    else
    {
        ContinuousDistribution::setConstParameter(name, var);
    }
}
