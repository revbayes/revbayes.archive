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
        std::map<std::pair<Subsplit,Subsplit>,std::pair<double,double> >&   getEdgeLengthDistributionParameters(void); // For each subsplit parent-child relationship, the parameters of the edge length distribution
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
        void                                incrementParentChildCounts(std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts, Tree& tree, double &weight);
        void                                incrementRootSplitCounts(std::map<Subsplit,double>& root_split_counts, Tree& tree, double &weight);
        bool                                isValidCPD(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent) const;
        bool                                isValidRootDistribution(void) const;
        void                                normalizeCPDForSubsplit(std::vector<std::pair<Subsplit,double> >& cpd, Subsplit& parent);
        void                                makeCPDs(std::map<std::pair<Subsplit,Subsplit>,double>& parent_child_counts);
        void                                makeRootSplits(std::map<Subsplit,double>& root_split_counts);
        void                                learnRootedUnconstrainedSBN( std::vector<Tree>& trees );
        void                                learnTimeCalibratedSBN( std::vector<Tree>& trees );
        void                                learnUnconstrainedSBNSA( std::vector<Tree> &trees, double &alpha );
        void                                learnUnconstrainedSBNEM( std::vector<Tree> &trees, double &alpha );

      private:
        // members
        size_t                                         num_taxa; // The number of taxa in the tree the SBN describes
        std::vector<Taxon>                             taxa; // The taxa in the tree the SBN describes
        std::vector<std::pair<Subsplit,double> >       root_splits; // The root splits in the tree and their probabilities
        std::map<Subsplit,std::vector<std::pair<Subsplit,double> > >        subsplit_cpds; // For each subsplit, its children and their probabilities
        std::map<std::pair<Subsplit,Subsplit>,std::pair<double,double> >    edge_length_distribution_parameters; // For each subsplit parent-child relationship, the parameters of th edge length distribution
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const SBNParameters& x);                                           //!< Overloaded output operator


}

#endif
