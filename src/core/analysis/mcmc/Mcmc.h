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
        Mcmc(const Model &m, const RbVector<Move> &moves, const RbVector<Monitor> &mons);
        Mcmc(const Mcmc &m);
        virtual                                            ~Mcmc(void);                                                                             //!< Virtual destructor
        
        Mcmc&                                               operator=(const Mcmc &m);                                                               //!< Overloaded assignment operator
        
        // public methods
        void                                                addFileMonitorExtension(const std::string &s, bool dir);
        void                                                addMonitor(const Monitor &m);
        void                                                disableScreenMonitor(bool all, size_t rep);                                             //!< Disable/remove all screen monitors
        Mcmc*                                               clone(void) const;
        void                                                finishMonitors(size_t n);                                                               //!< Finish the monitors
        double                                              getChainLikelihoodHeat(void) const;                                                     //!< Get the heat for this chain
        double                                              getChainPosteriorHeat(void) const;                                                      //!< Get the heat for this chain
        size_t                                              getChainIndex(void) const;                                                              //!< Get the index of this chain
        const Model&                                        getModel(void) const;
        double                                              getModelLnProbability(void);
        RbVector<Monitor>&                                  getMonitors(void);
        RbVector<Move>&                                     getMoves(void);
        MoveSchedule&                                       getSchedule(void);
        const MoveSchedule&                                 getSchedule(void) const;
        const std::string&                                  getScheduleType(void) const;
        std::string                                         getStrategyDescription(void) const;                                                     //!< Get the description of the strategy used here.
        void                                                initializeSampler(bool priorOnly=false);                                                //!< Initialize objects for mcmc sampling
        void                                                monitor(unsigned long g);
        void                                                nextCycle(bool advanceCycle);
        bool                                                isChainActive(void);
        void                                                printOperatorSummary(void) const;
        void                                                redrawStartingValues(void);                                                             //!< Redraw the starting values.
        void                                                removeMonitors(void);
        void                                                reset(void);                                                                            //!< Reset the sampler and set all the counters back to 0.
        void                                                setChainActive(bool tf);
        void                                                setChainLikelihoodHeat(double v);                                                       //!< Set the heating temparature of the likelihood of the chain
        void                                                setChainPosteriorHeat(double v);                                                        //!< Set the heating temparature of the posterior of the chain
        void                                                setChainIndex(size_t idx);                                                              //!< Set the index of the chain
        void                                                setLikelihoodHeat(double v);                                                            //!< Set the heating temparature of the likelihood of the chain
        void                                                setModel(Model *m);
        void                                                setScheduleType(const std::string &s);                                                  //!< Set the type of the move schedule
        void                                                startMonitors(size_t numCycles, bool reopen);                                           //!< Start the monitors
        void                                                tune(void);                                                                             //!< Tune the sampler and its moves.
        void                                                writeMonitorHeaders(void);                                                              //!< Write the headers of the monitors
        
    protected:
        void                                                initializeMonitors(void);                                                               //!< Assign model and mcmc ptrs to monitors
        void                                                replaceDag(const RbVector<Move> &mvs, const RbVector<Monitor> &mons);
        void                                                setActivePIDSpecialized(size_t i, size_t n);                                                      //!< Set the number of processes for this class.

        
        bool                                                chain_active;
        double                                              chain_likelihood_heat;
        double                                              chain_posterior_heat;
        size_t                                              chain_idx;
        Model*                                              model;
        RbVector<Monitor>                                   monitors;
        RbVector<Move>                                      moves;
        MoveSchedule*                                       schedule;
        std::string                                         schedule_type;                                                                           //!< Type of move schedule to be used

    };

}

#endif
