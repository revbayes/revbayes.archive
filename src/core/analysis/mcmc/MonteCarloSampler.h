#ifndef MonteCarloSampler_H
#define MonteCarloSampler_H

#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "SequenctialMoveSchedule.h"

#include <vector>

namespace RevBayesCore {
    
    class Monitor;
    
    /**
     * @brief Abstract class for all Monte Carlo samplers, such as the Markov chain Monte Carlo (MCMC) sampler and Metropolis coupled MCMC (MC^3).
     *
     * The Monte Carlo sampler base class provides the common methods for all Monte Carlo samplers, such as
     * - Markov chain Monte Carlo (MCMC)
     * - Metropolis coupled Markov chain Monte Carlo (MCMCMC or MC^3)
     * - PowerPosteriorSampler
     *
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-06-19
     *
     */
    class MonteCarloSampler {
        
    public:
        MonteCarloSampler(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons);
        MonteCarloSampler(const MonteCarloSampler &m);
        virtual                                            ~MonteCarloSampler(void);                                                                             //!< Virtual destructor
        
        // overloaded operators
        Mcmc&                                               operator=(const Mcmc &m);
        
        // public methods
        void                                                burnin(size_t g, size_t ti);
        Mcmc*                                               clone(void) const;
        double                                              getChainHeat(void);
        size_t                                              getChainIndex(void);
        double                                              getLnPosterior(void);
        double                                              getModelLnProbability(void);
        std::vector<Monitor*>&                              getMonitors(void);
        bool                                                isChainActive(void);
        void                                                monitor(unsigned long g);
        virtual unsigned long                               nextCycle(bool advanceCycle);
        void                                                printOperatorSummary(void) const;
        void                                                redrawChainState(void);
        virtual void                                        run(size_t g);
        void                                                setChainActive(bool tf);
        void                                                setChainHeat(double v);                                                                 //!< Set the heating temparature of the chain
        void                                                setChainIndex(size_t idx);                                                              //!< Set the index of the chain
        void                                                setScheduleType(const std::string &s);                                                  //!< Set the type of the move schedule
        void                                                startMonitors(void);                                                                    //!< Start the monitors
        
    protected:
        
        void                                                getOrderedStochasticNodes(  const DagNode*              dagNode,
                                                                                      std::vector<DagNode*>&      orderedStochasticNodes,
                                                                                      std::set<const DagNode*>&   visitedNodes);
        void                                                initializeChain(void);                                                                  //!< Initialize objects for mcmc sampling
        void                                                initializeMonitors(void);                                                               //!< Assign model and mcmc ptrs to monitors
        void                                                replaceDag(const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons);
        
        // members
        bool                                                chainActive;
        double                                              chainHeat;
        size_t                                              chainIdx;
        unsigned long                                       generation;
        double                                              lnProbability;
        Model                                               model;
        std::vector<Monitor*>                               monitors;
        std::vector<Move*>                                  moves;
        std::map<Monitor*, std::set<DagNode*> >             orgNodesMonitors;
        std::map<Move*, std::set<DagNode*> >                orgNodesMoves;
        MoveSchedule*                                       schedule;
        std::string                                         scheduleType;                                                                           //!< Type of move schedule to be used
        
    };
    
}

#endif
