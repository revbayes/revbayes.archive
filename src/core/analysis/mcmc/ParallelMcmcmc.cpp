//
//  ParallelMcmcmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/20/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "ParallelMcmcmc.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include <vector>
#include <cmath>

#if defined (USE_LIB_OPENMP)
    #include <omp.h>
#endif

using namespace RevBayesCore;

ParallelMcmcmc::ParallelMcmcmc(const Model& m, const std::vector<Move*> &moves, const std::vector<Monitor*> &mons, int nc, int si, double dt) : delta(dt), numChains(nc), swapInterval(si), gen(0)
{

    activeIndex = 0;
    
    for (int i = 0; i < numChains; i++)
    {
        // get chain heat
        double b = computeBeta(delta,i);
      
        // create inactive chain
        Mcmc* oneChain = new Mcmc(m, moves, mons, false, b);
        chains.push_back(oneChain);
    }
    chains[activeIndex]->setChainActive(true);
    
    // assign chains to processors
    int np = 0;
    #if defined (USE_LIB_OPENMP)
        np = omp_get_num_threads();
    #endif
    numProcesses = np;

    chainsPerProcess.resize(numProcesses,std::vector<int>());
    int pIdx = 0;
    for (int i = 0; i < numChains; i++)
    {
        chainsPerProcess[pIdx++].push_back(i);
        if (pIdx > numProcesses)
            pIdx = 0;
    }
    
}

ParallelMcmcmc::ParallelMcmcmc(const ParallelMcmcmc &m)
{   
    // MJL: Uncertain if I need a copy ctor for Analysis objects (non-DAG)
}

ParallelMcmcmc::~ParallelMcmcmc(void)
{
    
}

double ParallelMcmcmc::computeBeta(double delta, int idx)
{
    // MJL: May want other distributions of beta in the future
    return 1 / (1 + delta * idx);
}

void ParallelMcmcmc::burnin(int g, int ti)
{
    
}

ParallelMcmcmc* ParallelMcmcmc::clone(void) const
{
    // MJL: I think this is handled through Mcmc...
}

void ParallelMcmcmc::printOperatorSummary(void) const
{
    
}

void ParallelMcmcmc::run(int generations)
{
    // print file header
    if (gen == 0)
        chains[0]->monitor(0);
    
    // run chain
    for (int i = 1; i <= generations; i += swapInterval)
    {
        // start parallel job per block of swapInterval cycles
        int np = numProcesses;
        int pid = 1;
        #pragma omp parallel default(shared) private(np, pid)
        {
            #if defined (USE_LIB_OPENMP)
                pid = omp_get_thread_num();
            #endif
            printf("Hello from thread %d out of %d\n", pid, np);
            
            // Create process per chain
            for (int j = 0; j < chainsPerProcess[pid].size(); j++)
            {
                // get chain index from job vector
                int chainIdx = chainsPerProcess[pid][j];
                
                // Advance cycles in blocks of size swapInterval
                for (int k = 0; k < swapInterval && (i+k) <= generations; k++)
                {
                    // advance chain j by a single cycle
                    chains[chainIdx]->nextCycle(true);

                    // monitor chain activeIndex only
                    if (j == activeIndex)
                        chains[activeIndex]->monitor(i + k);
                }
            }
            
            // synchronize chains
            #pragma omp barrier
            
        } // processor job end
        
        // advance gen counter
        gen += swapInterval;
        
        // perform chain swap
        swapChains();
    }
}

void ParallelMcmcmc::initialize(void)
{
    
}

void ParallelMcmcmc::swapChains(void)
{
    size_t numChains = chains.size();
    
    // exit if there is only one chain
    if (numChains < 2)
        return;
    
    // otherwise, swap adjacent chains
    size_t j = GLOBAL_RNG->uniform01() * numChains;
    size_t k = 0;
    if (j == 0)
        k = 1;
    else if (j == numChains-1)
        k = numChains-2;
    else
        k = (GLOBAL_RNG->uniform01() < 0.5 ? k = numChains-1 : k = numChains+1);
    // MJL: proposal density is not symmetric for border cases, currently unaccounted for
    
    // compute exchange ratio
    double bj = chains[j]->getChainHeat();
    double bk = chains[k]->getChainHeat();
    double lnLj = chains[j]->getLnProbability();
    double lnLk = chains[k]->getLnProbability();
    double r = bj * lnLk + bk * lnLj - bj * lnLj - bk * lnLk;
    
    // determine whether we accept or reject the chain swap
    bool accept = false;
    if (r > 1)
        accept = true;
    else if (r < -100)
        accept = false;
    else if (exp(r) < GLOBAL_RNG->uniform01())
        accept = true;
    else
        accept = false;
    
    // on accept, swap beta values and active chains
    if (accept)
    {
        // swap chain heat values
        chains[j]->setChainHeat(bk);
        chains[k]->setChainHeat(bj);

        // swap active chain
        bool active = chains[j]->isChainActive();
        chains[j]->setChainActive(chains[k]->isChainActive());
        chains[k]->setChainActive(active);
    }
}
