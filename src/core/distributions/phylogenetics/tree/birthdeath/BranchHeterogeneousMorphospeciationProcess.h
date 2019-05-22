#ifndef BranchHeterogeneousMorphospeciationProcess_H
#define BranchHeterogeneousMorphospeciationProcess_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "TreeDiscreteCharacterData.h"
#include "Simplex.h"
#include "Morphospeciation_ODE.h"
#include "Taxon.h"
#include "Tree.h"
#include "TreeChangeEventListener.h"
#include "TypedDagNode.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "RlString.h"

typedef std::vector< double > state_type;

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
    class BranchHeterogeneousMorphospeciationProcess : public TypedDistribution<Tree>, public TreeChangeEventListener, public MemberObject< RbVector<double> > {
        
    public:
        BranchHeterogeneousMorphospeciationProcess(const TypedDagNode<double> *root,
                                  const TypedDagNode<double> *asym,
                                  const TypedDagNode<double> *ana,
                                  const TypedDagNode<double> *ext,
                                  const TypedDagNode<double> *foss,
                                  const TypedDagNode<double> *rh,
                                  const std::string &cdt,
                                  size_t n,
                                  size_t k,
                                  bool uo,
                                  size_t min_num_lineages,
                                  size_t max_num_lineages,
                                  size_t exact_num_lineages,
                                  double max_t,
                                  bool prune,
                                  bool condition_on_tip_states,
                                  bool condition_on_num_tips,
                                  bool condition_on_tree);
        
        // pure virtual member functions
        virtual BranchHeterogeneousMorphospeciationProcess*             clone(void) const;
        virtual                                                         ~BranchHeterogeneousMorphospeciationProcess(void);                                                                     //!< Virtual destructor

        void                                                            operator() ( const state_type &x , state_type &dxdt , const double t );
        double                                                          p( double t ) const;                //!< p(t)

        double                                                          computeLnProbability(void);
        void                                                            fireTreeChangeEvent(const TopologyNode &n, const unsigned& m=0);                                    //!< The tree has changed and we want to know which part.
        const AbstractHomologousDiscreteCharacterData&                  getCharacterData() const;
        double                                                          getOriginAge(void) const;
        double                                                          getRootAge(void) const;
        virtual void                                                    redrawValue(void);
        void                                                            setSiteRates(const TypedDagNode< RbVector< double > > *r);
        void                                                            setNumberOfTimeSlices(double n);                                                                    //!< Set the number of time slices for the numerical ODE.
        virtual void                                                    setValue(Tree *v, bool f=false);                                                                    //!< Set the current value, e.g. attach an observation (clamp)
        
        void                                                            numericallyIntegrateProcess(double begin_age, double end_age, double rate, state_type &transition_probs) const; //!< Wrapper function for the ODE time stepper function.
        void                                                            resizeVectors();
        
    protected:
        
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                    getAffected(RbOrderedSet<DagNode *>& affected, DagNode* affecter);                                  //!< get affected nodes
        virtual void                                                    keepSpecialization(DagNode* affecter);
        virtual void                                                    restoreSpecialization(DagNode *restorer);
        virtual void                                                    touchSpecialization(DagNode *toucher, bool touchAll);
        
        double                                                          lnProbTreeShape(void) const;

        // Parameter management functions. You need to override both if you have additional parameters
        virtual void                                                    swapParameterInternal(const DagNode *oldP, const DagNode *newP);                                    //!< Swap a parameter
        void                                                            executeMethod(const std::string &n, const std::vector<const DagNode*> &args, RbVector<double> &rv) const;  
        RevLanguage::RevPtr<RevLanguage::RevVariable>                   executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found);
        
        // helper functions
        void                                                            buildRandomBinaryTree(std::vector<TopologyNode *> &tips);
        void                                                            recursivelyFlagNodeDirty(const TopologyNode& n);
        bool                                                            simulateTree(size_t attempts = 0);
        void                                                            computeNodeProbability(const TopologyNode &n, size_t nIdx);
        double                                                          computeRootLikelihood();
        
        virtual void                                                    updateTransitionProbabilities(size_t node_idx);

        // members
        size_t                                                          num_sites;
        size_t                                                          num_states;
        size_t                                                          num_site_rates;
        std::vector<std::vector<std::vector<double> > >                 transition_prob_matrices;

        std::string                                                     condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        double                                                          dt;                                                                                                 //!< The size of the time slices used by the ODE for numerical integration.
        std::vector<bool>                                               active_likelihood;
        mutable std::vector<bool>                                       changed_nodes;
        mutable std::vector<bool>                                       dirty_nodes;

        mutable std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >         node_partial_likelihoods;

        mutable std::vector< std::vector< std::vector<double> > >       scaling_factors;                                                                                    //!< do we use the speciation rates from the cladogenetic event map?
        bool                                                            use_origin;
        
        // parameters
        const TypedDagNode<double>*                                     process_age;                                                                                        //!< Time since the origin.
        const TypedDagNode<double>*                                     lambda;
        const TypedDagNode<double>*                                     lambda_a;
        const TypedDagNode<double>*                                     mu;
        const TypedDagNode<double>*                                     psi;                                                                                              //!< Sampling probability of each species.
        const TypedDagNode<double>*                                     rho;                                                                                                //!< Sampling probability of each species.
        
        const TypedDagNode< RbVector< double > >*                       site_rates;

        std::vector<size_t>                                             speciation_chars;

        bool                                                            rate_variation_across_sites;

        size_t                                                          min_num_lineages;
        size_t                                                          max_num_lineages;
        size_t                                                          exact_num_lineages;
        double                                                          max_time;
        bool                                                            prune_extinct_lineages;
        double                                                          NUM_TIME_SLICES;
    };
    
}

#endif
