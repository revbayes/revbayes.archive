#ifndef ParallelMcmcmc_H
#define ParallelMcmcmc_H

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
    class ParallelMcmcmc : Cloneable {
        
    public:
        ParallelMcmcmc(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons, std::string sT="random", size_t nc=4, size_t si=100, double dt=0.1, double st=1.0, double sh=1.0);
        ParallelMcmcmc(const ParallelMcmcmc &m);
        virtual                                            ~ParallelMcmcmc(void);                                                          //!< Virtual destructor
        
        // public methods
        void                                                burnin(int g, int ti);
        ParallelMcmcmc*                                     clone(void) const;
        void                                                printOperatorSummary(void) const;
        void                                                run(size_t g);        
        
    private:
        void                                                initialize(void);
        void                                                swapChains(void);
        void                                                synchronizeValues(void);
        void                                                synchronizeHeats(void);
        void                                                updateChainState(size_t j);
        double                                              computeBeta(double d, double s, size_t i);   // incremental temperature schedule
        
        size_t                                              numChains;
        size_t                                              numProcesses;
        std::vector<size_t>                                 chainIdxByHeat;
        std::vector<std::vector<size_t> >                   chainsPerProcess;
        std::vector<size_t>                                 processPerChain;
        std::vector<Mcmc*>                                  chains;
        std::vector<double>                                 chainValues;
        std::vector<double>                                 chainHeats;
        std::string                                         scheduleType;
        size_t                                              currentGeneration;
        size_t                                              swapInterval;
        
        size_t                                              activeIndex;  // index of coldest chain, i.e. which one samples the posterior
        double                                              delta;        // delta-T, temperature increment for computeBeta
        double                                              sigma;        // scales power in heating schedule
        double                                              startingHeat; // default 1.0
        
    };
    
}


#endif /* defined(__rb_mlandis__ParallelMcmcmc__) */
