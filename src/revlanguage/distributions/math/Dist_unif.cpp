#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_unif.h"
#include "Real.h"
#include "RealPos.h"
#include "UniformDistribution.h"

using namespace RevLanguage;

Dist_unif::Dist_unif() : ContinuousDistribution()
{
    
}


Dist_unif::~Dist_unif()
{
    
}



Dist_unif* Dist_unif::clone( void ) const
{
    
    return new Dist_unif(*this);
}


RevBayesCore::UniformDistribution* Dist_unif::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const Real &>( lower->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const Real &>( upper->getRevObject() ).getDagNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_unif::getClassType(void)
{
    
    static std::string revType = "Dist_unif";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_unif::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the details for the constructor.
 */
std::vector<std::string> Dist_unif::getConstructorDetails(void) const
{

    std::vector<std::string> details;
    
    std::string d = "The uniform probability density is given by\n";
    d += "f(x) = 1 / (b-a)\n";
    d += "if x is within a and b and 0 otherwise.";
    details.push_back( d );
    
    return details;
}


/**
 * Get an exectuable example.
 */
std::string Dist_unif::getConstructorExample(void) const
{
    std::string example = "";
    
    example += "# draw some random numbers\n";
    example += "a <- runif(10,0,1)\n";
    example += "# compute the max\n";
    example += "max(a)\n";
    example += "\n";
    example += "# now let us create a random variable\n";
    example += "x ~ dnUniform(-10,10)\n";
    example += "# note that the type is a real number\n";
    example += "type(x)\n";
    example += "\n";
    example += "# now let us create another random variable\n";
    example += "y ~ dnUniform(0,10)\n";
    example += "# note that the type is a positive real number\n";
    example += "type(y)\n";
    example += "\n";
    example += "# now let us create yet another random variable\n";
    example += "z ~ dnUniform(0,1)\n";
    example += "# note that the type is a probability because it is between 0 and 1\n";
    example += "type(z)\n";
    
    return example;
}


/** 
 * Get the name of the person who implemented this function/distribution.
 */
std::string Dist_unif::getHelpAuthor(void) const
{
    std::string author = "Sebastian Hoehna";
    
    return author;
}


/**
 * Get a brief description of this object.
 */
std::vector<std::string> Dist_unif::getHelpDescription(void) const
{
    
    std::vector<std::string> description;
    
    std::string d = "Uniform probabability density.";
    description.push_back( d );
    
    return description;
}

/**
 * Get a vector of relevant references, if there are any.
 */
std::vector<RevBayesCore::RbHelpReference> Dist_unif::getHelpReferences(void) const
{
    
    std::vector<RevBayesCore::RbHelpReference> refs;
    
    return refs;
}


/**
 * Get a vector of other methods that might possibly be of interest too.
 */
std::vector<std::string> Dist_unif::getHelpSeeAlso(void) const
{
    
    std::vector<std::string> others;
    
    return others;
}


std::string Dist_unif::getHelpTitle(void) const
{
    std::string title = "Uniform distribution";
    
    return title;
}



/** Return member rules */
const MemberRules& Dist_unif::getParameterRules(void) const
{
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distUnifMemberRules.push_back( new ArgumentRule( "lower", Real::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper", Real::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& Dist_unif::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_unif::printValue(std::ostream& o) const
{
    
    o << " unif (lower=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ", upper=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_unif::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "lower" ) 
    {
        lower = var;
    }
    else if ( name == "upper" ) 
    {
        upper = var;
    }
    else 
    {
        ContinuousDistribution::setConstParameter(name, var);
    }
}
