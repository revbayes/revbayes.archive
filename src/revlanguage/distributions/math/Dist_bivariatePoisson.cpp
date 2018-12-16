#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BivariatePoissonDistribution.h"
#include "Dist_bivariatePoisson.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RbVector.h"
#include "RlTypedDistribution.h"
#include "StochasticNode.h"

#include <vector>

using namespace RevLanguage;

Dist_bivariatePoisson::Dist_bivariatePoisson() : TypedDistribution< RbVector<int> >()
{

}


Dist_bivariatePoisson::~Dist_bivariatePoisson()
{

}



Dist_bivariatePoisson* Dist_bivariatePoisson::clone( void ) const
{
    return new Dist_bivariatePoisson(*this);
}


RevBayesCore::BivariatePoissonDistribution* Dist_bivariatePoisson::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<double>* theta0          = static_cast<const RealPos &>( theta0->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* theta1          = static_cast<const RealPos &>( theta1->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* theta2          = static_cast<const RealPos &>( theta2->getRevObject() ).getDagNode();

    RevBayesCore::BivariatePoissonDistribution* d              = new RevBayesCore::BivariatePoissonDistribution( theta0, theta1, theta2 );

    return d;
}



/* Get Rev type of object */
const std::string& Dist_bivariatePoisson::getClassType(void)
{

    static std::string rev_type = "Dist_bivariatePoisson";

	return rev_type;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_bivariatePoisson::getClassTypeSpec(void)
{

  static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< RbVector<int> >::getClassTypeSpec() ) );

	return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_bivariatePoisson::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "bivariatePoisson";

    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_bivariatePoisson::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Alexander Zarebski" );
    authors.push_back( "Sebastian Hoehna" );

    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::string Dist_bivariatePoisson::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::string description = "Bivariate Poisson";

    return description;
}


/**
 * Get the more detailed description of the function
 */
std::string Dist_bivariatePoisson::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::string details = "";

    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Dist_bivariatePoisson::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";

    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_bivariatePoisson::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;


    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_bivariatePoisson::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "vector" );


    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_bivariatePoisson::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Bivariate Poisson Distribution";

    return title;
}


/** Return member rules (no members) */
const MemberRules& Dist_bivariatePoisson::getParameterRules(void) const
{

    static MemberRules memberRules;
    static bool rules_set = false;

    if ( !rules_set )
    {
        memberRules.push_back( new ArgumentRule( "theta0", ModelVector<RealPos>::getClassTypeSpec(), "The common parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "theta1", ModelVector<RealPos>::getClassTypeSpec(), "The parameter for the first component.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "theta2", ModelVector<RealPos>::getClassTypeSpec(), "The parameter for the second component.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );

        rules_set = true;
    }

    return memberRules;
}


const TypeSpec& Dist_bivariatePoisson::getTypeSpec( void ) const
{

    static TypeSpec ts = getClassTypeSpec();

    return ts;
}


/** Print value for user */
void Dist_bivariatePoisson::printValue(std::ostream& o) const
{

    o << " bivariatePoisson(theta0=";
    if ( theta0 != NULL )
    {
        o << theta0->getName();
    }
    else
    {
        o << "?";
    }
    o << ", theta1=";
    if ( theta1 != NULL )
      {
        o << theta1->getName();
      }
    else
      {
        o << "?";
      }
    o << ", theta2=";
    if ( theta2 != NULL )
      {
        o << theta2->getName();
      }
    else
      {
        o << "?";
      }
    o << ")";

}


/** Set a member variable */
void Dist_bivariatePoisson::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

    if ( name == "theta0" )
    {
        theta0 = var;
    }
    else if ( name == "theta1" )
      {
        theta1 = var;
      }
    else if ( name == "theta2" )
      {
        theta2 = var;
      }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
