#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_empiricalTree.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDistributionMemberFunction.h"
#include "RlString.h"
#include "StochasticNode.h"
#include "EmpiricalTreeDistribution.h"
#include "RlTraceTree.h"

using namespace RevLanguage;


Dist_empiricalTree::Dist_empiricalTree() : TypedDistribution<Tree>()
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
    const RevBayesCore::TraceTree &tt = static_cast<const TraceTree &>( trace->getRevObject() ).getValue().getTreeTrace();
    // burnin
    int b = 0;

    RevObject& burn = burnin->getRevObject();
    if ( burn.isType( Integer::getClassTypeSpec() ) )
    {
        b = static_cast<const Integer &>(burn).getValue();
    }
    else
    {
        double burninFrac = static_cast<const Probability &>(burn).getValue();
        b = int( floor( tt.getSamples()*burninFrac ) );
    }
    // create the internal distribution object
    RevBayesCore::EmpiricalTreeDistribution*   d = new RevBayesCore::EmpiricalTreeDistribution( tt, b );
    
    return d;
}


/* Get Rev type of object */
const std::string& Dist_empiricalTree::getClassType(void)
{
    
    static std::string rev_type = "Dist_empiricalTree";
    
    return rev_type;
}


/* Get class type spec describing type of object.  */
const TypeSpec& Dist_empiricalTree::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Tree>::getClassTypeSpec() ) );
    
    return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_empiricalTree::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "EmpiricalTree";
    
    return d_name;
}


MethodTable Dist_empiricalTree::getDistributionMethods( void ) const
{
    
    MethodTable methods = TypedDistribution<Tree>::getDistributionMethods();
    
    // member functions
    ArgumentRules* get_tree_index_arg_rules = new ArgumentRules();
    methods.addFunction( new DistributionMemberFunction<Dist_empiricalTree, Natural >( "getTreeIndex", variable, get_tree_index_arg_rules   ) );
    
    
    return methods;
}


/* Return member rules */
const MemberRules& Dist_empiricalTree::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        memberRules.push_back( new ArgumentRule( "trace", TraceTree::getClassTypeSpec(), "The trace of tree samples.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        std::vector<TypeSpec> burninTypes;
        burninTypes.push_back( Probability::getClassTypeSpec() );
        burninTypes.push_back( Integer::getClassTypeSpec() );
        memberRules.push_back( new ArgumentRule( "burnin", burninTypes, "The fraction/number of samples to discard.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.25) ) );
        
        rules_set = true;
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
    
    if ( name == "trace" ) {
        trace = var;
    }
    else if ( name == "burnin" ) {
        burnin = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}
