#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BranchRateJumpProcess.h"
#include "Dist_branchRateJumpProcess.h"
#include "ModelVector.h"
#include "Probability.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_branchRateJumpProcess::Dist_branchRateJumpProcess( void ) : TypedDistribution< ModelVector<RealPos> >(),
    valueDistribution( NULL ),
    tree( NULL ),
    lambda( NULL ),
    instantaneousJumpProbability( NULL )
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
RevBayesCore::BranchRateJumpProcess* Dist_branchRateJumpProcess::createDistribution( void ) const 
{
    
    // get the parameters

    // the value distribution
    const Distribution& rlDistribution                      = static_cast<const Distribution &>( valueDistribution->getRevObject() );
    RevBayesCore::TypedDistribution<double>* vd             = static_cast<RevBayesCore::TypedDistribution<double>*>( rlDistribution.createDistribution() );    
    // the tree
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* t   = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    // the scaling factor of the branch lengths
    RevBayesCore::TypedDagNode<double>* l                   = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    // the instantaneousJumpProbability
    RevBayesCore::TypedDagNode<double>* rho                 = static_cast<const RealPos &>( instantaneousJumpProbability->getRevObject() ).getDagNode();
    
    // create the internal distribution object
    RevBayesCore::BranchRateJumpProcess*   d = new RevBayesCore::BranchRateJumpProcess(vd,t,l,rho);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
Dist_branchRateJumpProcess* Dist_branchRateJumpProcess::clone( void ) const 
{
    return new Dist_branchRateJumpProcess(*this);
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_branchRateJumpProcess::getClassType(void) 
{ 
    
    static std::string revType = "Dist_branchRateJumpProcess";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_branchRateJumpProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution< ModelVector<RealPos> >::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the branch rate jump process are:
 * (1) the distribution from which new values are drawn if a jump occurred.
 * (2) the time tree.
 * (3) the scale parameter for the branch lengths.
 * (4) the instantaneous jump probability.
 *
 * \return The member rules.
 */
const MemberRules& Dist_branchRateJumpProcess::getParameterRules(void) const 
{
    
    static MemberRules distDist_branchRateJumpProcessMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distDist_branchRateJumpProcessMemberRules.push_back( new ArgumentRule( "valueDistribution",  Distribution::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distDist_branchRateJumpProcessMemberRules.push_back( new ArgumentRule( "tree",               TimeTree::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distDist_branchRateJumpProcessMemberRules.push_back( new ArgumentRule( "lambda",             RealPos::getClassTypeSpec()     , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        distDist_branchRateJumpProcessMemberRules.push_back( new ArgumentRule( "rho",                Probability::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new Probability(0.0) ) );
        
        rulesSet = true;
    }
    
    return distDist_branchRateJumpProcessMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_branchRateJumpProcess::getTypeSpec( void ) const 
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
void Dist_branchRateJumpProcess::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) 
{
    
    if ( name == "tree" ) 
    {
        tree = var;
    }
    else if ( name == "valueDistribution" ) 
    {
        valueDistribution = var;
    }
    else if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "rho" ) 
    {
        instantaneousJumpProbability = var;
    }
    else 
    {
        TypedDistribution< ModelVector< RealPos > >::setConstParameter(name, var);
    }
    
}
