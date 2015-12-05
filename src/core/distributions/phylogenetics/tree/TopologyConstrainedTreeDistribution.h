#ifndef TopologyConstrainedTreeDistribution_H
#define TopologyConstrainedTreeDistribution_H

#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    class Clade;
    
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
    class TopologyConstrainedTreeDistribution : public TypedDistribution<Tree> {
        
    public:
        TopologyConstrainedTreeDistribution(TypedDistribution<Tree> *base_dist, const std::vector<Clade> &c);
        
        // pure virtual member functions
        virtual TopologyConstrainedTreeDistribution*        clone(void) const;                                                                              //!< Create an independent clone
        
        
        // public member functions you may want to override
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        virtual void                                        redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        
    protected:
        
//        // virtual methods that may be overwritten, but then the derived class should call this methods
//        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
//        virtual void                                        keepSpecialization(DagNode* affecter);
//        virtual void                                        restoreSpecialization(DagNode *restorer);
//        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        bool                                                matchesConstraints(void);
        
        // members
        TypedDistribution<Tree>*                            base_distribution;                                                                                        //!< Topological constrains.
        std::vector<Clade>                                  constraints;                                                                                        //!< Topological constrains.
        
    };
    
}

#endif