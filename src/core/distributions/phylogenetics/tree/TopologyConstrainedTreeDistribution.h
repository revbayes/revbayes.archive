#ifndef TopologyConstrainedTreeDistribution_H
#define TopologyConstrainedTreeDistribution_H

#include "Clade.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    struct node_compare {
        bool operator() (TopologyNode* lhs, TopologyNode* rhs) const{
            return lhs->getName() < rhs->getName();
        }
    };
    struct clade_compare {
        bool operator() (const Clade& lhs, const Clade& rhs) const {
            return lhs.getNumberOfTaxa() < rhs.getNumberOfTaxa();
        }
    };
    
    /**
     * @file
     * This file contains the declaration of the random variable class for constant rate birth-death process.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class TopologyConstrainedTreeDistribution : public TypedDistribution<Tree>, TreeChangeEventListener {
        
    public:
        TopologyConstrainedTreeDistribution(TypedDistribution<Tree> *base_dist, const std::vector<Clade> &c, const TypedDagNode<Tree>* bb = NULL);
        TopologyConstrainedTreeDistribution(const TopologyConstrainedTreeDistribution &d);
        
        virtual ~TopologyConstrainedTreeDistribution(void);
        // pure virtual member functions
        virtual TopologyConstrainedTreeDistribution*        clone(void) const;                                                                                  //!< Create an independent clone
        
        
        // public member functions you may want to override
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        void                                                fireTreeChangeEvent(const TopologyNode &n, const unsigned& m=0);                                                 //!< The tree has changed and we want to know which part.
        virtual void                                        redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                        setStochasticNode(StochasticNode<Tree> *n);                                                         //!< Set the stochastic node holding this distribution
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        
    protected:
        
//        // virtual methods that may be overwritten, but then the derived class should call this methods
//        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        
        virtual void                                        initializeBitSets(void);
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        void                                                initializeBackbone(void);
        void                                                recursivelyBuildBackboneClades(const TopologyNode* n, std::map<const TopologyNode*, Clade>& m);
        void                                                recursivelyFlagNodesDirty(const TopologyNode& n);
        bool                                                matchesBackbone(void);
        bool                                                matchesConstraints(void);
        Tree*                                               simulateTree(void);
        
        // members
        TypedDistribution<Tree>*                            base_distribution;
        const TypedDagNode<Tree>*                           backbone_topology;
        std::map<const TopologyNode*, Clade>                backbone_clades;
        std::vector<Clade>                                  constraints;                                                                                              //!< Topological constrains.
        std::vector<bool>                                   dirty_nodes;
//        std::set<Clade, clade_compare>                      backbone_clades;

        
        
        // just for testing
        bool                                                owns_tree;
    };
    
}

#endif
