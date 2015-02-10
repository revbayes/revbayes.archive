#include "Mcmcmc.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RlUserInterface.h"
#include "RbConstants.h"
#include "RbException.h"

#include <iostream>
#include <vector>
#include <cmath>

#ifdef RB_MPI
#include <mpi.h>
#endif

#define DEBUG_PMC3 0

using namespace RevBayesCore;

Mcmcmc::Mcmcmc(const Model& m, const RbVector<Move> &moves, const RbVector<Monitor> &mons, std::string sT, size_t nc, size_t si, double dt) : MonteCarloSampler( ),
    pid(0),
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
        heatRanks.push_back(i);
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
            oneChain->setChainPosteriorHeat( b );
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
    
    pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    delta               = m.delta;
    numChains           = m.numChains;
    numProcesses        = m.numProcesses;
    
    heatRanks           = m.heatRanks;
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


Mcmcmc* Mcmcmc::clone(void) const
{
    return new Mcmcmc(*this);
}


double Mcmcmc::getModelLnProbability( void )
{
    synchronizeValues();
    
    for (size_t i=0; i<chains.size(); i++)
    {
        if ( chainHeats[i] == 1.0 )
        {
            return chainValues[i];
        }
    }
    
    return RbConstants::Double::neginf;
}

std::string Mcmcmc::getStrategyDescription( void ) const
{
    std::string description = "";
    std::stringstream stream;
    stream << "The MCMCMC simulator runs 1 cold chain and " << (numChains-1) << " heated chains.\n";
    stream << chains[ chainsPerProcess[pid][0] ]->getStrategyDescription();
    description = stream.str();
    
    return description;
}


void Mcmcmc::initializeSampler( bool priorOnly )
{
    
    // initialize each chain
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->initializeSampler( priorOnly );
    }
    
}


void Mcmcmc::monitor(unsigned long g)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        if (chains[ chainsPerProcess[pid][i] ]->isChainActive() )
        {
            chains[ chainsPerProcess[pid][i] ]->monitor(g);
        }
    }
    
}

void Mcmcmc::nextCycle(bool advanceCycle)
{
    
    // run each chain for this process
    for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
    {
        // get chain index from job vector
        size_t chainIdx = chainsPerProcess[pid][j];
        
        // advance chain j by a single cycle
        chains[chainIdx]->nextCycle( advanceCycle );
        
    } // loop over chains for this process
    
    if ( advanceCycle == true )
    {
        // advance gen counter
        ++currentGeneration;
    }
    
    if ( currentGeneration % swapInterval == 0 )
    {
#ifdef RB_MPI
        // wait until all chains complete
        MPI::COMM_WORLD.Barrier();
#endif
        
        // perform chain swap
        swapChains();
    }
    
}

void Mcmcmc::printOperatorSummary(void) const
{
    for (size_t i = 0; i < numChains; i++)
    {
        chains[i]->printOperatorSummary();
    }
}


void Mcmcmc::reset( void )
{
    
    // reset counters
    numAcceptedSwaps = 0;
    numAttemptedSwaps = 0;
    
//    /* Reset the monitors */
//    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
//    {
//        RbVector<Monitor>& monitors = chains[ chainsPerProcess[pid][i] ]->getMonitors();
//        for (size_t i=0; i<monitors.size(); i++)
//        {
//            monitors[i].reset();
//        }
//    }
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->reset();
    }

    
}


/**
 * Set the heat of the likelihood of the current chain.
 * This heat is used in posterior posterior MCMC algorithms to
 * heat the likelihood
 * The heat is passed to the moves for the accept-reject mechanism.
 */
void Mcmcmc::setLikelihoodHeat(double h)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->setLikelihoodHeat( h );
    }
    
}


void Mcmcmc::setReplicateIndex(size_t index)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->setReplicateIndex(index);
    }
}


void Mcmcmc::setStoneIndex(size_t index)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->setStoneIndex(index);
    }
}



void Mcmcmc::startMonitors( void )
{
    
    // Monitor
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        
        chains[ chainsPerProcess[pid][i] ]->startMonitors();
        
    }
    
}


void Mcmcmc::startMonitors(size_t numCycles)
{
    
    size_t pid = 0;
#ifdef RB_MPI
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    // Monitor
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        
        chains[ chainsPerProcess[pid][i] ]->startMonitors( numCycles );
        
        // monitor chain activeIndex only
        if (chains[ chainsPerProcess[pid][i] ]->isChainActive() )
        {
            chains[ chainsPerProcess[pid][i] ]->monitor(0);
        }
        
    }
    
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
        heats[k] = chains[k]->getChainPosteriorHeat();
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
            
            j = heatRanks[i-1];
            k = heatRanks[i];
            
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
                heatRanks[i-1] = k;
                heatRanks[i] = j;
                
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
        j = int(GLOBAL_RNG->uniform01() * numChains);
        if (numChains > 1)
        {
            do {
                k = int(GLOBAL_RNG->uniform01() * numChains);
            }
            while(j == k);
        }
        
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
            size_t tmp = heatRanks[j];
            heatRanks[j] = heatRanks[k];
            heatRanks[k] = tmp;
            
            ++numAcceptedSwaps;
        }
        
        
    }
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, 0);
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, 0);
#endif
    
    // update the chains accross processes
    // this is necessary because only process 0 does the swap
    // all the other processes need to be told that there was a swap
    updateChainState(j);
    updateChainState(k);
    
}


void Mcmcmc::tune( void )
{
    
    double rate = numAcceptedSwaps / double(numAttemptedSwaps);
    
    if ( rate > 0.44 )
    {
        delta *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        delta /= (2.0 - rate/0.44 );
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
        chains[j]->setChainPosteriorHeat(chainHeats[j]);
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
    
//    size_t rank = heatRanks[j];
//#ifdef RB_MPI
//    MPI::COMM_WORLD.Bcast(&rank, 1, MPI::DOUBLE, 0);
//#endif
//    heatRanks[j] = rank;
    
    
}

