/**
 * @file
 * This file contains the declaration of the MCMC class. 
 * An MCMC object manages the MCMC analysis by setting up the chain, calling the moves and the monitors etc.
 *
 *
 * @brief Declaration of MCMC class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-06-17
 *
 * $Id$
 */


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
    
    class Mcmc {
    
    public:
        Mcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, bool ca=true, double ch=1.0, int ci=0);
        Mcmc(const Mcmc &m);
        virtual                                            ~Mcmc(void);                                                          //!< Virtual destructor
       
        // public methods
        void                                                burnin(int g, int ti);
        Mcmc*                                               clone(void) const;
        void                                                printOperatorSummary(void) const;
        virtual void                                        run(int g);

        // MJL: added for p(MC)^3
        virtual int                                         nextCycle(bool advanceCycle);
        //double                                              getLnLikelihood(void);
        double                                              getLnPosterior(void);
        double                                              getModelLnProbability(void);
        bool                                                isChainActive(void);
        double                                              getChainHeat(void);
        void                                                setChainActive(bool tf);
        void                                                setChainHeat(double v);
        void                                                setChainIdx(int);
        int                                                 getChainIdx(void);
        void                                                monitor(int g);
        std::vector<Monitor*>&                              getMonitors(void);
        void                                                redrawChainState(void);
        
    protected:

        void                                                getOrderedStochasticNodes(const DagNode* dagNode,  std::vector<DagNode*>& orderedStochasticNodes, std::set<const DagNode*>& visitedNodes);
        void                                                initialize(void);
        void                                                replaceDag(const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons);
        void                                                assignMoveSchedule(void);
    
        // members
        double                                              lnProbability;
        Model                                               model;
        std::vector<Move*>                                  moves;
        std::vector<Monitor*>                               monitors;
        std::map<Move*, std::set<DagNode*> >                orgNodesMoves;
        std::map<Monitor*, std::set<DagNode*> >             orgNodesMonitors;
//      unsigned int                                        tuningInterval;
        unsigned int                                        gen;
        RandomMoveSchedule                                  schedule;
        MoveSchedule*                                       moveSchedule;

        int                                                 chainIdx;
        bool                                                chainActive;
        double                                              chainHeat;
    };

}

#endif
