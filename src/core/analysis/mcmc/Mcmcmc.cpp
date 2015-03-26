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

using namespace RevBayesCore;

Mcmcmc::Mcmcmc(const Model& m, const RbVector<Move> &mv, const RbVector<Monitor> &mn, std::string sT, size_t nc, size_t si, double dt) : MonteCarloSampler( ),
    numChains(nc),
    scheduleType(sT),
    currentGeneration(0),
    swapInterval(si),
    activeChainIndex( 0 ),
    delta( dt ),
    generation( 0 ),
    numAttemptedSwaps( 0 ),
    numAcceptedSwaps( 0 )
{
    
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
    baseChain = new Mcmc(m, mv, mn);
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
        if (j == pid-activePID)
        {
            
            // get chain heat
            double b = computeBeta(delta,i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *oneChain );
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

Mcmcmc::Mcmcmc(const Mcmcmc &m) : MonteCarloSampler(m)
{
    
    delta               = m.delta;
    numChains           = m.numChains;
    numProcesses        = m.numProcesses;
    heatRanks           = m.heatRanks;
    swapInterval        = m.swapInterval;
    activeChainIndex    = m.activeChainIndex;
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
    for (size_t i = 0; i < m.chainsPerProcess[pid-activePID].size(); i++)
    {
        size_t k    = m.chainsPerProcess[pid-activePID][i];
        chains[k]   = m.chains[k]->clone();
        std::cout << pid << " copy ctor " << i << " " << chains[k] << "\n";
    }
    
    chainValues         = m.chainValues;
    chainHeats          = m.chainHeats;
    processPerChain     = m.processPerChain;
    
    currentGeneration   = m.currentGeneration;
    baseChain           = m.baseChain;
    
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
    stream << chains[ chainsPerProcess[pid-activePID][0] ]->getStrategyDescription();
    description = stream.str();
    
    return description;
}


void Mcmcmc::initializeSampler( bool priorOnly )
{
    
    // initialize each chain
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        chains[ chainsPerProcess[pid-activePID][i] ]->initializeSampler( priorOnly );
    }
    
}


void Mcmcmc::monitor(unsigned long g)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        if (chains[ chainsPerProcess[pid-activePID][i] ]->isChainActive() )
        {
            chains[ chainsPerProcess[pid-activePID][i] ]->monitor(g);
        }
    }
    
}

void Mcmcmc::nextCycle(bool advanceCycle)
{
    
    // run each chain for this process
    for (size_t j = 0; j < chainsPerProcess[pid-activePID].size(); j++)
    {
        // get chain index from job vector
        size_t chainIdx = chainsPerProcess[pid-activePID][j];
        
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
    
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        chains[ chainsPerProcess[pid-activePID][i] ]->reset();
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
    
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        chains[ chainsPerProcess[pid-activePID][i] ]->setLikelihoodHeat( h );
    }
    
}

void Mcmcmc::setNumberOfProcesses(size_t n)
{
    MonteCarloSampler::setNumberOfProcesses(n);    
    
    
    // @MJL: Note to self. The ctor assumes numProcesses==1, so all chains are assigned to that processor.
    // After cloning all chains across processors, you will then want to thin out the chains as needed.
    // This should behave much like the old Mcmcmc ctor code, except it cannot assume a fresh object state.
    
    // only use a many processes as we have chains
    if (numChains < numProcesses)
    {
        numProcesses = numChains;
    }
        
    // initialize container sizes
    for (size_t i = 0; i < chains.size(); i++)
    {
        delete chains[i];
    }
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
        if (j == pid-activePID)
        {
            
            // get chain heat
            double b = computeBeta(delta,i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *baseChain );
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


void Mcmcmc::setReplicateIndex(size_t index)
{
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        chains[ chainsPerProcess[pid-activePID][i] ]->setReplicateIndex(index);
    }
}


void Mcmcmc::setStoneIndex(size_t index)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        chains[ chainsPerProcess[pid-activePID][i] ]->setStoneIndex(index);
    }
}



void Mcmcmc::startMonitors( void )
{
    
    // Monitor
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        
        chains[ chainsPerProcess[pid-activePID][i] ]->startMonitors();
        
    }
    
}


void Mcmcmc::startMonitors(size_t numCycles)
{
    
    // Monitor
    for (size_t i = 0; i < chainsPerProcess[pid-activePID].size(); i++)
    {
        
        chains[ chainsPerProcess[pid-activePID][i] ]->startMonitors( numCycles );
        
        // monitor chain activeIndex only
        if (chains[ chainsPerProcess[pid-activePID][i] ]->isChainActive() )
        {
            chains[ chainsPerProcess[pid-activePID][i] ]->monitor(0);
        }
        
    }
    
}


void Mcmcmc::synchronizeValues(void)
{
    
    // synchronize chain values
    double results[numChains];
    for (size_t j = 0; j < chainsPerProcess[pid-activePID].size(); j++)
    {
        size_t k = chainsPerProcess[pid-activePID][j];
        results[k] = chains[k]->getModelLnProbability();
    }
#ifdef RB_MPI
    MPI::COMM_WORLD.Send(&results, numChains, MPI::DOUBLE, activePID, 0);
#endif
    
    if ( processActive == true )
    {
#ifdef RB_MPI
        if ( processActive == true )
        {
            for (size_t i = 0; i < numProcesses; i++)
            {
                double tmp_results[numChains];
                MPI::COMM_WORLD.Recv(&tmp_results, numChains, MPI::DOUBLE, activePID+i, 0);
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
    
    // synchronize heat values
    double heats[numChains];
    for (size_t j = 0; j < chainsPerProcess[pid-activePID].size(); j++)
    {
        size_t k = chainsPerProcess[pid-activePID][j];
        heats[k] = chains[k]->getChainPosteriorHeat();
    }
    
    
#ifdef RB_MPI
    // share the heats accross processes
    MPI::COMM_WORLD.Send(&heats, numChains, MPI::DOUBLE, activePID, 0);
#endif
    
    if ( processActive == true )
    {
#ifdef RB_MPI
        for (size_t i = 0; i < numProcesses; i++)
        {
            double tmp_heats[numChains];
            MPI::COMM_WORLD.Recv(&tmp_heats, numChains, MPI::DOUBLE, activePID+i, 0);
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
            {
                accept = true;
            }
            else if (lnR < -100)
            {
                accept = false;
            }
            else if (u < exp(lnR))
            {
                accept = true;
            }
            else
            {
                accept = false;
            }
            
            if (accept == true)
            {
                numAccepted++;
            }
            
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
    
}


void Mcmcmc::swapRandomChains(void)
{
    
    double lnProposalRatio = 0.0;
    
    // randomly pick the indices of two chains
    int j = 0;
    int k = 0;
    
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
        {
            accept = true;
        }
        else if (lnR < -100)
        {
            accept = false;
        }
        else if (u < exp(lnR))
        {
            accept = true;
        }
        else
        {
            accept = false;
        }
        
        
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
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, activePID);
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, activePID);
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
    
#ifdef RB_MPI
    
    // update heat
    if (processActive == true)
    {
        MPI::COMM_WORLD.Send(&chainHeats[j], 1, MPI::DOUBLE, processPerChain[j], activePID);
    }
    if (pid-activePID == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&chainHeats[j], 1, MPI::DOUBLE, activePID, 0);
    }
#endif
    
    if (pid-activePID == processPerChain[j])
    {
        chains[j]->setChainPosteriorHeat(chainHeats[j]);
    }
    // update active state
    bool tf = activeChainIndex == j;
    
#ifdef RB_MPI
    if (processActive == true)
    {
        MPI::COMM_WORLD.Send(&tf, 1, MPI::BOOL, processPerChain[j], activePID);
    }
    if (pid-activePID == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&tf, 1, MPI::BOOL, activePID, 0);
    }
#endif
    
    if (pid-activePID == processPerChain[j])
    {
        chains[j]->setChainActive(tf);
    }
    
    
}

