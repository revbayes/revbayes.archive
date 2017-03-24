#ifndef AbstractBirthDeathProcess_H
#define AbstractBirthDeathProcess_H

#include "AbstractRootedTreeDistribution.h"

namespace RevBayesCore {
    
    /**
     * @file
     * This file contains the declaration of the random variable class for constant rate birth-death process.
     *
     * @brief Declaration of the constant rate Birth-Death process class.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class AbstractBirthDeathProcess : public AbstractRootedTreeDistribution {
        
    public:
        AbstractBirthDeathProcess(const TypedDagNode<double> *ra, const std::string &cdt, const std::vector<Taxon> &tn);
        
        
        virtual ~AbstractBirthDeathProcess(void);
        // pure virtual member functions
        virtual AbstractBirthDeathProcess*                  clone(void) const = 0;                                                                              //!< Create an independent clone
        
        
        
    protected:
        // pure virtual helper functions
        virtual double                                      computeLnProbabilityTimes(void) const = 0;                                                          //!< Compute the log-transformed probability of the current value.
        virtual double                                      lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const = 0;                             //!< Compute the log-transformed probability of the number of taxa.
//        virtual double                                      simNextAge(size_t n, double start, double end) const;                                             //!< Simulate one speciation events.
        virtual double                                      simulateDivergenceTime(double origin, double present) const = 0;                                    //!< Simulate n speciation events.
        virtual double                                      pSurvival(double start, double end) const = 0;                                                      //!< Compute the probability of survival of the process (without incomplete taxon sampling).
        virtual void                                        prepareProbComputation(void) const;

        
        // helper functions
        virtual double                                      computeLnProbabilityDivergenceTimes(void) const;                                                    //!< Compute the log-transformed probability of the current value.
        virtual std::vector<double>*                        simulateDivergenceTimes(size_t n, double origin, double present) const;                             //!< Simulate n speciation events.
        
        // members
        std::string                                         condition;                                                                                          //!< The condition of the process (none/survival/#taxa).
        
    };
    
}

#endif
