#ifndef AbstractBirthDeathProcess_H
#define AbstractBirthDeathProcess_H

#include "Taxon.h"
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
    class AbstractBirthDeathProcess : public TypedDistribution<Tree> {
        
    public:
        AbstractBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const std::string &cdt,
                                  const std::vector<Taxon> &tn, const std::vector<Clade> &c);
        
        // pure virtual member functions
        virtual AbstractBirthDeathProcess*                  clone(void) const = 0;                                                                              //!< Create an independent clone
        
        
        // public member functions you may want to override
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        virtual void                                        redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)

        
    protected:
        // pure virtual helper functions
        virtual double                                      computeLnProbabilityTimes(void) const = 0;                                                          //!< Compute the log-transformed probability of the current value.
        virtual std::vector<double>*                        simSpeciations(size_t n, double origin) const = 0;                                                  //!< Simulate n speciation events.
        virtual double                                      simNextAge(size_t n, double start, double end) const;                                               //!< Simulate one speciation events.
        virtual double                                      pSurvival(double start, double end) const = 0;                                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        virtual void                                        prepareProbComputation(void);
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                        getAffected(std::set<DagNode *>& affected, DagNode* affecter);                                      //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        void                                                attachTimes(Tree *psi, std::vector<TopologyNode *> &tips, size_t index,
                                                                        const std::vector<double> *times, double T);
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        std::vector<double>*                                divergenceTimesSinceOrigin(void) const;                                                             //!< Extract the divergence times from the tree.
        int                                                 diversity(double t) const;                                                                          //!< Diversity at time t.
        std::vector<double>*                                getAgesOfInternalNodesFromMostRecentSample(void) const;                                             //!< Get the ages of all internal nodes since the time of the most recent tip age.
        std::vector<double>*                                getAgesOfTipsFromMostRecentSample(void) const;                                                      //!< Get the ages of all tip nodes since the time of the most recent tip age.
        bool                                                matchesConstraints(void);
        void                                                simulateTree(void);
        void                                                simulateClade(std::vector<TopologyNode*> &n, double age);                                           //!< Simulate n speciation events.
        
        // members
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        std::vector<Clade>                                  constraints;                                                                                        //!< Topological constrains.
        const TypedDagNode<double>*                         origin;                                                                                             //!< Time since the origin.
        const TypedDagNode<double>*                         rootAge;                                                                                            //!< Time since the origin.
        size_t                                              numTaxa;                                                                                            //!< Number of taxa (needed for correct initialization).
        std::vector<Taxon>                                  taxa;                                                                                               //!< Taxon names that will be attached to new simulated trees.
        bool                                                startsAtRoot;
        double                                              logTreeTopologyProb;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        
    };
    
}

#endif