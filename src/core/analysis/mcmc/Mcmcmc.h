#ifndef Mcmcmc_H
#define Mcmcmc_H

#include "Cloneable.h"
#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Parallel Metropolis-Coupled Markov chain Monte Carlo (MCMCMC) algorithm class.
     *
     * This file contains the declaration of the Markov chain Monte Carlo (MCMC) algorithm class.
     * An MCMC object manages the MCMC analysis by setting up the chain, calling the moves, the monitors and etc.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Michael Landis & Sebastian Hoehna)
     * @since Version 1.0, 2013-05-20
     *
     */
    class Mcmcmc : Cloneable {
        
    public:
        Mcmcmc(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons, std::string sT="random", size_t nc=4, size_t si=100, double dt=0.1);
        Mcmcmc(const Mcmcmc &m);
        virtual                                            ~Mcmcmc(void);                                                          //!< Virtual destructor
        
        // public methods
        void                                                burnin(int g, int ti);
        Mcmcmc*                                             clone(void) const;
        void                                                printOperatorSummary(void) const;
        void                                                run(size_t g);
        
    private:
        void                                                initialize(void);
        void                                                swapChains(void);
        void                                                swapNeighborChains(void);
        void                                                swapRandomChains(void);
        void                                                synchronizeValues(void);
        void                                                synchronizeHeats(void);
        void                                                tune(void);
        void                                                updateChainState(size_t j);
        double                                              computeBeta(double d, size_t i);                                        // incremental temperature schedule
        
        size_t                                              numChains;
        size_t                                              numProcesses;
        std::vector<size_t>                                 heatRanks;
        std::vector<std::vector<size_t> >                   chainsPerProcess;
        std::vector<size_t>                                 processPerChain;
        std::vector<Mcmc*>                                  chains;
        std::vector<double>                                 chainValues;
        std::vector<double>                                 chainHeats;
        std::string                                         scheduleType;
        size_t                                              currentGeneration;
        size_t                                              swapInterval;
        
        size_t                                              activeChainIndex;               // index of coldest chain, i.e. which one samples the posterior
        bool                                                processActive;
        double                                              delta;                          // delta-T, temperature increment for computeBeta
        
        unsigned long                                       generation;
        unsigned long                                       numAttemptedSwaps;
        unsigned long                                       numAcceptedSwaps;
    };
    
}


#endif /* defined(__rb_mlandis__Mcmcmc__) */
