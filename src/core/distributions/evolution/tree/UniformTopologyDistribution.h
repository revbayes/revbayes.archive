/**
 * @file
 * This file contains the declaration of the random variable class for constant rate birth-death process.
 * This class is derived from the stochastic node and each instance will represent a random variable.
 *
 * @brief Declaration of the constant rate Birth-Death process class.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-17, version 1.0
 * @interface TypedDagNode
 *
 * $Id:$
 */

#ifndef UniformTopologyDistribution_H
#define UniformTopologyDistribution_H

#include "Topology.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class UniformTopologyDistribution : public TypedDistribution<Topology> {
        
    public:
        UniformTopologyDistribution(size_t nTaxa, const std::vector<std::string> &tn);
        virtual                                            ~UniformTopologyDistribution(void);                                                                    //!< Virtual destructor
        
        // public member functions
        UniformTopologyDistribution*                        clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        void                                                simulateTree(void);
        
        // members
        size_t                                              numTaxa;
        std::vector<std::string>                            taxonNames;
        double                                              logTreeTopologyProb;
        
    };
    
}

#endif
