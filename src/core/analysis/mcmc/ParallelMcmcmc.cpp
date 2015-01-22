#include "ParallelMcmcmc.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbOptions.h" // included for omp.h ... don't quite understand why it doesn't propagate through from main.cpp...
#include <iostream>
#include <vector>
#include <cmath>

#ifdef RB_MPI
#include <mpi.h>
#endif

#define DEBUG_PMC3 0

using namespace RevBayesCore;

ParallelMcmcmc::ParallelMcmcmc(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons, std::string sT, size_t nc, size_t si, double dt, double st, double sh) : Cloneable( ), numChains(nc), scheduleType(sT), currentGeneration(0), swapInterval(si), delta(dt),  sigma(st), startingHeat(sh)
{
    activeIndex = 0;
    
    // initialize np, nc, pid
    size_t pid = 0;
    numProcesses = 1;
#ifdef RB_MPI
    numProcesses = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    if (numChains < numProcesses)
        numProcesses = numChains;
    
    // initialize container sizes
    chains.resize(numChains);
    chainsPerProcess.resize(numProcesses);
    chainValues.resize(numChains, 0.0);
    chainHeats.resize(numChains, 0.0);
    processPerChain.resize(numChains);
    
    // assign chains to processors, instantiate Mcmc objects
    for (size_t i = 0, j = 0; i < numChains; i++, j++)
    {
        // all chains know heat-order and chain-processor schedules
        chainIdxByHeat.push_back(i);
        if (j >= numProcesses)
            j = 0;
        chainsPerProcess[j].push_back(i);
        processPerChain[i] = j;
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid) {
            
            // get chain heat
            double b = computeBeta(delta,sigma,i) * startingHeat;
            
            // create chains
            bool a = (i == 0 ? true : false);
            Mcmc* oneChain = new Mcmc(m, moves, mons);
            oneChain->setScheduleType( scheduleType );
            oneChain->setChainActive( a );
            oneChain->setChainPosteriorHeat( b );
            oneChain->setChainIndex(i);
            oneChain->startMonitors(0);
            chains[i] = oneChain;
        }
        else {
//            std::cout << "pid " << pid << ": " << "no match\n";            
            chains[i] = NULL;
        }
    }
}

ParallelMcmcmc::ParallelMcmcmc(const ParallelMcmcmc &m)
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    sigma = m.sigma;
    delta = m.delta;
    startingHeat = m.startingHeat;
    numChains = m.numChains;
    numProcesses = m.numProcesses;

    chainIdxByHeat = m.chainIdxByHeat;
    swapInterval = m.swapInterval;
    activeIndex = m.activeIndex;
    scheduleType = m.scheduleType;

    chainsPerProcess.clear();
    for (size_t i = 0; i < m.chainsPerProcess.size(); i++)
    {
        chainsPerProcess.push_back(m.chainsPerProcess[i]);
    }
    
    chains.clear();
    chains.resize(numChains, NULL);
    for (size_t i = 0; i < m.chainsPerProcess[pid].size(); i++)
    {
        size_t k = m.chainsPerProcess[pid][i];
        chains[k] = new Mcmc( *(m.chains[k]));
    }
    
    chainValues = m.chainValues;
    chainHeats = m.chainHeats;
    processPerChain = m.processPerChain;
    
    currentGeneration = m.currentGeneration;
    
}

ParallelMcmcmc::~ParallelMcmcmc(void)
{
    for (size_t i = 0; i < chains.size(); i++)
    {
        delete chains[i];
    }
    chains.clear();
}

void ParallelMcmcmc::initialize(void)
{
    
}

double ParallelMcmcmc::computeBeta(double d, double s, size_t idx)
{
    // MJL: May want other distributions of beta in the future
    return pow(1 + d, -pow(idx,s));
}

void ParallelMcmcmc::burnin(int g, int ti)
{
//    // Tell monitors how much job we have ahead of us
//    for( size_t i = 0; i < chains.size(); ++i )
//    {
//        RbVector<Monitor>& monitors = chains[i]->getMonitors();
//        for ( size_t j = 0; j < monitors.size(); ++j )
//            monitors[j].setNumCycles( g );
//    }
}

ParallelMcmcmc* ParallelMcmcmc::clone(void) const
{
    return new ParallelMcmcmc(*this);
}

void ParallelMcmcmc::printOperatorSummary(void) const
{
    for (size_t i = 0; i < numChains; i++)
    {
        //std::cout << "\nChain " << i << ":\n";
        chains[i]->printOperatorSummary();
    }
}

void ParallelMcmcmc::run(size_t generations)
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    // Tell monitors how much job we have ahead of us
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        RbVector<Monitor>& monitors = chains[ chainsPerProcess[pid][i] ]->getMonitors();
        for ( size_t j = 0; j < monitors.size(); ++j )
        {
            monitors[j].reset( generations );
        }
    }
    
    // print file header
    if (currentGeneration == 0 && pid == 0)
        chains[0]->monitor(0);
    
    // run chain
    for (size_t i = 1; i <= generations; i += swapInterval)
    {
        // Create process per chain
        for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
        {
            // get chain index from job vector
            size_t chainIdx = chainsPerProcess[pid][j];
            
            // Advance cycles in blocks of size swapInterval
            for (size_t k = 0; k < swapInterval && (i+k) <= generations; k++)
            {
                // advance chain j by a single cycle
                chains[chainIdx]->nextCycle(true);

                // monitor chain activeIndex only
                if (chains[chainIdx]->isChainActive() )
                {
                    chains[chainIdx]->monitor(i+k);
                }
            }
        }
        
#ifdef RB_MPI
        // wait until all chains complete
        MPI::COMM_WORLD.Barrier();
#endif
        
        // advance gen counter
        currentGeneration += swapInterval;
        
        // perform chain swap
        swapChains();
    }
}

void ParallelMcmcmc::synchronizeValues(void)
{

    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    // synchronize chain values
    double results[numChains];
    for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
    {
        size_t k = chainsPerProcess[pid][j];
        results[k] = chains[k]->getModelLnProbability();
    }
#ifdef RB_MPI
    MPI::COMM_WORLD.Send(&results, numChains, MPI::DOUBLE, 0, 0);
#endif
    if (pid == 0)
    {
#ifdef RB_MPI
        for (size_t i = 0; i < numProcesses; i++)
        {
            double tmp_results[numChains];
            MPI::COMM_WORLD.Recv(&tmp_results, numChains, MPI::DOUBLE, i, 0);
            for (size_t j = 0; j < chainsPerProcess[i].size(); j++)
            {
                size_t k = chainsPerProcess[i][j];
                results[k] = tmp_results[k];
            }
        }
#endif
        for (size_t i = 0; i < chainValues.size(); i++)
        {
            chainValues[i] = results[i];
        }
    }

}

void ParallelMcmcmc::synchronizeHeats(void)
{

    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    // synchronize heat values
    double heats[numChains];
    for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
    {
        size_t k = chainsPerProcess[pid][j];
        heats[k] = chains[k]->getChainPosteriorHeat();
    }
#ifdef RB_MPI
    MPI::COMM_WORLD.Send(&heats, numChains, MPI::DOUBLE, 0, 0);
#endif
    if (pid == 0)
    {
#ifdef RB_MPI
        for (size_t i = 0; i < numProcesses; i++)
        {
            double tmp_heats[numChains];
            MPI::COMM_WORLD.Recv(&tmp_heats, numChains, MPI::DOUBLE, i, 0);
            for (size_t j = 0; j < chainsPerProcess[i].size(); j++)
            {
                size_t k = chainsPerProcess[i][j];
                heats[k] = tmp_heats[k];
            }
        }
#endif
        for (size_t i = 0; i < chainValues.size(); i++)
        {
            chainHeats[i] = heats[i];
        }
    }
}

void ParallelMcmcmc::updateChainState(size_t j)
{
#ifdef RB_MPI
    size_t pid = 0;
    pid = MPI::COMM_WORLD.Get_rank();

    if (pid != 0 || pid != j)
    {
        ;
    }
    else
    {
        // update heat
        if (pid == 0)
        {
            MPI::COMM_WORLD.Send(&chainHeats[j], 1, MPI::DOUBLE, processPerChain[j], 0);
            chains[j]->setChainHeat(chainHeats[j]);
        }
        if (pid == processPerChain[j])
        {
            MPI::COMM_WORLD.Recv(&chainHeats[j], 1, MPI::DOUBLE, 0, 0);
            chains[j]->setChainHeat(chainHeats[j]);
        }

        // update active state
        bool tf = activeIndex == j;
        if (pid == 0)
        {
            MPI::COMM_WORLD.Send(&tf, 1, MPI::BOOL, processPerChain[j], 0);
            chains[j]->setChainHeat(chainHeats[j]);
        }
        if (pid == processPerChain[j])
        {
            MPI::COMM_WORLD.Recv(&tf, 1, MPI::BOOL, 0, 0);
            chains[j]->setChainActive(tf);
        }
    }
    #endif
}

// MJL: allow swapChains to take a swap function -- e.g. pairwise swap for 1..n-1
void ParallelMcmcmc::swapChains(void)
{
    size_t numChains = chains.size();
    double lnProposalRatio = 0.0;
    
    // exit if there is only one chain
    if (numChains < 2)
        return;
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif

    // send all chain values to pid 0
    synchronizeValues();
    
    // send all chain heats to pid 0
    synchronizeHeats();
   
    size_t numAccepted = 0;

    //for (size_t i = 1; i < numChains; i++)
    for (size_t i = numChains-1; i > 0; i--)
    {
        // swap?
        bool accept = false;
        // swap adjacent chains
        size_t j = 0;
        size_t k = 0;

        if (pid == 0)
        {
            j = chainIdxByHeat[i-1];
            k = chainIdxByHeat[i];

            // compute exchange ratio
            double bj = chainHeats[j];
            double bk = chainHeats[k];
            double lnPj = chainValues[j];
            double lnPk = chainValues[k];
            double lnR = bj * (lnPk - lnPj) + bk * (lnPj - lnPk) + lnProposalRatio;
            
            // determine whether we accept or reject the chain swap
            double u = GLOBAL_RNG->uniform01();
            if (lnR >= 0)
                accept = true;
            else if (lnR < -100)
                accept = false;
            else if (u < exp(lnR))
                accept = true;
            else
                accept = false;
            
            if (accept == true)
                numAccepted++;
            
            // test override
            //accept = true;
    #if DEBUG_PMC3
            std::cout << "\nbj " << bj << "; bk " << bk << "; lnPj " << lnPj << "; lnPk " << lnPk << "\n";
            std::cout << "bj*(lnPk-lnPj) " << bj*(lnPk-lnPj) << "; bk*(lnPj-lnPk) " << bk*(lnPj-lnPk) << "\n";
            std::cout << "swapChains()\t" << j << " <--> " << k << "  " << lnR << "\n";
            std::cout << u << "  " << exp(lnR) << "  " << (accept ? "accept\n" : "reject\n");
    #endif

            // on accept, swap beta values and active chains
            if (accept)
            {
                
                //size_t tmpIdx = j;
                chainIdxByHeat[i-1] = k;
                chainIdxByHeat[i] = j;
                
                // swap active chain
                if (activeIndex == j)
                {
                    activeIndex = k;
                }
                else if (activeIndex == k)
                {
                    activeIndex = j;
                }
            }
        }
        
        if (accept)
        {
            updateChainState(j);
            updateChainState(k);
        }
    }
    
#if DEBUG_PMC3
    
    int nc = (numChains < 10 || true ? numChains : 10);
    for (int j = 0; j < nc; j++)
    {
        int i = chainIdxByHeat[j];
        std::cout << i << " " << chains[i]->getChainHeat() << " * " << chains[i]->getLnPosterior() << " = " << chains[i]->getChainHeat() * chains[i]->getLnPosterior() << "\n";
        //chains[i]->getModelLnProbability() << " " << (chains[i]->isChainActive() ? "*" : "") << (i == activeIndex ? "#" : "") << "\n";
    }
    std::cout << "freq accepted: " << (double)numAccepted/(numChains-1) << "\n";
    
    std::cout << "\n";
# endif
    
}
