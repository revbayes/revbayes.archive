#ifndef MonteCarloSampler_H
#define MonteCarloSampler_H

#include "Cloneable.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RbVector.h"
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
    class MonteCarloSampler : public Cloneable {
        
    public:
        MonteCarloSampler(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons);
        MonteCarloSampler(const MonteCarloSampler &m);
        virtual                                            ~MonteCarloSampler(void);                                                                             //!< Virtual destructor
        
        // overloaded operators
        MonteCarloSampler&                                  operator=(const MonteCarloSampler &m);
        
        // pure virtual public methods
        virtual MonteCarloSampler*                          clone(void) const = 0;
        virtual void                                        run(size_t g) = 0;
        
        
        // public methods
        void                                                burnin(size_t g, size_t ti);
        double                                              getChainHeat(void) const;                                                               //!< Get the heat for this chain
        size_t                                              getChainIndex(void) const;                                                              //!< Get the index of this chain
        size_t                                              getCurrentGeneration(void) const;                                                       //!< Get the current generations number
//        double                                              getLnPosterior(void);
        double                                              getModelLnProbability(void);
        RbVector<Monitor>&                                  getMonitors(void);
        RbVector<Move>&                                     getMoves(void);
        MoveSchedule&                                       getSchedule(void);
        const MoveSchedule&                                 getSchedule(void) const;
        const std::string&                                  getScheduleType(void) const;
        bool                                                isChainActive(void);
        void                                                monitor(unsigned long g);
        virtual unsigned long                               nextCycle(bool advanceCycle);
        void                                                printOperatorSummary(void) const;
        void                                                redrawChainState(void);
        void                                                setChainActive(bool tf);
        void                                                setChainHeat(double v);                                                                 //!< Set the heating temparature of the chain
        void                                                setChainIndex(size_t idx);                                                              //!< Set the index of the chain
        void                                                setScheduleType(const std::string &s);                                                  //!< Set the type of the move schedule
        void                                                startMonitors(void);                                                        //!< Start the monitors
        void                                                startMonitors(size_t numCycles);                                                        //!< Start the monitors
        
    protected:
        
        void                                                getOrderedStochasticNodes(  const DagNode*              dagNode,
                                                                                        std::vector<DagNode*>&      orderedStochasticNodes,
                                                                                        std::set<const DagNode*>&   visitedNodes);
        void                                                initializeChain(bool priorOnly=false);                                                  //!< Initialize objects for mcmc sampling
        void                                                initializeMonitors(void);                                                               //!< Assign model and mcmc ptrs to monitors
        void                                                replaceDag(const RbVector<Move> &mvs, const RbVector<Monitor> &mons);
        
        // members
        bool                                                chainActive;
        double                                              chainHeat;
        size_t                                              chainIdx;
        unsigned long                                       generation;
//        double                                              lnProbability;
        Model                                               model;
        RbVector<Monitor>                                   monitors;
        RbVector<Move>                                      moves;
//        std::map<Monitor*, std::set<DagNode*> >             orgNodesMonitors;
//        std::map<Move*, std::set<DagNode*> >                orgNodesMoves;
        MoveSchedule*                                       schedule;
        std::string                                         scheduleType;                                                                           //!< Type of move schedule to be used
        
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const MonteCarloSampler& x);                                //!< Overloaded output operator

}

#endif
