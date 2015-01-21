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
        MonteCarloSampler(void);
        virtual                                            ~MonteCarloSampler(void);                            //!< Virtual destructor
                
        // pure virtual public methods
        virtual MonteCarloSampler*                          clone(void) const = 0;
//        virtual void                                        run(size_t g) = 0;
        virtual double                                      getModelLnProbability(void) = 0;
        virtual std::string                                 getStrategyDescription(void) const = 0;             //!< Get the discription of the strategy used for this sampler.
        virtual void                                        initializeSampler(bool priorOnly=false) = 0;        //!< Initialize objects for mcmc sampling
        virtual void                                        monitor(unsigned long g) = 0;
        virtual void                                        nextCycle(bool advanceCycle) = 0;
        virtual void                                        printOperatorSummary(void) const = 0;
        virtual void                                        reset(void) = 0;                                    //!< Reset the sampler for a new run.
        virtual void                                        setLikelihoodHeat(double v);                        //!< Set the heating temparature of the likelihood of the chain
        virtual void                                        setReplicateIndex(size_t i) = 0;                    //!< Set the index for this replication.
        virtual void                                        setStoneIndex(size_t i) = 0;                        //!< Set the index for this stone.
        virtual void                                        startMonitors(void) = 0;                            //!< Start the monitors
        virtual void                                        startMonitors(size_t numCycles) = 0;                //!< Start the monitors
        virtual void                                        tune(void) = 0;                                     //!< Tune the sampler and its moves.
        
        // public methods
        size_t                                              getCurrentGeneration(void) const;                   //!< Get the current generations number
//        void                                                initializeMonitors(void);                         //!< Assign model and mcmc ptrs to monitors
//        void                                                redrawChainState(void);
        
    protected:
                
        // members
        unsigned long                                       generation;
        
    };

    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const MonteCarloSampler& x);                                //!< Overloaded output operator

}

#endif
