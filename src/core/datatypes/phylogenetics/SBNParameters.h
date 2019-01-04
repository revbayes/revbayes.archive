// /**
//  * @file
//  * This file contains the declaration of SBNParameters, which is
//  * class that holds the parameters for an SBN.
//  *
//  * @brief Declaration of SBNParameters
//  *
//  * (c) Copyright 2009-
//  * @date Last modified: $Date$
//  * @author The RevBayes Development Core Team
//  * @license GPL version 3
//  *
//  * $Id$
//  */
//
// #ifndef SBNParameters_H
// #define SBNParameters_H
//
// #include "Subsplit.h"
//
// #include <vector>
//
// namespace RevBayesCore {
//
//     class SBNParameters {
//
//     public:
//         SBNParameters(size_t n);                              //!< Constructor
//         SBNParameters(const SBNParameters &tpm);
//         virtual                            ~SBNParameters();
//
//
//         // overloaded operators
//         SBNParameters&        operator=(const SBNParameters& tpm);
//         double*                             operator[](size_t i);                                               //!< Subscript operator
//         const double*                       operator[](size_t i) const;                                         //!< Subscript operator (const)
//         SBNParameters&        operator*=(const SBNParameters& B);                   //!< Matrix-matrix multiply
//
// //        std::vector<std::vector<double> >::const_iterator       begin(void) const;
// //        std::vector<std::vector<double> >::iterator             begin(void);
// //        std::vector<std::vector<double> >::const_iterator       end(void) const;
// //        std::vector<std::vector<double> >::iterator             end(void);
//
//         size_t                              getNumberOfStates(void) const;
//         double                              getElement(size_t i, size_t j) const;
//         double&                             getElement(size_t i, size_t j);
//         double*                             getElements(void);
//         const double*                       getElements(void) const;
//         size_t                              size(void) const;
//
// //    private:
//
//         size_t                              num_states;                                                          //!< The number of character states
//         size_t                              nElements;
//         double*                             theMatrix;                                                          //!< Holds the transition probability matrix
//         size_t                                              num_taxa; // The number of taxa in the tree the SBN describes
//         std::vector<Taxon>                                  taxa; // The taxa in the tree the SBN describes
//         std::vector<std::pair<RbBitSet,RbBitSet> >          root_splits; // The root splits in the tree (as subsplits)
//         std::map<std::pair<RbBitSet,RbBitSet>,double>       root_split_probabilities;
//         RbVector<RbVector<RbVector<long> > >                subsplit_children; // This is a vector of vectors of vectors, where subsplit_children[i] is all children of subsplit[i], subsplit_children[i][0] is all subsplits of clade X of i, and subsplit_children[i][1][j] is a specific child of subsplit i's clade Y.
//         RbVector<RbVector<RbVector<double> > >              subsplit_probabilities; // For every parent-child subsplit pair recorded in subsplit_children, this is the probability of that transition
//         std::map<Subsplit,size_t>                           subsplit_bitset_map;
//         std::map<Subsplit,double>                           root_split_probability_map; // For any root split, this tells us the probability of it
//         std::map<std::pair<size_t,size_t>,double>           subsplit_parent_child_probability_pair_map; // For any parent-child subsplit pair (i,j) (parent being i), this tells us the probability of the transition
//
//     };
//
//     // Global functions using the class
//     std::ostream&                       operator<<(std::ostream& o, const SBNParameters& x);                                           //!< Overloaded output operator
//
//
// }
//
// #endif
