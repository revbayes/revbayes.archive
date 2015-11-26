#ifndef PiecewiseConstantSerialSampledBirthDeathProcess_H
#define PiecewiseConstantSerialSampledBirthDeathProcess_H

#include "AbstractBirthDeathProcess.h"
#include "RbVector.h"

#include <vector>
#include <set>

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    /**
     * @brief Piecewise-Constant birth-death process with serially sampled tips.
     *
     * The piecewise-constant birth-death process has constant rates for each time interval.
     * At the end of each time interval there may be an abrupt rate-shift (jump) for each
     * of the rates. Additionally, there may be sampling at the end of each interval.
     * Finally, tips may be sampled when a lineages dies (e.g., fossils) with rate psi or at
     * sampling times (including the present).
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
    class PiecewiseConstantSerialSampledBirthDeathProcess : public AbstractBirthDeathProcess {
        
    public:
        PiecewiseConstantSerialSampledBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra,
                                                        const TypedDagNode<RbVector<double> > *s, const TypedDagNode<RbVector<double> > *st,
                                                        const TypedDagNode<RbVector<double> > *e, const TypedDagNode<RbVector<double> > *et,
                                                        const TypedDagNode<RbVector<double> > *p, const TypedDagNode<RbVector<double> > *pt,
                                                        const TypedDagNode<RbVector<double> > *r, const TypedDagNode<RbVector<double> > *rt,
                                                        double tLastSample, const std::string &cdt, const std::vector<Taxon> &tn);        //!< Constructor
        
        // public member functions
        PiecewiseConstantSerialSampledBirthDeathProcess*    clone(void) const;                                                                                  //!< Create an independent clone
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        size_t                                              l(double t) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1] 
        size_t                                              l(double t, size_t min, size_t max) const;                                                                                  //!< Find the max index so that rateChangeTimes[index] < t < rateChangeTimes[index+1] 
        std::vector<double>*                                simSpeciations(size_t n, double origin) const;                                                      //!< Simulate n speciation events.
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const { throw RbException("Cannot compute P(nTaxa)."); }
        double                                              pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double                                              p(size_t i, double t) const;
        void                                                prepareProbComputation(void);
        double                                              q(size_t i, double t) const;
        int                                                 survivors(double t) const;                                                                          //!< Number of species alive at time t.
        
        // members
        const TypedDagNode<RbVector<double> >*              lambda;                                                                                             //!< The speciation rates.
        const TypedDagNode<RbVector<double> >*              lambdaTimes;                                                                                        //!< The time of the speciation rate changes.
        const TypedDagNode<RbVector<double> >*              mu;                                                                                                 //!< The extinction rates.
        const TypedDagNode<RbVector<double> >*              muTimes;                                                                                            //!< The times of the extinction rate changes.
        const TypedDagNode<RbVector<double> >*              psi;                                                                                                //!< The (fossil) sampling rates.
        const TypedDagNode<RbVector<double> >*              psiTimes;                                                                                           //!< The times of the (fossil) sampling rate changes.
        const TypedDagNode<RbVector<double> >*              rho;                                                                                                //!< The instantaneous sampling probability.
        const TypedDagNode<RbVector<double> >*              rhoTimes;                                                                                           //!< The times of the instantaneous sampling events.
        double                                              timeSinceLastSample;
        
        mutable std::vector<double>                         rateChangeTimes;
        mutable std::vector<double>                         birth;
        mutable std::vector<double>                         death;
        mutable std::vector<double>                         fossil;
        mutable std::vector<double>                         sampling;
    };
    
}

#endif
