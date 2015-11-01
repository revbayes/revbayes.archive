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

#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class UniformTopologyDistribution : public TypedDistribution<Tree> {
        
    public:
        UniformTopologyDistribution(size_t nTaxa, const std::vector<Taxon> &ta, const std::vector<Clade> &c);
		virtual                                            ~UniformTopologyDistribution(void);                                                                    //!< Virtual destructor
        
        // public member functions
        UniformTopologyDistribution*                        clone(void) const;                                                                                  //!< Create an independent clone
        double                                              computeLnProbability(void);
        void                                                redrawValue(void);
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        void                                                simulateTree(void);
        bool                                                matchesConstraints(void);
        void                                                simulateClade(std::vector<TopologyNode*> &n, bool bifurcating);                                           //!< Simulate n speciation events.

        // members
        size_t                                              numTaxa;
        std::vector<Taxon>                                  taxa;
        std::vector<Clade>                                  constraints;
        double                                              logTreeTopologyProb;                                                 //!< Topological constrains.
    };
    
}

#endif
