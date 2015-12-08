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
    if (numChains < num_processes)
    {
        num_processes = numChains;
    }
    
    // initialize container sizes
    chains.resize(numChains);
    chainsPerProcess.resize(num_processes);
    chainValues.resize(numChains, 0.0);
    chainHeats.resize(numChains, 0.0);
    processPerChain.resize(numChains);
    
    // assign chains to processors, instantiate Mcmc objects
    baseChain = new Mcmc(m, mv, mn);
    for (size_t i = 0, j = 0; i < numChains; i++, j++)
    {
        // all chains know heat-order and chain-processor schedules
        heatRanks.push_back(i);
        if (j >= num_processes)
        {
            j = j % num_processes;
        }
        chainsPerProcess[j].push_back(i);
        processPerChain[i] = j;
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid)
        {
            
            // get chain heat
            double b = computeBeta(delta,i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *baseChain );
            oneChain->setScheduleType( scheduleType );
            oneChain->setChainActive( i == 0 );
            oneChain->setChainPosteriorHeat( b );
            oneChain->setChainIndex( i );
            oneChain->setActivePID( i );
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
//    for (size_t i = 0; i < m.chainsPerProcess[pid].size(); i++)
    for (size_t i = 0; i < m.chainsPerProcess[pid].size(); i++)
    {
//        size_t k    = m.chainsPerProcess[pid][i];
        size_t k    = m.chainsPerProcess[pid][i];
        if (m.chains[k] != NULL) {
            chains[k]   = m.chains[k]->clone();
        }
    }
    
    chainValues         = m.chainValues;
    chainHeats          = m.chainHeats;
    processPerChain     = m.processPerChain;
    
    currentGeneration   = m.currentGeneration;
    baseChain           = m.baseChain->clone();
    
}

Mcmcmc::~Mcmcmc(void)
{
    for (size_t i = 0; i < chains.size(); i++)
    {
        if (chains[i] != NULL)
        {
            delete chains[i];
        }
    }
    chains.clear();
    delete baseChain;
}


void Mcmcmc::addFileMonitorExtension(const std::string &s, bool dir)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->addFileMonitorExtension(s, dir);
    }
    
}


void Mcmcmc::addMonitor(const Monitor &m)
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->addMonitor( m );
    }
    
}


void Mcmcmc::disableScreenMonitor( void )
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->disableScreenMonitor();
    }
    
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


/**
  * Start the monitors at the beginning of a run which will simply delegate this call to each chain.
  */
void Mcmcmc::finishMonitors( void)
{
    
    // Monitor
    for (size_t i = 0; i < chains.size(); i++)
    {
        
        chains[ chainsPerProcess[pid][i] ]->finishMonitors();
        
    }
    
}



/**
 * Get the model instance.
 */
const Model& Mcmcmc::getModel( void ) const
{
    
    return chains[0]->getModel();
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
//    stream << chains[ chainsPerProcess[pid][0] ]->getStrategyDescription();
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
        for (size_t i = 0; i < numChains; i++)
        {
            swapChains();
        }
    }
    
}

void Mcmcmc::printOperatorSummary(void) const
{
    for (size_t i = 0; i < numChains; i++)
    {
        if (chains[i] != NULL)
        {
            chains[i]->printOperatorSummary();
        }
        
    }
    
}


void Mcmcmc::removeMonitors( void )
{
    
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        chains[ chainsPerProcess[pid][i] ]->removeMonitors();
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
        if (chains[ chainsPerProcess[pid][i] ] != NULL)
        {
            chains[ chainsPerProcess[pid][i] ]->setLikelihoodHeat( h );
        }
        
    }
    
}


/**
  * Set the model by delegating the model to the chains.
  */
void Mcmcmc::setModel( Model *m )
{
    
    // set the models of the chains
    for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
    {
        if (chains[ chainsPerProcess[pid][i] ] != NULL)
        {
            chains[ chainsPerProcess[pid][i] ]->setModel( m->clone() );
        }
        
    }
    
    delete m;
    
}

void Mcmcmc::setNumberOfProcessesSpecialized(size_t n)
{
    
    // @MJL: Note to self. The ctor assumes num_processes==1, so all chains are assigned to that processor.
    // After cloning all chains across processors, you will then want to thin out the chains as needed.
    // This should behave much like the old Mcmcmc ctor code, except it cannot assume a fresh object state.
    
    // only use a many processes as we have chains
    if (numChains < num_processes)
    {
        num_processes = numChains;
    }
        
    // initialize container sizes
    for (size_t i = 0; i < chains.size(); i++)
    {
        if (chains[i] != NULL)
            delete chains[i];
    }
 
    chains.clear();
    chainsPerProcess.clear();
    chainValues.clear();
    chainHeats.clear();
    processPerChain.clear();
    
    chains.resize(numChains);
    chainsPerProcess.resize(num_processes);
    chainValues.resize(numChains, 0.0);
    chainHeats.resize(numChains, 0.0);
    processPerChain.resize(numChains);
    
    // assign chains to processors, instantiate Mcmc objects
    for (size_t i = 0, j = 0; i < numChains; i++, j++)
    {
        // all chains know heat-order and chain-processor schedules
        heatRanks.push_back(i);
        if (j >= num_processes)
        {
            j = j % num_processes;
        }
        
        chainsPerProcess[j].push_back(i);
        processPerChain[i] = j;
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid)
        {
            // get chain heat
            double b = computeBeta(delta, i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *baseChain );
            oneChain->setScheduleType( scheduleType );
            oneChain->setChainActive( i == 0 );
            oneChain->setChainPosteriorHeat( b );
            oneChain->setChainIndex( i );
            oneChain->setActivePID( i );
            chains[i] = oneChain;
        }
        else
        {
            chains[i] = NULL;
        }
    }
    
}


/**
 * Start the monitors at the beginning of a run which will simply delegate this call to each chain.
 */
void Mcmcmc::startMonitors(size_t numCycles)
{
    
    // Monitor
    for (size_t i = 0; i < chains.size(); i++)
    {
        
        chains[ chainsPerProcess[pid][i] ]->startMonitors( numCycles );
        
    }
    
}


void Mcmcmc::synchronizeValues(void)
{
    
    // synchronize chain values
    double results[numChains];
    for (size_t j = 0; j < numChains; j++)
    {
        results[j] = 0.0;
    }
    for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
    {
        size_t k = chainsPerProcess[pid][j];
        results[k] = chains[k]->getModelLnProbability();
    }
    
#ifdef RB_MPI
    if (active_PID != pid)
    {
        MPI::COMM_WORLD.Send(&results, int(numChains), MPI::DOUBLE, (int)active_PID, 0);
    }
    MPI::COMM_WORLD.Barrier();
#endif
    
//    if ( processActive == true )
    if ( active_PID == pid )
    {
#ifdef RB_MPI
        for (size_t i = 0; i < num_processes; i++)
//            for (size_t i = 0; i < chainsPerProcess[pid].size(); i++)
        {
            // ignore self
            if (pid == i)
            {
                continue;
            }
            
            double tmp_results[numChains];
            for (size_t j = 0; j < numChains; j++)
            {
                tmp_results[j] = 0.0;
            }
            MPI::COMM_WORLD.Recv(&tmp_results, int(numChains), MPI::DOUBLE, (int)i, 0);
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
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Barrier();
#endif
    
}

void Mcmcmc::synchronizeHeats(void)
{
    
    // synchronize heat values
    double heats[numChains];
    for (size_t j = 0; j < numChains; j++)
    {
        heats[j] = 0.0;
    }
    for (size_t j = 0; j < chainsPerProcess[pid].size(); j++)
    {
        size_t k = chainsPerProcess[pid][j];
        if (chains[k] != NULL)
        {
            heats[k] = chains[k]->getChainPosteriorHeat();
        }
    }
    
#ifdef RB_MPI
    // share the heats accross processes
    if (active_PID != pid)
    {
        MPI::COMM_WORLD.Send(&heats, (int)numChains, MPI::DOUBLE, (int)active_PID, 0);
    }
    MPI::COMM_WORLD.Barrier();
#endif
    
    if ( active_PID == pid )
    {
#ifdef RB_MPI
        for (size_t i = 0; i < num_processes; i++)
        {
            if (pid == i)
                continue;
            
            double tmp_heats[numChains];
            for (size_t j = 0; j < numChains; j++)
            {
                tmp_heats[j] = 0.0;
            }
            
            MPI::COMM_WORLD.Recv(&tmp_heats, (int)numChains, MPI::DOUBLE, (int)i, 0);
            
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
#ifdef RB_MPI
    MPI::COMM_WORLD.Barrier();
#endif
    
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
#ifdef RB_MPI
    MPI::COMM_WORLD.Barrier();
#endif
    
    // send all chain values to pid 0
    synchronizeValues();
    
#ifdef RB_MPI
    // wait until all chains complete
    MPI::COMM_WORLD.Barrier();
#endif
    
    // send all chain heats to pid 0
    synchronizeHeats();
    
#ifdef RB_MPI
    // wait until all chains complete
    MPI::COMM_WORLD.Barrier();
#endif
    // swap chains
    swapNeighborChains();
    swapRandomChains();
    
#ifdef RB_MPI
    // wait until all chains complete
    MPI::COMM_WORLD.Barrier();
#endif

}
//
//
//void Mcmcmc::swapNeighborChains(void)
//{
//    
//    size_t numAccepted = 0;
//    double lnProposalRatio = 0.0;
//    
//    //for (size_t i = 1; i < numChains; i++)
//    for (size_t i = numChains-1; i > 0; i--)
//    {
//        // swap?
//        bool accept = false;
//        // swap adjacent chains
//        size_t j = 0;
//        size_t k = 0;
//        
//        if (processActive == true)
//        {
//            ++numAttemptedSwaps;
//            
//            j = heatRanks[i-1];
//            k = heatRanks[i];
//            
//            // compute exchange ratio
//            double bj = chainHeats[j];
//            double bk = chainHeats[k];
//            double lnPj = chainValues[j];
//            double lnPk = chainValues[k];
//            double lnR = bj * (lnPk - lnPj) + bk * (lnPj - lnPk) + lnProposalRatio;
//            
//            // determine whether we accept or reject the chain swap
//            double u = GLOBAL_RNG->uniform01();
//            if (lnR >= 0)
//            {
//                accept = true;
//            }
//            else if (lnR < -100)
//            {
//                accept = false;
//            }
//            else if (u < exp(lnR))
//            {
//                accept = true;
//            }
//            else
//            {
//                accept = false;
//            }
//            
//            if (accept == true)
//            {
//                numAccepted++;
//            }
//            
//            // on accept, swap beta values and active chains
//            if (accept)
//            {
//                
//                //size_t tmpIdx = j;
//                heatRanks[i-1] = k;
//                heatRanks[i] = j;
//                
//                // swap active chain
//                if (activeChainIndex == j)
//                {
//                    activeChainIndex = k;
//                }
//                else if (activeChainIndex == k)
//                {
//                    activeChainIndex = j;
//                }
//                
//            }
//        }
//        
//        if (accept)
//        {
//            updateChainState(j);
//            updateChainState(k);
//            
//            ++numAcceptedSwaps;
//        }
//    }
//    
//}



void Mcmcmc::swapNeighborChains(void)
{
    
    double lnProposalRatio = 0.0;
    
    // randomly pick the indices of two chains
    int j = 0;
    int k = 0;
    
    // swap?
    bool accept = false;
    if (numChains < 2) return;
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Barrier();
#endif
    if ( pid == active_PID )
    {
        j = int(GLOBAL_RNG->uniform01() * (numChains-1));
        k = j + 1;
//        if (numChains > 1)
//        {
//            do {
//                k = int(GLOBAL_RNG->uniform01() * numChains);
//            }
//            while(j == k);
//        }
        
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
    
    MPI::COMM_WORLD.Barrier();
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Barrier();
    
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Barrier();
#endif
    
    
    // update the chains accross processes
    // this is necessary because only process 0 does the swap
    // all the other processes need to be told that there was a swap
    updateChainState(j);
    updateChainState(k);
    
}



void Mcmcmc::swapRandomChains(void)
{
    
    double lnProposalRatio = 0.0;
    
    // randomly pick the indices of two chains
    int j = 0;
    int k = 0;
    
    // swap?
    bool accept = false;
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Barrier();
#endif
    if ( pid == active_PID )
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
    
    MPI::COMM_WORLD.Barrier();
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Barrier();
    
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Barrier();
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
    MPI::COMM_WORLD.Barrier();
    // update heat
    if (pid == active_PID && pid == processPerChain[j])
    {
        ; // do nothing
    }
    else if (pid == active_PID)
    {
        MPI::COMM_WORLD.Send(&chainHeats[j], 1, MPI::DOUBLE, (int)processPerChain[j], 0);
    }
    //MPI::COMM_WORLD.Barrier();
    else if (pid == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&chainHeats[j], 1, MPI::DOUBLE, (int)active_PID, 0);
    }
    MPI::COMM_WORLD.Barrier();
#endif
    
    if (pid == processPerChain[j])
    {
        chains[j]->setChainPosteriorHeat(chainHeats[j]);
    }
    
#ifdef RB_MPI
    // update active state
    bool tf = activeChainIndex == j;
    
    MPI::COMM_WORLD.Barrier();
    if (pid == active_PID && pid == processPerChain[j])
    {
        ; // do nothing
    }
    else if (pid == active_PID)
    {
        MPI::COMM_WORLD.Send(&tf, 1, MPI::BOOL, (int)processPerChain[j], 0);
    }
    else if (pid == processPerChain[j])
    {
        MPI::COMM_WORLD.Recv(&tf, 1, MPI::BOOL, (int)active_PID, 0);
    }
    MPI::COMM_WORLD.Barrier();
#endif
    
    if (pid == processPerChain[j])
    {
        chains[j]->setChainActive( chainHeats[j] == 1.0 );
    }
    
}

