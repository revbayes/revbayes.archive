#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_UnconstrainedSBN.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "Probability.h"
#include "RlString.h"
#include "StochasticNode.h"
#include "UnconstrainedSBN.h"
#include "Clade.h"
#include "RlClade.h"
#include "RlSubsplit.h"
#include "RlTaxon.h"


using namespace RevLanguage;

/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_UnconstrainedSBN::Dist_UnconstrainedSBN() : TypedDistribution<BranchLengthTree>()
{

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
Dist_UnconstrainedSBN* Dist_UnconstrainedSBN::clone( void ) const
{

    return new Dist_UnconstrainedSBN(*this);
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
RevBayesCore::UnconstrainedSBN* Dist_UnconstrainedSBN::createDistribution( void ) const {

    // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
    const std::vector<RevBayesCore::Taxon>                     t  = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();

    const std::vector<RevBayesCore::Subsplit>                  s  = static_cast<const ModelVector<Subsplit> &>( subsplits->getRevObject() ).getValue();

    const RevBayesCore::RbVector<long>                  r  = static_cast<const ModelVector<Natural> &>( rootSplits->getRevObject() ).getValue();

    const RevBayesCore::RbVector<double> rp       = static_cast<const ModelVector<RealPos> &>( rootSplitProbabilities->getRevObject() ).getValue();

    // std::vector<RealPos>                             rp = static_cast<const ModelVector<RealPos> &>( rootSplitProbabilities->getRevObject() ).getValue();

    const RevBayesCore::RbVector<RevBayesCore::RbVector<RevBayesCore::RbVector<long> > >  c  = static_cast<const ModelVector<ModelVector<ModelVector<Natural> > > &>( subsplitChildren->getRevObject() ).getValue();

    // std::vector<std::vector<std::vector<RealPos> > > tp = static_cast<const ModelVector< ModelVector<ModelVector<RealPos> > > &>( subsplitProbabilities->getRevObject() ).getValue();
    // RevBayesCore::TypedDagNode< RevBayesCore::RbVector<RevBayesCore::RbVector<RevBayesCore::RbVector<double> > > >* rp       = static_cast<const ModelVector< ModelVector<ModelVector<RealPos> > > &>( subsplitProbabilities->getRevObject() ).getDagNode();

    const RevBayesCore::RbVector<RevBayesCore::RbVector<RevBayesCore::RbVector<double> > > tp       = static_cast<const ModelVector<ModelVector<ModelVector<RealPos> > > &>( subsplitProbabilities->getRevObject() ).getValue();

    RevBayesCore::UnconstrainedSBN* d = new RevBayesCore::UnconstrainedSBN( t, s, r, rp, c, tp);

    return d;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_UnconstrainedSBN::getClassType(void) {

    static std::string rev_type = "Dist_UnconstrainedSBN";
	return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_UnconstrainedSBN::getClassTypeSpec(void) {

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<BranchLengthTree>::getClassTypeSpec() ) );
	return rev_type_spec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_UnconstrainedSBN::getDistributionFunctionName( void ) const {

    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "UnconstrainedSBN";
    return d_name;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the uniform topology distribution are:
 * (1) the number of taxa.
 * (2) the names of the taxa.
 *
 * \return The member rules.
 */
const MemberRules& Dist_UnconstrainedSBN::getParameterRules(void) const {

    static MemberRules memberRules;
    static bool rules_set = false;

    if ( !rules_set )
        {
        memberRules.push_back( new ArgumentRule( "taxa"                    , ModelVector<Taxon>::getClassTypeSpec()                              , "The vector of taxa that will be used for the tips.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "subsplits"               , ModelVector<Subsplit>::getClassTypeSpec()                           , "The vector of subsplits that are in the SBN.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "rootSplits"              , ModelVector<Natural>::getClassTypeSpec()                            , "The root splits in the SBN (unrooted SBNs marginalize over these) as indices of subsplits in subsplits vector.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "rootSplitProbabilities"  , ModelVector<Probability>::getClassTypeSpec()                        , "The probabilities of the root splits in the SBN.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "subsplitChildren"        , ModelVector<ModelVector<ModelVector<Natural> > >::getClassTypeSpec(), "The child splits of all subsplits (with left/right distinction) in the SBN as indices of subsplits in subsplits vector.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "subsplitProbabilities"   , ModelVector<ModelVector<ModelVector<Probability> > >::getClassTypeSpec() , "The conditional probability distributions for parent-child pairs.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );

        rules_set = true;
        }

    return memberRules;
}

/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_UnconstrainedSBN::getTypeSpec( void ) const {

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
void Dist_UnconstrainedSBN::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {

    if ( name == "taxa" )
    {
        taxa = var;
    }
    else if ( name == "subsplits" )
    {
        subsplits = var;
    }
    else if ( name == "rootSplits" )
    {
        rootSplits = var;
    }
    else if ( name == "rootSplitProbabilities" )
    {
        rootSplitProbabilities = var;
    }
    else if ( name == "subsplitChildren" )
    {
        subsplitChildren = var;
    }
    else if ( name == "subsplitProbabilities" )
    {
        subsplitProbabilities = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
