#ifndef PiecewiseConstantMorphospeciationProcess_H
#define PiecewiseConstantMorphospeciationProcess_H

#include "RbVector.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"

typedef std::vector< double > state_type;

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    /**
     * @brief Piecewise-constant morphospeciation process with serially sampled fossils.
     *
     * The piecewise-constant morphospeciation process has constant rates for each time interval.
     * At the end of each time interval there may be an abrupt rate-shift (jump) for each
     * of the rates. Additionally, there may be sampling at the end of each interval.
     * Finally, fossils are sampled with rate psi, the others (fossils and extant taxa) are
     * sampled at sampling times (including the present).
     *
     * We assume that the rate vectors have one more element than the rate-change vectors.
     * Thus, one rate-change means always two interval, two rate-changes three interval, and so on.
     *
     */
    class PiecewiseConstantMorphospeciationProcess : public PiecewiseConstantFossilizedBirthDeathProcess {
        
        using AbstractBirthDeathProcess::taxa;

    public:
        PiecewiseConstantMorphospeciationProcess (const TypedDagNode<double>* ra,
                                                      const DagNode *speciation,
                                                      const DagNode *extinction,
                                                      const DagNode *psi,
                                                      const DagNode *counts,
                                                      const TypedDagNode<double>* rho,
                                                      const DagNode *lambda_a,
                                                      const DagNode *beta,
                                                      const TypedDagNode<RbVector<double> > *times,
                                                      const std::string &condition,
                                                      const std::vector<Taxon> &taxa,
                                                      bool uo,
                                                      bool pa,
                                                      bool ex,
                                                      size_t n,
                                                      size_t k);  //!< Constructor
        
        // public member functions
        PiecewiseConstantMorphospeciationProcess*       clone(void) const;                                         //!< Create an independent clone

        void operator() ( const state_type &x , state_type &dxdt , const double t );

    protected:
        
        void                                                            numericallyIntegrateProcess(double begin_age, double end_age, double rate, state_type &transition_probs) const; //!< Wrapper function for the ODE time stepper function.
        virtual void                                                    updateTransitionProbabilities(size_t node_idx);

        // members
        size_t                                                          num_sites;
        size_t                                                          num_states;
        size_t                                                          num_site_rates;
        std::vector<std::vector<std::vector<double> > >                 transition_prob_matrices;

        double                                                          dt;                                                                                                 //!< The size of the time slices used by the ODE for numerical integration.
        std::vector<bool>                                               active_likelihood;
        mutable std::vector<bool>                                       changed_nodes;
        mutable std::vector<bool>                                       dirty_nodes;

        mutable std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >         node_partial_likelihoods;

        mutable std::vector< std::vector< std::vector<double> > >       scaling_factors;                                                                                    //!< do we use the speciation rates from the cladogenetic event map?

        const TypedDagNode< RbVector< double > >*                       site_rates;

        std::vector<size_t>                                             speciation_chars;

        bool                                                            rate_variation_across_sites;

        double                                                          NUM_TIME_SLICES;

    };
}

#endif
