#ifndef Mcmc_H
#define Mcmc_H

#include "MonteCarloSampler.h"

namespace RevBayesCore {
    
    class Monitor;
    
    /**
     * @brief Declaration of MCMC class
     * 
     * This file contains the declaration of the Markov chain Monte Carlo (MCMC) algorithm class. 
     * An MCMC object manages the MCMC analysis by setting up the chain, calling the moves, the monitors and etc.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class Mcmc : public MonteCarloSampler {
    
    public:
        Mcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons);
        virtual                                            ~Mcmc(void);                                                                             //!< Virtual destructor
        
        // public methods
        Mcmc*                                               clone(void) const;
        void                                                monitor(unsigned long g);
        virtual unsigned long                               nextCycle(bool advanceCycle);
        virtual void                                        run(size_t g);

    protected:

    };

}

#endif
