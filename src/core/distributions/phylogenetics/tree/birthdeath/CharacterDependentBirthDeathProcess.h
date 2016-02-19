#ifndef CharacterDependentBirthDeathProcess_H
#define CharacterDependentBirthDeathProcess_H

#include "TreeDiscreteCharacterData.h"
#include "CDSE.h"
#include "RateMatrix.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"


#include <vector>

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @file
     * This file contains the declaration of the random variable class for character-dependent birth-death process.
     * This class is derived from the stochastic node and each instance will represent a random variable.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2016-02-18, version 1.0
     *
     */
    class CharacterDependentBirthDeathProcess : public TypedDistribution<Tree> {
        
    public:
        CharacterDependentBirthDeathProcess(const TypedDagNode<double> *ro,
                                            const TypedDagNode<RbVector<double> >* lo, const TypedDagNode<RbVector<double> >* lh,
                                            const TypedDagNode<RbVector<double> >* mo, const TypedDagNode<RbVector<double> >* mh,
                                            const TypedDagNode<RateGenerator>* q,
                                            const TypedDagNode< double >* r, const TypedDagNode< RbVector< double > >* p,
                                            const TypedDagNode<double> *rh, const std::string &cdt, const std::vector<Taxon> &tn);
        
        // pure virtual member functions
        virtual CharacterDependentBirthDeathProcess*        clone(void) const;                                                                                  //!< Create an independent clone
        
        // public member functions you may want to override
        double                                              computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        virtual void                                        redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        virtual void                                        setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        
    protected:
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                        getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                  //!< get affected nodes
        virtual void                                        keepSpecialization(DagNode* affecter);
        virtual void                                        restoreSpecialization(DagNode *restorer);
        virtual void                                        touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        
        
        // helper functions
        void                                                buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
//        std::vector<double>*                                divergenceTimesSinceOrigin(void) const;                                                             //!< Extract the divergence times from the tree.
//        int                                                 diversity(double t) const;                                                                          //!< Diversity at time t.
//        std::vector<double>*                                getAgesOfInternalNodesFromMostRecentSample(void) const;                                             //!< Get the ages of all internal nodes since the time of the most recent tip age.
//        std::vector<double>*                                getAgesOfTipsFromMostRecentSample(void) const;                                                      //!< Get the ages of all tip nodes since the time of the most recent tip age.
//        void                                                simulateClade(std::vector<TopologyNode *> &n, double age, double present);
//        double                                              simulateNextAge(size_t n, double start, double end, double present) const;
        void                                                prepareProbComputation(void) const;
        void                                                simulateTree(void);
        
        
//        virtual double                                      computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        void                                                computeNodeProbability(const TopologyNode &n, size_t nIdx) const;
        double                                              computeRootLikelihood() const;
        virtual double                                      pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double                                              simulateDivergenceTime(double origin, double present) const;                                        //!< Simulate a speciation event.
        
        
        // helper functions
//        double                                              computeLnProbabilityDivergenceTimes(void) const;                                                          //!< Compute the log-transformed probability of the current value.
//        virtual std::vector<double>*                        simulateDivergenceTimes(size_t n, double origin, double present) const;                                                  //!< Simulate n speciation events.
        
        // members
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        size_t                                              num_taxa;                                                                                            //!< Number of taxa (needed for correct initialization).
        std::vector<Taxon>                                  taxa;                                                                                               //!< Taxon names that will be attached to new simulated trees.
        double                                              log_tree_topology_prob;                                                                                //!< Log-transformed tree topology probability (combinatorial constant).
        std::vector<size_t>                                 active_likelihood;
        mutable std::vector<bool>                           changed_nodes;
        mutable std::vector<bool>                           dirty_nodes;
        mutable std::vector<std::vector<state_type> >       node_states;
        size_t                                              num_rate_categories;
        size_t                                              num_observed_states;
        size_t                                              num_hidden_states;
        mutable std::vector<std::vector<double> >           scaling_factors;
        mutable double                                      total_scaling;

        // parameters
        const TypedDagNode<double>*                         root_age;                                                                                            //!< Time since the origin.
        const TypedDagNode<RbVector<double> >*              lambda_unobserved;
        const TypedDagNode<RbVector<double> >*              lambda_observed;
        const TypedDagNode<RbVector<double> >*              mu_unobserved;
        const TypedDagNode<RbVector<double> >*              mu_observed;
        const TypedDagNode< RbVector< double > >*           pi;                                                                                                 //!< The root frequencies (probabilities of the root states).
        const TypedDagNode<RateGenerator>*                  Q;
        const TypedDagNode<double>*                         rate;                                                                                                //!< Sampling probability of each species.
        const TypedDagNode<double>*                         rho;                                                                                                //!< Sampling probability of each species.
        
        mutable std::vector<double>                         speciation_rates;
        mutable std::vector<double>                         extinction_rates;
    };
    
}

#endif
