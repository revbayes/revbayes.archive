#ifndef BirthDeathProcess_H
#define BirthDeathProcess_H

#include "Taxon.h" 
#include "Tree.h"
#include "TypedDagNode.h"
#include "AbstractBirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @file
     * This file contains the declaration of the random variable class for constant rate birth-death process.
     *
     * @brief Declaration of the abstract Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class BirthDeathProcess : public AbstractBirthDeathProcess {
        
    public:
        BirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ro,
                                    const TypedDagNode<double> *rh, const std::string& ss, const std::string &cdt,
                                    const std::vector<Taxon> &tn);
        
        // pure virtual member functions
        virtual BirthDeathProcess*                          clone(void) const = 0;                                                      //!< Create an independent clone
        
        
    protected:
        // Parameter management functions
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        // pure virtual helper functions
        virtual double                                      lnSpeciationRate(double t) const = 0;                                       //!< Get the log-transformed speciation rate at time t.
        virtual double                                      rateIntegral(double t_low, double t_high) const = 0;                        //!< Compute the rate integral.
        virtual double                                      simulateDivergenceTime(double origin, double present, double rho) const = 0;//!< Simulate a speciation event.
        virtual double                                      pSurvival(double start, double end) const = 0;                              //!< Compute the probability of survival of the process (without incomplete taxon sampling).

        
        // helper functions
        virtual double                                      computeLnProbabilityTimes(void) const;                                      //!< Compute the log-transformed probability of the current value.
        double                                              lnP1(double t, double T, double r) const;
        double                                              lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const;         //!< Compute the log-transformed probability of the number of taxa.
        double                                              pSurvival(double start, double end, double r) const;                        //!< Compute the probability of survival of the process including uniform taxon sampling.
        double                                              simulateDivergenceTime(double origin, double present) const;                //!< Simulate a speciation event.
        
        // members
        const TypedDagNode<double>*                         rho;                                                                        //!< Sampling probability of each species.
        std::string                                         samplingStrategy;                                                           //!< The incomplete taxon sampling strategy (uniform/diversified).
        
    };
    
}

#endif
