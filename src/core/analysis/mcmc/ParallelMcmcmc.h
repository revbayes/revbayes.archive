//
//  ParallelMcmcmc.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/20/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__ParallelMcmcmc__
#define __rb_mlandis__ParallelMcmcmc__

#include "Mcmc.h"
#include "Model.h"
#include "Monitor.h"
#include "Move.h"

#include <vector>

namespace RevBayesCore {
    
    class ParallelMcmcmc {
        
    public:
        ParallelMcmcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, int numProc, int si, double dt);
        ParallelMcmcmc(const ParallelMcmcmc &m);
        virtual                                            ~ParallelMcmcmc(void);                                                          //!< Virtual destructor
        
        // public methods
        void                                                burnin(int g, int ti);
        ParallelMcmcmc*                                     clone(void) const;
        void                                                printOperatorSummary(void) const;
        void                                                run(int g);        
        
    private:
        void                                                initialize(void);
        void                                                swapChains(void);
        double                                              computeBeta(double d, int i);   // incremental temperature schedule
        
        // MCMC variables
        int                                                 numChains;
        std::vector<Mcmc*>                                  chains;
        unsigned int                                        gen;
        unsigned int                                        swapInterval;
        
        // MCMCMC variables
        int                                                 activeIndex; // index of coldest chain, i.e. which one samples the posterior
        double                                              delta;       // delta-T, temperature increment for computeBeta
        
    };
    
}


#endif /* defined(__rb_mlandis__ParallelMcmcmc__) */
