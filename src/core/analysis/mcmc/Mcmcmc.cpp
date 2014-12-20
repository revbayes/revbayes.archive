#include "Mcmcmc.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RlUserInterface.h"
#include "RbException.h"

#include <iostream>
#include <vector>
#include <cmath>

#ifdef RB_MPI
#include <mpi.h>
#endif

#define DEBUG_PMC3 0

using namespace RevBayesCore;

Mcmcmc::Mcmcmc(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons, std::string sT, size_t nc, size_t si, double dt) : Cloneable( ),
    numChains(nc),
    scheduleType(sT),
    currentGeneration(0),
    swapInterval(si),
    activeChainIndex( 0 ),
    processActive( true ),
    delta( dt ),
    generation( 0 ),
    numAttemptedSwaps( 0 ),
    numAcceptedSwaps( 0 )
{
    
    // initialize np, nc, pid
    size_t pid = 0;
    numProcesses = 1;
    
#ifdef RB_MPI
    numProcesses = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    processActive = (pid == 0);
    
    // only use a many processes as we have chains
    if (numChains < numProcesses)
    {
        numProcesses = numChains;
    }
    
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
        {
            j = j % numProcesses;
        }
        
        chainsPerProcess[j].push_back(i);
        processPerChain[i] = j;
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid)
        {
            
            // get chain heat
            double b = computeBeta(delta,i);
            
            // create chains
            Mcmc* oneChain = new Mcmc(m, moves, mons);
            oneChain->setScheduleType( scheduleType );
            oneChain->setChainActive( i == 0 );
            oneChain->setChainHeat( b );
            oneChain->setChainIndex( i );
            chains[i] = oneChain;
        }
        else
        {
            chains[i] = NULL;
        }
    }
    
}

Mcmcmc::Mcmcmc(const Mcmcmc &m)
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    delta               = m.delta;
    numChains           = m.numChains;
    numProcesses        = m.numProcesses;
    
    chainIdxByHeat      = m.chainIdxByHeat;
    swapInterval        = m.swapInterval;
    activeChainIndex    = m.activeChainIndex;
    processActive       = m.processActive;
    scheduleType        = m.scheduleType;
    
    numAttemptedSwaps   = m.numAttemptedSwaps;
    numAcceptedSwaps    = m.numAcceptedSwaps;
    generation          = m.generation;
    
    chainsPerProcess.clear();
    for (size_t i = 0; i < m.chainsPerProcess.size(); i++)
    {
        chainsPerProcess.push_back(m.chainsPerProcess[i]);
    }
    
    chains.clear();
    chains.resize(numChains, NULL);
    for (size_t i = 0; i < m.chainsPerProcess[pid].size(); i++)
    {
        size_t k    = m.chainsPerProcess[pid][i];
        chains[k]   = m.chains[k]->clone();
    }
    
    chainValues         = m.chainValues;
    chainHeats          = m.chainHeats;
    processPerChain     = m.processPerChain;
    
    currentGeneration   = m.currentGeneration;
    
}

Mcmcmc::~Mcmcmc(void)
{
    for (size_t i = 0; i < chains.size(); i++)
    {
        delete chains[i];
    }
    chains.clear();
    
}

void Mcmcmc::initialize(void)
{
    
}

double Mcmcmc::computeBeta(double d, size_t idx)
{

    return 1.0 / (1.0+delta*idx);
}

void Mcmcmc::burnin(int g, int ti)
{
    //    // Tell monitors how much job we have ahead of us
    //    for( size_t i = 0; i < chains.size(); ++i )
    //    {
    //        RbVector<Monitor>& monitors = chains[i]->getMonitors();
    //        for ( size_t j = 0; j < monitors.size(); ++j )
    //            monitors[j].setNumCycles( g );
    //    }
    throw RbException("No burnin in MC^3");
}

Mcmcmc* Mcmcmc::clone(void) const
{
    return new Mcmcmc(*this);
}

void Mcmcmc::printOperatorSummary(void) const
{
    for (size_t i = 0; i < numChains; i++)
    {
        chains[i]->printOperatorSummary();
    }
}

void Mcmcmc::run(size_t kIterations)
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    /* Let user know what we are doing */
    if ( generation == 0 )
    {
        std::stringstream ss;
        ss << "\nRunning MCMC simulation for " << kIterations << " iterations";
        RBOUT( ss.str() );
    }
    else
    {
        std::stringstream ss;
        ss << "Appending " << kIterations << " iterations to previous MCMC simulation of " << generation << " iterations";
        RBOUT( ss.str() );
    }
    
    if ( scheduleType == "single" )
    {
        std::stringstream ss;
        if ( chainsPerProcess[pid].size() > 0 )
        {
            RbVector<Move>& moves = chains[ chainsPerProcess[pid][0] ]->getMoves();
            ss << "The simulator uses " << moves.size() << " different moves, with a single move picked randomly per iteration" << std::endl;
        }
        else
        {
            ss << " ";
        }
        RBOUT( ss.str() );
    }
    else if ( scheduleType == "random" )
    {
        std::stringstream ss;
        if ( chainsPerProcess[pid].size() > 0 )
        {
            RbVector<Move>& moves = chains[ chainsPerProcess[pid][0] ]->getMoves();
            ss << "The simulator uses " << moves.size() << " different moves in a random move schedule with " << chains[ chainsPerProcess[pid][0] ]->getSchedule().getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        }
        else
        {
            ss << " ";
        }
        RBOUT( ss.str() );
    }
    else if ( scheduleType == "sequential" )
    {
        std::stringstream ss;
        if ( chainsPerProcess[pid].size() > 0 )
        {
            RbVector<Move>& moves = chains[ chainsPerProcess[pid][0] ]->getMoves();
            ss << "The simulator uses " << moves.size() << " different moves in a sequential move schedule with " << chains[ chainsPerProcess[pid][0] ]->getSchedule().getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        }
        else
        {
            ss << " ";
        }
        RBOUT( ss.str() );
    }
    
    if ( generation == 0 )
    {
        // Monitor
        for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
        {
            
            chains[ chainsPerProcess[pid][i] ]->startMonitors( kIterations );
            
            // monitor chain activeIndex only
            if (chains[ chainsPerProcess[pid][i] ]->isChainActive() )
            {
                chains[ chainsPerProcess[pid][i] ]->monitor(0);
            }
            
        }
        
    }
    
    /* Reset the monitors */
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        RbVector<Monitor>& monitors = chains[ chainsPerProcess[pid][i] ]->getMonitors();
        for (size_t i=0; i<monitors.size(); i++)
        {
            monitors[i].reset( kIterations );
        }
    }
    
    // reset the counters for the move schedules
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        RbVector<Move>& moves = chains[ chainsPerProcess[pid][i] ]->getMoves();
        for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
        {
            it->resetCounters();
        }
    }
    
    // run chain
    for (size_t i = 1; i <= kIterations; i += swapInterval)
    {
        // run each chain for this process
        for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
        {
            // get chain index from job vector
            size_t chainIdx = chainsPerProcess[pid][j];
            
            // Advance cycles in blocks of size swapInterval
            for (size_t k = 0; k < swapInterval && (i+k) <= kIterations; k++)
            {
                // advance chain j by a single cycle
                chains[chainIdx]->nextCycle(true);
                
                // monitor chain activeIndex only
                if (chains[chainIdx]->isChainActive() )
                {
                    chains[chainIdx]->monitor(i+k);
                }
                
            } // loop over iterations for this swap interval
            
        } // loop over chains for this process
        
#ifdef RB_MPI
        // wait until all chains complete
        MPI::COMM_WORLD.Barrier();
#endif
        
        // advance gen counter
        currentGeneration += swapInterval;
        
        // perform chain swap
        
        swapChains();
        
    } // loop over generations
    
    generation += kIterations;
    
}

void Mcmcmc::synchronizeValues(void)
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
    
    if ( processActive == true )
    {
#ifdef RB_MPI
        if ( processActive == true )
        {
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
        }
#endif
        for (size_t i = 0; i < chainValues.size(); i++)
        {
            chainValues[i] = results[i];
        }
    }
    
}

void Mcmcmc::synchronizeHeats(void)
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
        heats[k] = chains[k]->getChainHeat();
    }
    
    
#ifdef RB_MPI
    // share the heats accross processes
    MPI::COMM_WORLD.Send(&heats, numChains, MPI::DOUBLE, 0, 0);
#endif
    
    if ( processActive == true )
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

void Mcmcmc::updateChainState(size_t j)
{
    
    size_t pid = 0;
    
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
    
    // update heat
    if (processActive == true)
    {
        MPI::COMM_WORLD.Send(&chainHeats[j], 1, MPI::DOUBLE, processPerChain[j], 0);
    }
    if (pid == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&chainHeats[j], 1, MPI::DOUBLE, 0, 0);
    }
#endif
    
    if (pid == processPerChain[j])
    {
        chains[j]->setChainHeat(chainHeats[j]);
    }
    // update active state
    bool tf = activeChainIndex == j;
    
#ifdef RB_MPI
    if (processActive == true)
    {
        MPI::COMM_WORLD.Send(&tf, 1, MPI::BOOL, processPerChain[j], 0);
    }
    if (pid == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&tf, 1, MPI::BOOL, 0, 0);
    }
#endif
    
    if (pid == processPerChain[j])
    {
        chains[j]->setChainActive(tf);
    }
    
}

// MJL: allow swapChains to take a swap function -- e.g. pairwise swap for 1..n-1
void Mcmcmc::swapChains(void)
{
    size_t numChains = chains.size();
    
    // exit if there is only one chain
    if (numChains < 2)
    {
        return;
    }
    
    // send all chain values to pid 0
    synchronizeValues();
    
    // send all chain heats to pid 0
    synchronizeHeats();
    
    // swap chains
//    swapNeighborChains();
    swapRandomChains();

}


void Mcmcmc::swapNeighborChains(void)
{
    
    size_t numAccepted = 0;
    double lnProposalRatio = 0.0;
    
    //for (size_t i = 1; i < numChains; i++)
    for (size_t i = numChains-1; i > 0; i--)
    {
        // swap?
        bool accept = false;
        // swap adjacent chains
        size_t j = 0;
        size_t k = 0;
        
        if (processActive == true)
        {
            ++numAttemptedSwaps;
            
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
                if (activeChainIndex == j)
                {
                    activeChainIndex = k;
                }
                else if (activeChainIndex == k)
                {
                    activeChainIndex = j;
                }
                
            }
        }
        
        if (accept)
        {
            updateChainState(j);
            updateChainState(k);
            
            ++numAcceptedSwaps;
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


void Mcmcmc::swapRandomChains(void)
{
    
    double lnProposalRatio = 0.0;
    
    // randomly pick the indices of two chains
    int j = 0;
    int k = 0;
//    if ( k >= j)
//    {
//        ++k;
//    }
    
    // swap?
    bool accept = false;
    
    if ( processActive == true )
    {
        j = int(GLOBAL_RNG->uniform01() * (numChains-1));
        k = int(GLOBAL_RNG->uniform01() * (numChains-j-1)) + j + 1;
        
        ++numAttemptedSwaps;
            
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
        
        
        // on accept, swap beta values and active chains
        if (accept == true )
        {
            
            // swap active chain
            if (activeChainIndex == j)
            {
                activeChainIndex = k;
            }
            else if (activeChainIndex == k)
            {
                activeChainIndex = j;
            }
            
            chainHeats[j] = bk;
            chainHeats[k] = bj;

            ++numAcceptedSwaps;
        }
        
        
    }
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, 0);
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, 0);
#endif
    
//    if ( accept == true )
//    {
        // update the chains accross processes
        // this is necessary because only process 0 does the swap
        // all the other processes need to be told that there was a swap
        updateChainState(j);
        updateChainState(k);
        
//    }
    
}
