// #ifndef Dist_unrootedSBN_H
// #define Dist_unrootedSBN_H
//
// #include "UnrootedSBN.h"
// #include "RlBranchLengthTree.h"
// #include "RlTypedDistribution.h"
//
// namespace RevLanguage {
//
//     /**
//      * The RevLanguage wrapper of the uniform topology distribution
//      *
//      * The RevLanguage wrapper of the uniform topology distribution connects
//      * the variables/parameters of the distribution and creates the internal UnrootedSBN object.
//      * Please read the UnrootedSBN.h for more info.
//      *
//      *
//      * @copyright Copyright 2009-
//      * @author The RevBayes Development Core Team (Sebastian Hoehna)
//      * @since 2014-01-26, version 1.0
//      *
//      */
//     class Dist_unrootedSBN : public TypedDistribution<BranchLengthTree> {
//
//     public:
//         Dist_unrootedSBN( void );                                                                                                                   //!< Default constructor
//
//         // Basic utility functions
//         Dist_unrootedSBN*                           clone(void) const;                                                                              //!< Clone the object
//         static const std::string&                       getClassType(void);                                                                             //!< Get Rev type
//         static const TypeSpec&                          getClassTypeSpec(void);                                                                         //!< Get class type spec
//         std::string                                     getDistributionFunctionName(void) const;                                                        //!< Get the Rev-name for this distribution.
//         const TypeSpec&                                 getTypeSpec(void) const;                                                                        //!< Get the type spec of the instance
//         const MemberRules&                              getParameterRules(void) const;                                                                  //!< Get member rules (const)
//
//
//         // Distribution functions you have to override
//         RevBayesCore::UnrootedSBN*      createDistribution(void) const;                                                                 //!< Create the internal distribution object
//
//     protected:
//
//         void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);               //!< Set member variable
//
//     private:
//
//         RevPtr<const RevVariable>                       taxa;                                                                                           //!< Taxon names
//         RevPtr<const RevVariable>                       subsplits; // Subsplits list should contain trivial subsplits, so the children of A|B should be A and B (and the child of A should be A)
//         RevPtr<const RevVariable>                       root_splits;
//         RevPtr<const RevVariable>                       root_split_probabilities;
//         RevPtr<const RevVariable>                       subsplit_children; // This is a vector of vectors of vectors, where subsplit_children[i] is all children of subsplit[i], subsplit_children[i][0] is all subsplits of clade X of i, and subsplit_children[i][1][j] is a specific child of subsplit i's clade Y.
//         RevPtr<const RevVariable>                       subsplit_probabilities; // For every parent-child subsplit pair recorded in subsplit_children, this is the probability of that transition
//     };
//
// }
//
// #endif
