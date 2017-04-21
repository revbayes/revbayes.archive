#ifndef StateDependentSpeciationExtinctionProcess_H
#define StateDependentSpeciationExtinctionProcess_H

#include "TreeDiscreteCharacterData.h"
#include "CDCladoSE.h"
#include "MatrixReal.h"
#include "RateMatrix.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"


#include <vector>

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @file
     * This file contains the declaration of the random variable class for the character-dependent
     * cladogenetic birth-death process: ClaSSE as described in Golberg & Igic 2012
     *
     * Will Freyman 6/22/16
     *
     */
    class StateDependentSpeciationExtinctionProcess : public TypedDistribution<Tree> {
        
    public:
        StateDependentSpeciationExtinctionProcess(const TypedDagNode<double> *root,
                                                  const TypedDagNode<RbVector<double> >* m,
                                                  const TypedDagNode<RateGenerator>* q,
                                                  const TypedDagNode<double>* r,
                                                  const TypedDagNode< RbVector< double > >* p,
                                                  const TypedDagNode<double> *rh,
                                                  const std::string &cdt,
                                                  const std::vector<Taxon> &tn);
        
        // pure virtual member functions
        virtual StateDependentSpeciationExtinctionProcess*              clone(void) const;                                                                                  //!< Create an independent clone
        
        double                                                          computeLnProbability(void);                                                                         //!< Compute the log-transformed probability of the current value.
        const AbstractHomologousDiscreteCharacterData&                  getCharacterData() const;
        virtual void                                                    redrawValue(void);                                                                                  //!< Draw a new random value from the distribution
        void                                                            setCladogenesisMatrix(const TypedDagNode< MatrixReal > *r);
        void                                                            setFossilizationRates(const TypedDagNode< RbVector<double> > *r);
        void                                                            setSampleCharacterHistory(bool sample_history);                                                     //!< Set whether or not we are sampling the character history along branches.
        void                                                            setSpeciationRates(const TypedDagNode< RbVector<double> > *r);
        void                                                            setNumberOfTimeSlices(double n);                                                                    //!< Set the number of time slices for the numerical ODE.
        virtual void                                                    setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        void                                                            drawJointConditionalAncestralStates(std::vector<size_t>& startStates, std::vector<size_t>& endStates);
        void                                                            recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<size_t>& startStates, std::vector<size_t>& endStates);
        void                                                            drawStochasticCharacterMap(std::vector<std::string*>& character_histories);
        void                                                            recursivelyDrawStochasticCharacterMap(const TopologyNode &node, size_t start_state, std::vector<std::string*>& character_histories);
        void                                                            numericallyIntegrateProcess(state_type &likelihoods, double begin_age, double end_age, bool use_backward, bool extinction_only) const; //!< Wrapper function for the ODE time stepper function.
        
    protected:
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                    getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                  //!< get affected nodes
        virtual void                                                    keepSpecialization(DagNode* affecter);
        virtual void                                                    restoreSpecialization(DagNode *restorer);
        virtual void                                                    touchSpecialization(DagNode *toucher, bool touchAll);
        
        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        void                                                            executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found);
        
        // helper functions
        void                                                            buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        std::vector<double>                                             pExtinction(double start, double end) const;                                                        //!< Compute the probability of extinction of the process (without incomplete taxon sampling).
        virtual double                                                  pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        void                                                            simulateTree(void);
        void                                                            computeNodeProbability(const TopologyNode &n, size_t nIdx) const;
        double                                                          computeRootLikelihood() const;
        
        // members
        std::string                                                     condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        double                                                          dt;                                                                                                 //!< The size of the time slices used by the ODE for numerical integration.
        size_t                                                          num_taxa;                                                                                           //!< Number of taxa (needed for correct initialization).
        std::vector<Taxon>                                              taxa;                                                                                               //!< Taxon names that will be attached to new simulated trees.
        double                                                          log_tree_topology_prob;                                                                             //!< Log-transformed tree topology probability (combinatorial constant).
        std::vector<size_t>                                             active_likelihood;
        mutable std::vector<bool>                                       changed_nodes;
        mutable std::vector<bool>                                       dirty_nodes;
        mutable std::vector<std::vector<std::vector<double> > >         node_partial_likelihoods;
        mutable std::map<size_t, std::vector<std::vector<double> > >    branch_partial_likelihoods;
        mutable std::vector<std::vector<double> >                       extinction_probabilities;
        size_t                                                          num_states;
        mutable std::vector<std::vector<double> >                       scaling_factors;
        mutable double                                                  total_scaling;
        bool                                                            use_cladogenetic_events;                                                                            //!< do we use the speciation rates from the cladogenetic event map?
        bool                                                            sample_character_history;                                                                           //!< are we sampling the character history along branches?
        
        // parameters
        const TypedDagNode< MatrixReal >*                               cladogenesis_matrix;
        const TypedDagNode<double>*                                     root_age;                                                                                           //!< Time since the origin.
        const TypedDagNode<RbVector<double> >*                          mu;
        const TypedDagNode<RbVector<double> >*                          lambda;
        const TypedDagNode<RbVector<double> >*                          psi;
        const TypedDagNode<RbVector< double > >*                        pi;                                                                                                 //!< The root frequencies (probabilities of the root states).
        const TypedDagNode<RateGenerator>*                              Q;
        const TypedDagNode<double>*                                     rate;                                                                                               //!< Sampling probability of each species.
        const TypedDagNode<double>*                                     rho;                                                                                                //!< Sampling probability of each species.
        
        double                                                          NUM_TIME_SLICES;
    };
    
}

#endif
