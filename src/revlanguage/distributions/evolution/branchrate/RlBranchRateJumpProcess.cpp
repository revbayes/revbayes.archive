#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBranchRateJumpProcess.h"
#include "BranchRateJumpProcess.h"
#include "Probability.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
BranchRateJumpProcess::BranchRateJumpProcess( void ) : TypedDistribution< Vector<RealPos> >(),
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
RevBayesCore::BranchRateJumpProcess* BranchRateJumpProcess::createDistribution( void ) const 
{
    
    // get the parameters

    // the value distribution
    const Distribution& rlDistribution                      = static_cast<const Distribution &>( valueDistribution->getValue() );
    RevBayesCore::TypedDistribution<double>* vd             = static_cast<RevBayesCore::TypedDistribution<double>*>( rlDistribution.createDistribution() );    
    // the tree
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* t   = static_cast<const TimeTree &>( tree->getValue() ).getValueNode();
    // the scaling factor of the branch lengths
    RevBayesCore::TypedDagNode<double>* l                   = static_cast<const RealPos &>( lambda->getValue() ).getValueNode();
    // the instantaneousJumpProbability
    RevBayesCore::TypedDagNode<double>* rho                 = static_cast<const RealPos &>( instantaneousJumpProbability->getValue() ).getValueNode();
    
    // create the internal distribution object
    RevBayesCore::BranchRateJumpProcess*   d = new RevBayesCore::BranchRateJumpProcess(vd,t,l,rho);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of the process. 
 */
BranchRateJumpProcess* BranchRateJumpProcess::clone( void ) const 
{
    return new BranchRateJumpProcess(*this);
}


/**
 * Get class name of object 
 *
 * \return The class' name.
 */
const std::string& BranchRateJumpProcess::getClassName(void) 
{ 
    
    static std::string rbClassName = "BranchRateJumpProcess";
    
	return rbClassName; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& BranchRateJumpProcess::getClassTypeSpec(void) 
{ 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution< Vector<RealPos> >::getClassTypeSpec() ) );
    
	return rbClass; 
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
const MemberRules& BranchRateJumpProcess::getMemberRules(void) const 
{
    
    static MemberRules distBranchRateJumpProcessMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distBranchRateJumpProcessMemberRules.push_back( new ArgumentRule( "valueDistribution",  true, Distribution::getClassTypeSpec() ) );
        distBranchRateJumpProcessMemberRules.push_back( new ArgumentRule( "tree",               true, TimeTree::getClassTypeSpec() ) );
        distBranchRateJumpProcessMemberRules.push_back( new ArgumentRule( "lambda",             true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        distBranchRateJumpProcessMemberRules.push_back( new ArgumentRule( "rho",                true, Probability::getClassTypeSpec(), new Probability(0.0) ) );
        
        rulesSet = true;
    }
    
    return distBranchRateJumpProcessMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& BranchRateJumpProcess::getTypeSpec( void ) const 
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
void BranchRateJumpProcess::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
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
        TypedDistribution< Vector< RealPos > >::setConstMemberVariable(name, var);
    }
    
}
