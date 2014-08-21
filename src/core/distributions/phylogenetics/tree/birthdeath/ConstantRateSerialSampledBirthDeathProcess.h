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
     * The diversity-dependent pure-birth process is a species tree distribution without extinction events.
     * The speciation rate depends on the current diversity by the function:
     * lambda = (1.0 - n/K) * lambda_0
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
        
        // Parameter management functions
        std::set<const DagNode*>                            getParameters(void) const;                                          //!< Return parameters
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
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
