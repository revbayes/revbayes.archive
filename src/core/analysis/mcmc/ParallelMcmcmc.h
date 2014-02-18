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
#include "RbOptions.h" // included for omp.h ... don't quite understand why it doesn't propagate through from main.cpp...

#include <vector>

namespace RevBayesCore {
    
    class ParallelMcmcmc {
        
    public:
        ParallelMcmcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, int nc=4, int np=4, int si=1000, double dt=0.1, double sh=1.0);
        ParallelMcmcmc(const ParallelMcmcmc &m);
        virtual                                            ~ParallelMcmcmc(void);                                                          //!< Virtual destructor
        
        // public methods
        void                                                burnin(int g, int ti);
        ParallelMcmcmc*                                     clone(void) const;
        void                                                printOperatorSummary(void) const;
        void                                                run(size_t g);        
        
    private:
        void                                                initialize(void);
        void                                                swapChains(void);
        double                                              computeBeta(double d, int i);   // incremental temperature schedule
        
        size_t                                              numChains;
        size_t                                              numProcesses;
        std::vector<size_t>                                    chainIdxByHeat;
        std::vector<std::vector<size_t> >                      chainsPerProcess;
        std::vector<Mcmc*>                                  chains;
        unsigned int                                        gen;
        unsigned int                                        swapInterval;
        
        int                                                 activeIndex; // index of coldest chain, i.e. which one samples the posterior
        double                                              delta;       // delta-T, temperature increment for computeBeta
        double                                              startingHeat; // default 1.0
        
    };
    
}


#endif /* defined(__rb_mlandis__ParallelMcmcmc__) */
