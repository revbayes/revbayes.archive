#ifndef Mcmcmc_H
#define Mcmcmc_H

#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "MonteCarloSampler.h"
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
    class Mcmcmc : public MonteCarloSampler {
        
    public:
        Mcmcmc(const Model& m, const RbVector<Move> &mv, const RbVector<Monitor> &mn, std::string sT="random", size_t nc=4, size_t si=100, double dt=0.1, bool th=true, double tht=0.23, std::string sm="neighbor", std::string smo="multiple");
        Mcmcmc(const Mcmcmc &m);
        virtual                                ~Mcmcmc(void);                                       //!< Virtual destructor
        
        // public methods
        void                                    addFileMonitorExtension(const std::string &s, bool dir);
        void                                    addMonitor(const Monitor &m);
        void                                    disableScreenMonitor(bool all, size_t rep);         //!< Disable/remove all screen monitors
        Mcmcmc*                                 clone(void) const;
        void                                    finishMonitors(size_t n);                           //!< Finish the monitors
        const Model&                            getModel(void) const;
        double                                  getModelLnProbability(bool likelihood_only);
        RbVector<Monitor>&                      getMonitors( void );
        std::string                             getStrategyDescription(void) const;                 //!< Get the discription of the strategy used for this sampler.
        void                                    initializeSampler(bool priorOnly=false);            //!< Initialize objects for mcmc sampling
        void                                    monitor(unsigned long g);
        void                                    nextCycle(bool advanceCycle);
        void                                    printMoveSummary(std::ostream &o, size_t chainId, size_t moveId, Move &mv) const;
        void                                    printOperatorSummary(void);
        void                                    printSwapSummary(std::ostream &o) const;
        void                                    printSwapSummaryPair(std::ostream &o, const size_t &row, const size_t &col) const;
        void                                    redrawStartingValues(void);                         //!< Redraw the starting values.
        void                                    removeMonitors(void);
        void                                    reset(void);                                        //!< Reset the sampler for a new run.
        void                                    resetCounters(void);                                //!< Reset the counters.
        void                                    setHeatsInitial(const std::vector<double> &ht);
        void                                    setSwapInterval2(const size_t &si2);
        void                                    setLikelihoodHeat(double h);                        //!< Set the heat of the likelihood function.
        void                                    setModel(Model *m, bool redraw);
        void                                    setNumberOfProcesses(size_t i);                     //!< Set the number of processes for this replication.
        void                                    startMonitors(size_t numCycles, bool reopen);       //!< Start the monitors
        void                                    tune(void);                                         //!< Tune the sampler and its moves.
        void                                    writeMonitorHeaders(void);                          //!< Write the headers of the monitors.

        
    protected:
        void                                    setActivePIDSpecialized(size_t i, size_t n);                  //!< Set the number of processes for this class.

        
    private:
        void                                    initializeChains(void);
        void                                    swapChains(const std::string swap_method);
        void                                    swapMovesTuningInfo(RbVector<Move> &mvsj, RbVector<Move> &mvsk);
        void                                    swapNeighborChains(void);
        void                                    swapRandomChains(void);
        void                                    synchronizeValues(bool likelihood_only);
        void                                    synchronizeHeats(void);
        void                                    synchronizeTuningInfo(void);
        void                                    updateChainState(size_t j);
        double                                  computeBeta(double d, size_t i);                    // incremental temperature schedule
        
        size_t                                  num_chains;
        std::vector<size_t>                     heat_ranks;
        std::vector<size_t>                     pid_per_chain;
        std::vector<Mcmc*>                      chains;
        std::vector<double>                     chain_values;
        std::vector<double>                     chain_heats;
        std::string                             schedule_type;
        size_t                                  current_generation;
        size_t                                  burnin_generation;
        size_t                                  swap_interval;
        size_t                                  swap_interval2;
        
        size_t                                  active_chain_index;                                 // index of coldest chain, i.e. which one samples the posterior
        double                                  delta;                                              // delta-T, temperature increment for computeBeta
        std::vector<double>                     heat_temps;
        bool                                    tune_heat;
        double                                  tune_heat_target;
        bool                                    useNeighborSwapping;
        bool                                    useRandomSwapping;
        std::string                             swap_mode;
        
        Mcmc*                                   base_chain;
        
        unsigned long                           generation;
        std::vector< std::vector<unsigned long> > numAttemptedSwaps;
        std::vector< std::vector<unsigned long> > numAcceptedSwaps;
        
        std::vector< std::vector<Mcmc::tuningInfo> >  chain_moves_tuningInfo;
    };
    
}


#endif /* defined(__rb_mlandis__Mcmcmc__) */
