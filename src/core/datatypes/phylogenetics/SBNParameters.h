/**
 * @file
 * This file contains the declaration of SBNParameters, which is
 * class that holds the parameters for an SBN, and methods for use
 * with SBNs.
 *
 * @brief Declaration of SBNParameters
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SBNParameters_H
#define SBNParameters_H

#include "RbConstants.h"
#include "RbBitSet.h"
#include "Subsplit.h"
#include "TopologyNode.h"
#include "Tree.h"
#include "TypedDistribution.h"

#include <vector>

namespace RevBayesCore {

    class SBNParameters {

    public:
                                           SBNParameters();  //!< Constructor
                                           SBNParameters(std::vector<Taxon> taxa);  //!< Constructor
                                           SBNParameters(const SBNParameters &sbn);
        virtual                            ~SBNParameters();


        // overloaded operators
        SBNParameters&        operator=(const SBNParameters& sbn);

        // Access to members
        // std::map<RbBitSet,std::pair<double,double> >&   getEdgeLengthDistributionParameters(void); // For each subsplit parent-child relationship, the parameters of the edge length distribution
        std::map<RbBitSet,std::pair<std::vector<double>,std::vector<double> > >&   getEdgeLengthDistributionParameters(void); // For each subsplit parent-child relationship, the parameters of the edge length distribution
        const size_t                                                        getNumTaxa(void) const; // The number of taxa in the tree the SBN describes
        std::vector<std::pair<Subsplit,double> >&                           getRootSplits(void); // The root splits in the tree and their probabilities
        std::map<Subsplit,std::vector<std::pair<Subsplit,double> > >&       getSubsplitCPDs(void); // For each subsplit, its children and their probabilities
        std::vector<Taxon>&                                                 getTaxa(void); // The taxa in the tree the SBN describes
        const std::vector<Taxon>&                                           getTaxa(void) const; // The taxa in the tree the SBN describes

        // Helper functions for SBN distributions
        double                              computeRootSplitProbability( const Subsplit &root_split ) const;
        double                              computeSubsplitTransitionProbability( const Subsplit &parent, const Subsplit &child ) const;
        Subsplit                            drawRootSplit( void ) const;
        Subsplit                            drawSubsplitForY( const Subsplit &s ) const;
        Subsplit                            drawSubsplitForZ( const Subsplit &s ) const;
        bool                                isValid(void) const;

        // Functions for learning SBNs
        void                                countAllSubsplits(Tree& tree, std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts, std::map<Subsplit,double>& root_split_counts, std::map<Subsplit,double>& q, bool doSA);
        void                                fitBranchLengthDistributions(std::vector<Tree> &trees);
        bool                                isValidCPD(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent) const;
        bool                                isValidRootDistribution(void) const;
        void                                normalizeCPDForSubsplit(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent);
        void                                makeCPDs(std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts);
        void                                makeRootSplits(std::map<Subsplit,double>& root_split_counts);
        void                                learnRootedUnconstrainedSBN( std::vector<Tree>& trees );
        void                                learnTimeCalibratedSBN( std::vector<Tree>& trees );
        void                                learnUnconstrainedSBNSA( std::vector<Tree> &trees );
        void                                learnUnconstrainedSBNEM( std::vector<Tree> &trees, double &alpha );

        // Helper functions for learning SBNs
        void                                addTreeToAllParentChildCounts(std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts, Tree& tree, double &weight);
        void                                addTreeToAllRootSplitCounts(std::map<Subsplit,double>& root_split_counts, Tree& tree, double &weight);
        void                                incrementParentChildCount(std::map<std::pair<Subsplit,Subsplit>,double> &parent_child_counts, std::pair<Subsplit,Subsplit> &this_parent_child, double &weight);
        void                                incrementRootSplitCount(std::map<Subsplit,double>& root_split_counts, Subsplit &this_root_split, double &weight);
      private:
        // members
        size_t                                         num_taxa; // The number of taxa in the tree the SBN describes
        std::vector<Taxon>                             taxa; // The taxa in the tree the SBN describes
        std::vector<std::pair<Subsplit,double> >       root_splits; // The root splits in the tree and their probabilities
        std::map<Subsplit,std::vector<std::pair<Subsplit,double> > >        subsplit_cpds; // For each subsplit, its children and their probabilities
        // std::map<RbBitSet,std::pair<double,double> >    edge_length_distribution_parameters; // In a rooted unconstrained SBN, we learn branch lengths as functions of the clades they subtend. In an unrooted unconstrained SBN, we learn branch lengths as functions of the splits they represent. Either way, we can represnt this as a bitset
        std::map<RbBitSet,std::pair<std::vector<double>,std::vector<double> > >    edge_length_distribution_parameters; // In a rooted unconstrained SBN, we learn branch lengths as functions of the clades they subtend. In an unrooted unconstrained SBN, we learn branch lengths as functions of the splits they represent. Either way, we can represnt this as a bitset
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const SBNParameters& x);                                           //!< Overloaded output operator


}

#endif
