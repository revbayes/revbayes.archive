#ifndef ConstantRateSerialSampledBirthDeathProcess_H
#define ConstantRateSerialSampledBirthDeathProcess_H

#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    class Taxon;
    
    /**
     * @brief Constant-rate birth-death process with serially sampled tips.
     *
     *
     * The constant rate birth-death process with serially sampled tips is an extension
     * of the ordinary constant rate birth-death process. The process has four parameters:
     * lambda := speciation rate
     * mu     := extinction rate
     * psi    := sampling rate
     * rho    := sampling probability at present
     * Note that at a sampling event the lineage also goes extinct afterwards.
     * That means, there cannot be any ancestral samples. This may be realistic for virus evolution
     * when a sampled strain will be contained.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-18, version 1.0
     *
     */
    class ConstantRateSerialSampledBirthDeathProcess : public AbstractBirthDeathProcess {
        
    public:
        ConstantRateSerialSampledBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e, const TypedDagNode<double> *p, const TypedDagNode<double> *r,
                                           double tLastSample, const std::string &cdt, const std::vector<Taxon> &tn, const std::vector<Clade> &c);        //!< Constructor
        
        // public member functions
        ConstantRateSerialSampledBirthDeathProcess*         clone(void) const;                                                                                  //!< Create an independent clone
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
    private:
        
        // helper functions
        double                                              computeLnProbabilityTimes(void) const;                                                              //!< Compute the log-transformed probability of the current value.
        std::vector<double>*                                simSpeciations(size_t n, double origin) const;                                                      //!< Simulate n speciation events.
        double                                              pSurvival(double start, double end) const;                                                          //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        double                                              q(double t) const;
        
        // members
        const TypedDagNode<double>*                         lambda;                                                                                             //!< The speciation rate.
        const TypedDagNode<double>*                         mu;                                                                                                 //!< The extinction rate.
        const TypedDagNode<double>*                         psi;                                                                                                  //!< The sampling probability of a just extinct species.
        const TypedDagNode<double>*                         rho;                                                                                                //!< The sampling probability of extant taxa.
        double                                              timeSinceLastSample;
        
    };
    
}

#endif
