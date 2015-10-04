#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_empiricalTree.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlString.h"
#include "StochasticNode.h"
#include "EmpiricalTreeDistribution.h"
#include "RlTreeTrace.h"

using namespace RevLanguage;


Dist_empiricalTree::Dist_empiricalTree() : TypedDistribution()
{
    
}


Dist_empiricalTree::~Dist_empiricalTree()
{
    
}


Dist_empiricalTree* Dist_empiricalTree::clone( void ) const
{
    
    return new Dist_empiricalTree( *this );
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
RevBayesCore::EmpiricalTreeDistribution* Dist_empiricalTree::createDistribution( void ) const
{
    
    // get the parameters
    
    // tree trace
    const RevBayesCore::TreeTrace &tt = static_cast<const TreeTrace &>( trace->getRevObject() ).getValue();
    // burnin
    int b = static_cast<const Natural &>( burnin->getRevObject() ).getDagNode()->getValue();
    
    // create the internal distribution object
    RevBayesCore::EmpiricalTreeDistribution*   d = new RevBayesCore::EmpiricalTreeDistribution( tt, b );
    
    return d;
}


/* Get Rev type of object */
const std::string& Dist_empiricalTree::getClassType(void)
{
    
    static std::string revType = "Dist_empiricalTree";
    
    return revType;
}


/* Get class type spec describing type of object.  */
const TypeSpec& Dist_empiricalTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/* Return member rules */
const MemberRules& Dist_empiricalTree::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "burnin", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule( "treetrace", TreeTrace::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_empiricalTree::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void Dist_empiricalTree::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "treetrace" ) {
        trace = var;
    }
    else if ( name == "burnin" ) {
        burnin = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
