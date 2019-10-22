#ifndef EpisodicBirthDeathSamplingTreatmentProcess_H
#define EpisodicBirthDeathSamplingTreatmentProcess_H

#include "AbstractBirthDeathProcess.h"
#include "RbVector.h"

#include <vector>
#include <set>

namespace RevBayesCore {

    class Clade;
    class Taxon;

    /**
     * @brief Piecewise-constant birth-death-sampling-treatment process with mass extinctions, burst speciation, and event-sampling.
     *
     * The piecewise-constant birth-death process has constant rates for each time interval.
     * At the end of each time interval there may be an abrupt rate-shift (jump) for each
     * of the rates. Additionally, there may be sampling at the end of each interval.
     * Finally, fossils are sampled with rate psi, the others (fossils and extant taxa) are
     * sampled at sampling times (including the present).
     *
     * We assume that the rate vectors have one more element than the rate-change vectors.
     * Thus, one rate-change means always two interval, two rate-changes three interval, and so on.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-18, version 1.0
     *
     */
    class EpisodicBirthDeathSamplingTreatmentProcess : public AbstractBirthDeathProcess {

    public:
        EpisodicBirthDeathSamplingTreatmentProcess (const TypedDagNode<double> *ra,
                                                    const DagNode *speciation,
                                                    const DagNode *extinction,
                                                    const DagNode *sampling,
                                                    const DagNode *treatment,
                                                    const DagNode *event_speciation,
                                                    const DagNode *event_extinction,
                                                    const DagNode *event_sampling,
                                                    const DagNode *event_treatment,
                                                    const TypedDagNode< RbVector<double> > *timeline,
                                                    const TypedDagNode< RbVector<double> > *speciation_timeline,
                                                    const TypedDagNode< RbVector<double> > *extinction_timeline,
                                                    const TypedDagNode< RbVector<double> > *sampling_timeline,
                                                    const TypedDagNode< RbVector<double> > *treatment_timeline,
                                                    const TypedDagNode< RbVector<double> > *event_speciation_timeline,
                                                    const TypedDagNode< RbVector<double> > *event_extinction_timeline,
                                                    const TypedDagNode< RbVector<double> > *event_sampling_timeline,
                                                    const std::string &cdt,
                                                    const std::vector<Taxon> &tn,
                                                    bool use_origin);  //!< Constructor

        // public member functions
        EpisodicBirthDeathSamplingTreatmentProcess*   clone(void) const;                                                    //!< Create an independent clone

    protected:
        // Parameter management functions
        double                                          computeLnProbabilityDivergenceTimes(void);                          //!< Compute the log-transformed probability of the current value.
        // Parameter management functions
        void                                            swapParameterInternal(const DagNode *oldP, const DagNode *newP);    //!< Swap a parameter

        // helper functions
        double                                          computeLnProbabilityTimes(void);                                    //!< Compute the log-transformed probability of the current value.
        void                                            countAllNodes(void);                                                 //!< Count bifurcating nodes, count all heterochronous nodes as either phi- or Phi-sampled and as either sampled ancestors or sampled extinct tips
        double                                          lnD(size_t i, double t) const;                                      //!< Branch-segment probability at time t with index i, using pre-computed vectors
        double                                          E(size_t i, double t, bool computeSurvival = false) const;                                       //!< Extinction probability at time t with index i, using pre-computed vectors
        size_t                                          findIndex(double t) const;                                          //!< Find the index so that times[index-1] < t < times[index]
        void                                            getOffset(void) const;
        double                                          lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        double                                          lnProbTreeShape(void) const;
        void                                            updateVectorParameters(void);
        void                                            prepareProbComputation(void);
        double                                          pSampling(double t) const;
        double                                          pSurvival(double start, double end) const;
        double                                          simulateDivergenceTime(double origin, double present) const;    //!< Simulate a speciation event.
        int                                             survivors(double t) const;                                 //!< Number of species alive at time t.
        int                                             whichIntervalTime(double t) const;                                //!< If a time corresponds to an interval/event time, returns that interval, otherwise returns -1

        // members
        const TypedDagNode<double >*                    homogeneous_lambda;                                    //!< The homogeneous birth rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_lambda;                                  //!< The heterogeneous birth rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_Lambda;                                  //!< The heterogeneous birth burst rates.
        const TypedDagNode<double >*                    homogeneous_mu;                                        //!< The homogeneous death rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_mu;                                      //!< The heterogeneous death rates.
        const TypedDagNode<double >*                    homogeneous_r;                                         //!< The homogeneous conditional probability of death upon treatment.
        const TypedDagNode<RbVector<double> >*          heterogeneous_r;                                       //!< The heterogeneous conditional probability of death upon treatment.
        const TypedDagNode<RbVector<double> >*          heterogeneous_Mu;                                      //!< The heterogeneous death burst (mass extinction) probabilities.
        const TypedDagNode<double >*                    homogeneous_phi;                                       //!< The homogeneous sampling rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_phi;                                     //!< The heterogeneous sampling rates.
        const TypedDagNode<double >*                    homogeneous_Phi;                                       //!< The probability of sampling a tip at the present.
        const TypedDagNode<RbVector<double> >*          heterogeneous_Phi;                                     //!< The probability of sampling individuals at set time intervals.
        const TypedDagNode<double >*                    homogeneous_R;                                         //!< The homogeneous conditional probability of death upon treatment.
        const TypedDagNode<RbVector<double> >*          heterogeneous_R;                                       //!< The heterogeneous conditional probability of death upon treatment.
        
        const TypedDagNode<RbVector<double> >*          interval_times;                                        //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_speciation;                             //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_extinction;                             //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_sampling;                               //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_treatment;                              //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_event_speciation;                       //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_event_extinction;                       //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        const TypedDagNode<RbVector<double> >*          interval_times_event_sampling;                         //!< The user-specified non-zero times of the instantaneous events and rate shifts.
        mutable std::vector<double>                     timeline;                                              //!< The times of the instantaneous events and rate shifts.

        std::vector<double>                             serial_tip_ages;                                       //!< The ages of all sampled dead lineages sampled by rate-sampling
        std::vector<double>                             serial_sampled_ancestor_ages;                          //!< The ages of all sampled ancestors sampled by rate-sampling
        std::vector<std::vector<double> >               event_tip_ages;                                        //!< The ages of all sampled dead lineages sampled at sampling events
        std::vector<std::vector<double> >               event_sampled_ancestor_ages;                           //!< The ages of all sampled ancestors sampled at sampling events
        std::vector<double>                             serial_bifurcation_times;                              //!< The ages of all bifurcation events in the tree NOT at a burst event
        std::vector<std::vector<double> >               event_bifurcation_times;                               //!< The ages of all bifurcation events in the tree at burst events

        mutable double                                  offset;                                                //!< In the case there the most recent tip is at time y, we internally adjust by this time and treat y as the present; this does not affect the boundary times of the rate shifts
        int                                             num_extant_taxa;

        std::vector<double>                             lambda;
        std::vector<double>                             mu;
        std::vector<double>                             phi;
        std::vector<double>                             r;
        std::vector<double>                             lambda_event;
        std::vector<double>                             mu_event;
        std::vector<double>                             phi_event;

        std::vector<double>                             A_i;                                                   //!< Helper values
        std::vector<double>                             B_i;                                                   //!< Helper values
        std::vector<double>                             C_i;                                                   //!< Helper values
        std::vector<double>                             E_previous;                                            //!< The probability that a lineage at time t has no sampled descendants.
        std::vector<double>                             lnD_previous;                                          //!< The probability of an observed lineage

        std::vector<double>                             A_survival_i;                                          //!< Helper values
        std::vector<double>                             B_survival_i;                                          //!< Helper values
        std::vector<double>                             C_survival_i;                                          //!< Helper values
        std::vector<double>                             E_survival_previous;                                   //!< The probability that a lineage at time t has no sampled EXTANT descendants.

    };
}

#endif
