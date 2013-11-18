#ifndef Mcmc_H
#define Mcmc_H

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
     * @brief Declaration of MCMC class
     * 
     * This file contains the declaration of the MCMC class. 
     * An MCMC object manages the MCMC analysis by setting up the chain, calling the moves and the monitors etc.
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class Mcmc {
    
    public:
        Mcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, bool ca=true, double ch=1.0, int ci=0);
        Mcmc(const Mcmc &m);
        virtual                                            ~Mcmc(void);                                                          //!< Virtual destructor
       
        // overloaded operators
        Mcmc&                                               operator=(const Mcmc &m);
        
        // public methods
        void                                                burnin(int g, int ti);
        Mcmc*                                               clone(void) const;
        double                                              getChainHeat(void);
        int                                                 getChainIdx(void);
        double                                              getLnPosterior(void);
        double                                              getModelLnProbability(void);
        std::vector<Monitor*>&                              getMonitors(void);
        bool                                                isChainActive(void);
        void                                                monitor(int g);
        virtual int                                         nextCycle(bool advanceCycle);
        void                                                printOperatorSummary(void) const;
        void                                                redrawChainState(void);
        virtual void                                        run(int g);
        void                                                setChainActive(bool tf);
        void                                                setChainHeat(double v);                                                                 //!< Set the heating temparature of the chain
        void                                                setChainIdx(int);                                                                       //!< Set the index of the chain
        void                                                startMonitors(void);                                                                    //!< Start the monitors

    protected:

        void                                                assignMoveSchedule(void);
        void                                                getOrderedStochasticNodes(  const DagNode*              dagNode,
                                                                                        std::vector<DagNode*>&      orderedStochasticNodes,
                                                                                        std::set<const DagNode*>&   visitedNodes
                                                                                     );
        void                                                initializeChain(void);                                                                  //!< Initialize objects for mcmc sampling
        void                                                initializeMonitors(void);                                                               //!< Assign model and mcmc ptrs to monitors
        bool                                                isAComputableNumber(double x);                                                          //!< Decide whether number is computable (not inf, neginf, or nan)
        void                                                replaceDag(const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons);
    
        // members
        int                                                 chainIdx;
        bool                                                chainActive;
        double                                              chainHeat;
        unsigned int                                        generation;
        double                                              lnProbability;
        Model                                               model;
        std::vector<Move*>                                  moves;
        MoveSchedule*                                       moveSchedule;
        std::vector<Monitor*>                               monitors;
        std::map<Move*, std::set<DagNode*> >                orgNodesMoves;
        std::map<Monitor*, std::set<DagNode*> >             orgNodesMonitors;
        RandomMoveSchedule                                  schedule;

    };

}

#endif
