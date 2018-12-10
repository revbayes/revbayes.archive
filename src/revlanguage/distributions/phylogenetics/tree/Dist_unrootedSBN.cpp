// #include "ArgumentRule.h"
// #include "ArgumentRules.h"
// #include "Dist_unrootedSBN.h"
// #include "ModelVector.h"
// #include "Natural.h"
// #include "Real.h"
// #include "RealPos.h"
// #include "RlString.h"
// #include "StochasticNode.h"
// #include "UnrootedSBN.h"
// #include "Clade.h"
// #include "RlClade.h"
// #include "RlTaxon.h"
// #include "Subsplit.h"
//
//
// using namespace RevLanguage;
//
// /**
//  * Default constructor.
//  *
//  * The default constructor does nothing except allocating the object.
//  */
// Dist_unrootedSBN::Dist_unrootedSBN() : TypedDistribution<BranchLengthTree>()
// {
//
// }
//
//
// /**
//  * The clone function is a convenience function to create proper copies of inherited objected.
//  * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
//  *
//  * \return A new copy of myself
//  */
// Dist_unrootedSBN* Dist_unrootedSBN::clone( void ) const
// {
//
//     return new Dist_unrootedSBN(*this);
// }
//
//
// /**
//  * Create a new internal distribution object.
//  *
//  * This function simply dynamically allocates a new internal distribution object that can be
//  * associated with the variable. The internal distribution object is created by calling its
//  * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the
//  * constructor. The distribution constructor takes care of the proper hook-ups.
//  *
//  * \return A new internal distribution object.
//  */
// RevBayesCore::UnrootedSBN* Dist_unrootedSBN::createDistribution( void ) const {
//
//     // get the taxa to simulate either from a vector of rev taxon objects or a vector of names
//     std::vector<RevBayesCore::Taxon>                     t  = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
//
//     std::vector<RevBayesCore::Subsplit>                  s  = static_cast<const ModelVector<RevBayesCore::Subsplit> &>( subsplits->getRevObject() ).getValue();
//
//     std::vector<size_t>                                  r  = static_cast<const ModelVector<Natural> &>( rootSplits->getRevObject() ).getValue();
//
//     RevBayesCore::TypedDagNode< RevBayesCore::Simplex >* rp = static_cast<const Simplex&>( rootSplitProbabilities->getRevObject() ).getDagNode();
//
//     std::vector<std::vector<std::vector<Natural> > >     c  = static_cast<const ModelVector<Taxon> &>( subsplitChildren->getRevObject() ).getValue();
//
//     std::vector<std::vector<std::vector<RevBayesCore::TypedDagNode< RevBayesCore::Simplex >*> > >     tp  = static_cast<const ModelVector<Taxon> &>( subsplitProbabilities->getRevObject() ).getValue();
//
// 	RevBayesCore::UnrootedSBN* d = new RevBayesCore::UnrootedSBN( t, s, r, rp, c, tp);
//
//
//   std::vector<Subsplit>                               subsplits; // Subsplits list should contain trivial subsplits, so the children of A|B should be A and B (and the child of A should be A)
//   std::vector<size_t>                                 root_splits;
//   std::vector<double>                                 root_split_probabilities;
//   std::vector<std::vector<std::vector<size_t> > >     subsplit_children; // This is a vector of vectors of vectors, where subsplit_children[i] is all children of subsplit[i], subsplit_children[i][0] is all subsplits of clade X of i, and subsplit_children[i][1][j] is a specific child of subsplit i's clade Y.
//   std::vector<std::vector<std::vector<double> > >     subsplit_probabilities; // For every parent-child subsplit pair recorded in subsplit_children, this is the probability of that transition
//
//     return d;
// }
//
//
// /**
//  * Get Rev type of object
//  *
//  * \return The class' name.
//  */
// const std::string& Dist_unrootedSBN::getClassType(void) {
//
//     static std::string rev_type = "Dist_unrootedSBN";
// 	return rev_type;
// }
//
//
// /**
//  * Get class type spec describing type of an object from this class (static).
//  *
//  * \return TypeSpec of this class.
//  */
// const TypeSpec& Dist_unrootedSBN::getClassTypeSpec(void) {
//
//     static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<BranchLengthTree>::getClassTypeSpec() ) );
// 	return rev_type_spec;
// }
//
//
// /**
//  * Get the Rev name for the distribution.
//  * This name is used for the constructor and the distribution functions,
//  * such as the density and random value function
//  *
//  * \return Rev name of constructor function.
//  */
// std::string Dist_unrootedSBN::getDistributionFunctionName( void ) const {
//
//     // create a distribution name variable that is the same for all instance of this class
//     std::string d_name = "UnrootedSBN";
//     return d_name;
// }
//
//
// /**
//  * Get the member rules used to create the constructor of this object.
//  *
//  * The member rules of the uniform topology distribution are:
//  * (1) the number of taxa.
//  * (2) the names of the taxa.
//  *
//  * \return The member rules.
//  */
// const MemberRules& Dist_unrootedSBN::getParameterRules(void) const {
//
//     static MemberRules memberRules;
//     static bool rules_set = false;
//
//     if ( !rules_set )
//         {
//         memberRules.push_back( new ArgumentRule( "taxa"                    , ModelVector<Taxon>::getClassTypeSpec()                              , "The vector of taxa that will be used for the tips.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
//         memberRules.push_back( new ArgumentRule( "subsplits"               , ModelVector<Subsplit>::getClassTypeSpec()                           , "The vector of subsplits that are in the SBN.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//         memberRules.push_back( new ArgumentRule( "rootSplits"              , ModelVector<Natural>::getClassTypeSpec()                            , "The root splits in the SBN (unrooted SBNs marginalize over these) as indices of subsplits in subsplits vector.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//         memberRules.push_back( new ArgumentRule( "rootSplitProbabilities"  , Simplex::getClassTypeSpec()                                         , "The probabilities of the root splits in the SBN.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//         memberRules.push_back( new ArgumentRule( "subsplitChildren"        , ModelVector<ModelVector<ModelVector<Natural> > >::getClassTypeSpec(), "The child splits of all subsplits (with left/right distinction) in the SBN as indices of subsplits in subsplits vector.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//         memberRules.push_back( new ArgumentRule( "subsplitProbabilities"   , ModelVector<ModelVector<Simplex> >::getClassTypeSpec()              , "The conditional probability distributions for parent-child pairs.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
//
//         rules_set = true;
//         }
//
//     return memberRules;
// }
//
// /**
//  * Get type-specification on this object (non-static).
//  *
//  * \return The type spec of this object.
//  */
// const TypeSpec& Dist_unrootedSBN::getTypeSpec( void ) const {
//
//     static TypeSpec ts = getClassTypeSpec();
//     return ts;
// }
//
//
//
// /**
//  * Set a member variable.
//  *
//  * Sets a member variable with the given name and store the pointer to the variable.
//  * The value of the variable might still change but this function needs to be called again if the pointer to
//  * the variable changes. The current values will be used to create the distribution object.
//  *
//  * \param[in]    name     Name of the member variable.
//  * \param[in]    var      Pointer to the variable.
//  */
// void Dist_unrootedSBN::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
//
//     if ( name == "taxa" )
//     {
//         taxa = var;
//     }
//     else if ( name == "subsplits" )
//     {
//         subsplits = var;
//     }
//     else if ( name == "rootSplits" )
//     {
//         root_splits = var;
//     }
//     else if ( name == "rootSplitProbabilities" )
//     {
//         root_split_probabilities = var;
//     }
//     else if ( name == "subsplitChildren" )
//     {
//         subsplit_children = var;
//     }
//     else if ( name == "subsplitProbabilities" )
//     {
//         subsplit_probabilities = var;
//     }
//     else
//     {
//         Distribution::setConstParameter(name, var);
//     }
// }
