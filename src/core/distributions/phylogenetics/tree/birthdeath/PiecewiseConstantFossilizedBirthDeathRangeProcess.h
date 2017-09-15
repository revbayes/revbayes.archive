#ifndef PiecewiseConstantFossilizedBirthDeathRangeProcess_H
#define PiecewiseConstantFossilizedBirthDeathRangeProcess_H

#include "MatrixReal.h"
#include "RbVector.h"
#include "TypedDagNode.h"
#include "TypedDistribution.h"

#include <vector>
#include <set>

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    /**
     * @brief Piecewise-constant fossilized birth-death process with serially sampled fossils.
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
    class PiecewiseConstantFossilizedBirthDeathRangeProcess : public TypedDistribution<MatrixReal > {
        
    public:
        PiecewiseConstantFossilizedBirthDeathRangeProcess (const DagNode *speciation,
                                                      const DagNode *extinction,
                                                      const DagNode *psi,
                                                      const DagNode *counts,
                                                      const TypedDagNode<double>* rho,
                                                      const TypedDagNode<RbVector<double> > *times,
                                                      const std::string &condition,
                                                      const std::vector<Taxon> &taxa);  //!< Constructor
        
        // public member functions
        PiecewiseConstantFossilizedBirthDeathRangeProcess*   clone(void) const;                                         //!< Create an independent clone

        double                                          getExtinctionRate( size_t index ) const;
        int                                             getFossilCount( size_t index ) const;
        double                                          getFossilizationRate( size_t index ) const;
        double                                          getIntervalTime( size_t index ) const;
        double                                          getSpeciationRate( size_t index ) const;

    protected:
        // Parameter management functions
        double                                          computeLnProbability(void);                            //!< Compute the log-transformed probability of the current value.

        // Parameter management functions
        void                                            swapParameterInternal(const DagNode *oldP, const DagNode *newP);                //!< Swap a parameter

        void                                            keepSpecialization(DagNode *toucher);
        void                                            restoreSpecialization(DagNode *toucher);
        void                                            touchSpecialization(DagNode *toucher, bool touchAll);

    private:
        
        // helper functions
        void                                            updateGamma(bool force = false);                             //!< Number of species alive at time t.
        size_t                                          l(double t) const;                                     //!< Find the index so that times[index-1] < t < times[index]
        double                                          pSurvival(double start, double end) const;             //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double                                          p(size_t i, double t) const;
        double                                          q(size_t i, double t, bool tilde = false) const;

        void                                            updateIntervals();
        void                                            redrawValue(void);

        size_t                                          num_intervals;
        size_t                                          num_fossil_counts;

        // members
        const TypedDagNode<double >*                    homogeneous_lambda;                                    //!< The homogeneous speciation rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_lambda;                                  //!< The heterogeneous speciation rates.
        const TypedDagNode<double >*                    homogeneous_mu;                                        //!< The homogeneous speciation rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_mu;                                      //!< The heterogeneous speciation rates.
        const TypedDagNode<double >*                    homogeneous_psi;                                       //!< The homogeneous speciation rates.
        const TypedDagNode<RbVector<double> >*          heterogeneous_psi;                                     //!< The heterogeneous speciation rates.
        const TypedDagNode<double >*                    homogeneous_rho;                                       //!< The homogeneous speciation rates.
        const TypedDagNode<RbVector<double> >*          timeline;                                              //!< The times of the instantaneous sampling events.
        const TypedDagNode<long>*                        homogeneous_fossil_counts;                             //!< The number of fossil observations, per interval.
        const TypedDagNode<RbVector<long> >*             heterogeneous_fossil_counts;                           //!< The number of fossil observations, per interval.

        std::vector<double>                             birth;
        std::vector<double>                             death;
        std::vector<double>                             fossil;
        std::vector<double>                             times;

        std::vector<double>                             q_i;
        std::vector<double>                             q_tilde_i;
        std::vector<double>                             p_i;

        std::vector<size_t>                             gamma_i;
        std::vector<std::vector<bool> >                 gamma_links;
        std::vector<bool>                               dirty_gamma;


        std::string                                     condition;
        std::vector<Taxon>                              taxa;                                                                                               //!< Taxon names that will be attached to new simulated trees.
    };
}

#endif
