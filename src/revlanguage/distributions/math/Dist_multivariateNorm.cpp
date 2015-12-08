#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_multivariateNorm.h"
#include "DistributionMemberFunction.h"
#include "MemberProcedure.h"
#include "MultivariateNormalDistribution.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlMatrixRealSymmetric.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_multivariateNorm::Dist_multivariateNorm(void) : TypedDistribution<ModelVector<Real> >()
{

    
    // member functions
    ArgumentRules* clampAtArgRules = new ArgumentRules();
    clampAtArgRules->push_back( new ArgumentRule( "index", Natural::getClassTypeSpec(), "The index of the value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    clampAtArgRules->push_back( new ArgumentRule( "value", Real::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
//    methods.addFunction("clampAt", new DistributionMemberFunction<TimeTree,RealPos>(this, clampAtArgRules   ) );
    methods.addFunction( new MemberProcedure( "clampAt", RlUtils::Void, clampAtArgRules ) );

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
RevBayesCore::MultivariateNormalDistribution* Dist_multivariateNorm::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* m = static_cast<const ModelVector<Real> &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* cov = NULL;
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixReal>* pre = NULL;
    RevBayesCore::TypedDagNode<double>* sc = static_cast<const RealPos &>( scale->getRevObject() ).getDagNode();
    
    if ( covariance->getRevObject() != RevNullObject::getInstance() && precision->getRevObject() != RevNullObject::getInstance() )
    {
        throw RbException("You can only provide a covariance matrix OR a precision matrix");
    }
    else if ( covariance->getRevObject() != RevNullObject::getInstance() )
    {
        cov = static_cast<const MatrixRealSymmetric &>( covariance->getRevObject() ).getDagNode();
    }
    else if ( precision->getRevObject() != RevNullObject::getInstance() )
    {
        pre = static_cast<const MatrixRealSymmetric &>( precision->getRevObject() ).getDagNode();
    }
    else
    {
        throw RbException("You need to provide a covariance matrix OR a precision matrix");
    }
    
    RevBayesCore::MultivariateNormalDistribution* d = new RevBayesCore::MultivariateNormalDistribution(m, cov, pre, sc);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
Dist_multivariateNorm* Dist_multivariateNorm::clone( void ) const
{
    
    return new Dist_multivariateNorm(*this);
}

/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_multivariateNorm::getClassType(void)
{ 
    
    static std::string revType = "Dist_multivariateNorm";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_multivariateNorm::getClassTypeSpec(void)
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<ModelVector<Real> >::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_multivariateNorm::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "MultivariateNormal";
    
    return d_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Dist_multivariateNorm::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Dist_multivariateNorm::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "A multivariate normal distribution." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Dist_multivariateNorm::getHelpDetails(void) const
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
std::string Dist_multivariateNorm::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
//    example += "p ~ dnBeta(1.0,1.0)\n";
//    example += "x ~ dnBernoulli(p)\n";
//    example += "x.clamp(1)\n";
//    example += "moves[1] = mvSlide(p, delta=0.1, weight=1.0)\n";
//    example += "monitors[1] = screenmonitor(printgen=1000, separator = "	", speciation)\n";
//    example += "mymodel = model(p)\n";
//    example += "mymcmc = mcmc(mymodel, monitors, moves)\n";
//    example += "mymcmc.burnin(generations=20000,tuningInterval=100)\n";
//    example += "mymcmc.run(generations=200000)\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Dist_multivariateNorm::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Dist_multivariateNorm::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "dnBinomial" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Dist_multivariateNorm::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Bernoulli Distribution";
    
    return title;
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
const MemberRules& Dist_multivariateNorm::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMemberRules.push_back( new ArgumentRule( "mean"      , ModelVector<Real>::getClassTypeSpec()  , "The vector of mean values.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY) );
        distMemberRules.push_back( new ArgumentRule( "covariance", MatrixRealSymmetric::getClassTypeSpec(), "The variance-covariance matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "precision" , MatrixRealSymmetric::getClassTypeSpec(), "The precision matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "scale"     , RealPos::getClassTypeSpec()            , "The scaling factor of the variance matrix.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_multivariateNorm::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_multivariateNorm::printValue(std::ostream& o) const
{
    
    o << " MVNorm(mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ", covariance=";
    if ( covariance != NULL )
    {
        o << covariance->getName();
    }
    else
    {
        o << "?";
    }
    o << ", precision=";
    if ( precision != NULL )
    {
        o << precision->getName();
    }
    else
    {
        o << "?";
    }
    o << ", scale=";
    if ( scale != NULL )
    {
        o << scale->getName();
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
void Dist_multivariateNorm::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "covariance" )
    {
        covariance = var;
    }
    else if ( name == "precision" )
    {
        precision = var;
    }
    else if ( name == "scale" )
    {
        scale = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
