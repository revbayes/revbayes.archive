#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "DirichletDistribution.h"
#include "Dist_dirichlet.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlSimplex.h"
#include "StochasticNode.h"

using namespace RevLanguage;

Dist_dirichlet::Dist_dirichlet() : TypedDistribution<Simplex>()
{
    
}


Dist_dirichlet::~Dist_dirichlet()
{
    
}



Dist_dirichlet* Dist_dirichlet::clone( void ) const
{
    return new Dist_dirichlet(*this);
}


RevBayesCore::DirichletDistribution* Dist_dirichlet::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* a = static_cast<const ModelVector<RealPos> &>( alpha->getRevObject() ).getDagNode();
    RevBayesCore::DirichletDistribution* d              = new RevBayesCore::DirichletDistribution( a );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_dirichlet::getClassType(void)
{
    
    static std::string revType = "Dist_dirichlet";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_dirichlet::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Simplex>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_dirichlet::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "dirichlet";
    
    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_dirichlet::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Dist_dirichlet::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "A Bernoulli-distributed random variable takes the value 1 with probability p and the value 0 with probability 1-p." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Dist_dirichlet::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Dist_dirichlet::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "p ~ dnBeta(1.0,1.0)\n";
    example += "x ~ dnBernoulli(p)\n";
    example += "x.clamp(1)\n";
    example += "moves[1] = mvSlide(p, delta=0.1, weight=1.0)\n";
    example += "monitors[1] = screenmonitor(printgen=1000, separator = "	", speciation)\n";
    example += "mymodel = model(p)\n";
    example += "mymcmc = mcmc(mymodel, monitors, moves)\n";
    example += "mymcmc.burnin(generations=20000,tuningInterval=100)\n";
    example += "mymcmc.run(generations=200000)\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_dirichlet::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_dirichlet::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "dnBinomial" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_dirichlet::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Bernoulli Distribution";
    
    return title;
}


/** Return member rules (no members) */
const MemberRules& Dist_dirichlet::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule( "alpha", ModelVector<RealPos>::getClassTypeSpec(), "The concentration parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_dirichlet::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_dirichlet::printValue(std::ostream& o) const
{
    
    o << " dirichlet(alpha=";
    if ( alpha != NULL )
    {
        o << alpha->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
    
}


/** Set a member variable */
void Dist_dirichlet::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "alpha" ) 
    {
        alpha = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
