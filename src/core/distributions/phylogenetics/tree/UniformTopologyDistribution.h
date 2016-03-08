#ifndef UniformTopologyDistribution_H
#define UniformTopologyDistribution_H

#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class UniformTopologyDistribution : public TypedDistribution<Tree> {
        
    public:
        UniformTopologyDistribution(const std::vector<Taxon> &ta, const std::vector<Clade> &c);
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
