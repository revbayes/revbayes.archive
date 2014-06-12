/**
 * @file
 * This file contains the declaration of the PowerPosteriorMcmc class. 
 * An PowerPosteriorMcmc object manages the marginal likelihood computation for a given model
 * using either the stepping stone sampling or path sampling.
 *
 * @brief Declaration of PowerPosteriorMcmc class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-06-17
 *
 * $Id$
 */


#ifndef PowerPosteriorMcmc_H
#define PowerPosteriorMcmc_H

#include "Model.h"
#include "Monitor.h"
#include "MoveInterface.h"

#include <vector>

namespace RevBayesCore {
    
    class PowerPosteriorMcmc {
        
    public:
        PowerPosteriorMcmc(const Model& m, const std::vector<MoveInterface*> &moves);
        PowerPosteriorMcmc(const PowerPosteriorMcmc &m);
        virtual                                            ~PowerPosteriorMcmc(void);                                                          //!< Virtual destructor
        
        // public methods
        PowerPosteriorMcmc*                                 clone(void) const;
        void                                                burnin(size_t g, size_t ti);
        const std::vector<double>&                          getSamples(void) const;
        double                                              pathSampling();
        void                                                run(size_t g);
        double                                              steppingStoneSampling();
        
//        void                                                setGenerations(size_t g);
        void                                                setBeta(const std::vector<double> &b);
        void                                                setSampleFreq(size_t sf);
        
    private:
        void                                                initialize(void);
        
        // members       
        double                                              lnProbability;
        Model                                               model;
        std::vector<MoveInterface*>                         moves;
        std::map<MoveInterface*, std::set<DagNode*> >       orgNodesMoves;
    
        std::vector<double>                                 beta;
        size_t                                              sampleFreq;
        std::vector<double>                                 samples;
        size_t                                              samplesPerPath;
    };
    
}

#endif
