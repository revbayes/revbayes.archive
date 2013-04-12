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

#include <vector>

namespace RevBayesCore {
    
    class Mcmc {
    
    public:
        Mcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons);
        Mcmc(const Mcmc &m);
        virtual                                            ~Mcmc(void);                                                          //!< Virtual destructor
       
        // public methods
        void                                                burnin(int g, int ti);
        Mcmc*                                               clone(void) const;
        void                                                printOperatorSummary(void) const;
        void                                                run(int g);


    private:
        void                                                getOrderedStochasticNodes(const DagNode* dagNode,  std::vector<DagNode*>& orderedStochasticNodes, std::set<const DagNode*>& visitedNodes);
        void                                                initialize(void);
    
        // members
        double                                              lnProbability;
        Model                                               model;
        std::vector<Move*>                                  moves;
        std::vector<Monitor*>                               monitors;
        std::map<Move*, std::set<DagNode*> >                orgNodesMoves;
        std::map<Monitor*, std::set<DagNode*> >             orgNodesMonitors;
//        unsigned int                                        tuningInterval;
        unsigned int                                        gen;
    };

}

#endif
