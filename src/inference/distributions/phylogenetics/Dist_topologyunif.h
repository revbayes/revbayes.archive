/**
 * @file
 * This file contains the declaration of Dist_topologyunif, which holds
 * the parameters and functions related to a uniform distribution on
 * topologies (unrooted or rooted, binary or polytomous).
 *
 * @brief Declaration of Dist_topologyunif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_topologyunif_H
#define Dist_topologyunif_H

#include "DistributionDiscrete.h"
#include "Topology.h"
#include "Variable.h"

#include <ostream>
#include <string>

class TopologyNode;


class Dist_topologyunif: public DistributionDiscrete {

    public:
                                            Dist_topologyunif(void);                                            //!< Parser constructor

        // Basic utility functions
        Dist_topologyunif*                  clone(void) const;                                                  //!< Clone object

        // Discrete distribution functions
        size_t                              getNumberOfStates(void) const;                                      //!< Get number of states

    protected:
        // functions you have to override
        double                              lnPdfSingleValue(std::vector<size_t> &result_offsets) const;        //!< Ln probability density
        double                              pdfSingleValue(std::vector<size_t> &result_offsets) const;          //!< Probability density function
        void                                rvSingleValue(std::vector<size_t> &offsets);                        //!< Generate a random draw
        void                                setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
        void                                setInternalObservedValue(const RbValue<void*> &v);                  //!< Set the pointers to the observation (random variable) of the distribution.
    
    private:
        void                                buildRandomBinaryTree(std::vector<TopologyNode* >& tips, size_t numTaxa);    //!< Build a random binary tree recursively until we have numTaxa tips
        void                                calculateTopologyProb(void);                                        //!< Calculate and set probability member variables
        void                                calculateNumberOfStates(void);                                      //!< Calculate and set number of states member variables

        double                              topologyProb;                                                       //!< The probability of a topology
        double                              lnTopologyProb;                                                     //!< The log probability of a topology
        size_t                              numberOfStates;                                                     //!< The number of states for this topology distribution
        
        // values
        RbValue<bool*>                      binary;
        RbValue<std::vector<std::string>*>  names;
        RbValue<int*>                       nTaxa;
        RbValue<bool*>                      rooted;
        RbValue<Topology*>                  topology;
};

#endif

