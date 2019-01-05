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

#include <vector>

namespace RevBayesCore {

    class SBNParameters {

    public:
                                           SBNParameters();  //!< Constructor
                                           SBNParameters(const SBNParameters &sbn);
        virtual                            ~SBNParameters();


        // overloaded operators
        SBNParameters&        operator=(const SBNParameters& tpm);

        // Helper functions for SBN distributions
        double                              computeRootSplitProbability( const Subsplit root_split ) const;
        double                              computeSubsplitTransitionProbability( const Subsplit parent, const Subsplit child ) const;
        Subsplit                            drawRootSplit( void ) const;
        Subsplit                            drawSubsplitForY( Subsplit s ) const;
        Subsplit                            drawSubsplitForZ( Subsplit s ) const;

        // Functions for learning SBNs
        SBNParameters                       learnRootedUnconstrainedSBN( std::vector<Tree> ) const;
        SBNParameters                       learnTimeCalibratedSBN( std::vector<Tree> ) const;
        SBNParameters                       learnUnrootedSBN( std::vector<Tree>, double alpha=RbConstants::Double::nan ) const;

      private:
        // members
        size_t                                         num_taxa; // The number of taxa in the tree the SBN describes
        std::vector<Taxon>                             taxa; // The taxa in the tree the SBN describes
        std::vector<std::pair<Subsplit,double> >       root_splits; // The root splits in the tree and their probabilities
        std::map<Subsplit,std::vector<std::pair<Subsplit,double> > >   subsplit_cpds; // For each subsplit, its children and their probabilities
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const SBNParameters& x);                                           //!< Overloaded output operator


}

#endif
