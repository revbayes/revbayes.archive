#include <math.h>
#include <stddef.h>
#include <ostream>
#include <string>
#include <vector>

#include "ArgumentRule.h"
#include "Dist_exponentialError.h"
#include "Natural.h"
#include "RealPos.h"
#include "StochasticNode.h"
#include "DistanceMatrix.h"
#include "ExponentialErrorDistribution.h"
#include "ArgumentRules.h"
#include "DagMemberFunction.h"
#include "DeterministicNode.h"
#include "DynamicNode.h"
#include "MatrixReal.h"
#include "ModelObject.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "RlDagMemberFunction.h"
#include "RlDeterministicNode.h"
#include "RlDistanceMatrix.h"
#include "RlDistribution.h"
#include "RlStochasticNode.h"
#include "RlTypedDistribution.h"
#include "RlTypedFunction.h"
#include "TypeSpec.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"
#include "TypedFunction.h"

namespace RevBayesCore { template <class valueType> class RbVector; }

using namespace RevLanguage;

/**
* Default constructor.
*
* The default constructor does nothing except allocating the object.
*/
Dist_exponentialError::Dist_exponentialError() : TypedDistribution<DistanceMatrix>()
{
    
}


/**
* Clone the object
*
* \return a clone of the object.
*/
Dist_exponentialError::~Dist_exponentialError()
{
    
}


Dist_exponentialError* Dist_exponentialError::clone( void ) const
{
    return new Dist_exponentialError(*this);
}


RevBayesCore::ExponentialErrorDistribution* Dist_exponentialError::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::DistanceMatrix>* averDM = static_cast<const DistanceMatrix &>( avgDistMat->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* la = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    
    RevBayesCore::ExponentialErrorDistribution* ee = new RevBayesCore::ExponentialErrorDistribution( averDM, la );
    
    return ee;
    
}


/* Get class name of object */
const std::string& Dist_exponentialError::getClassType(void) {
    
    static std::string revClassType = "Dist_exponentialError";
    
    return revClassType;
}


/* Get class type spec describing type of object */
const TypeSpec& Dist_exponentialError::getClassTypeSpec(void)
{
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
    return revClassTypeSpec;
}


/*
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Dist_exponentialError::getDistributionFunctionAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "expError" );
    
    return a_names;
}


/*
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_exponentialError::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "ExponentialError";
    
    return d_name;
}


/* Return member rules (no members) */
const MemberRules& Dist_exponentialError::getParameterRules(void) const
{
    
    static MemberRules dist_member_rules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        dist_member_rules.push_back( new ArgumentRule( "avgDistMat", DistanceMatrix::getClassTypeSpec(), "The distance matrix of the proposed supertree.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist_member_rules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), "The rate of the exponential distribution.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rules_set = true;
    }
    
    return dist_member_rules;
}


const TypeSpec& Dist_exponentialError::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/* Print value for user */
void Dist_exponentialError::printValue(std::ostream& o) const {
    
    o << " ExponentialError(";
    o << ")";
}


/* Set a member variable */
void Dist_exponentialError::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "avgDistMat" ) {
        avgDistMat = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
