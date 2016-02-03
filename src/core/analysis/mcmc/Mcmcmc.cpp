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
    num_chains(nc),
    schedule_type(sT),
    current_generation(0),
    swap_interval(si),
    active_chain_index( 0 ),
    delta( dt ),
    generation( 0 ),
    numAttemptedSwaps( 0 ),
    numAcceptedSwaps( 0 )
{
    
    // only use a many processes as we have chains
    if (num_chains < num_processes)
    {
        num_processes = num_chains;
    }
    
    // initialize container sizes
    chains = std::vector<Mcmc*>(num_chains, NULL);
    chain_values.resize(num_chains, 0.0);
    chain_heats.resize(num_chains, 0.0);
    pid_per_chain.resize(num_chains, 0);
    
    // assign chains to processors, instantiate Mcmc objects
    base_chain = new Mcmc(m, mv, mn);
    for (size_t i = 0, j = 0; i < num_chains; ++i, ++j)
    {
        // all chains know heat-order and chain-processor schedules
        heat_ranks.push_back(i);
        if (j >= num_processes)
        {
            j = j % num_processes;
        }
//        chainsPerProcess[j].push_back(i);
        pid_per_chain[i] = j;
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid)
        {
            
            // get chain heat
            double b = computeBeta(delta,i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *base_chain );
            oneChain->setScheduleType( schedule_type );
            oneChain->setChainActive( i == 0 );
            oneChain->setChainPosteriorHeat( b );
            oneChain->setChainIndex( i );
            oneChain->setActivePID( pid );
            oneChain->setNumberOfProcesses( 1 );
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
    num_chains          = m.num_chains;
    heat_ranks          = m.heat_ranks;
    swap_interval       = m.swap_interval;
    active_chain_index  = m.active_chain_index;
    schedule_type       = m.schedule_type;
    pid_per_chain       = m.pid_per_chain;
    
    numAttemptedSwaps   = m.numAttemptedSwaps;
    numAcceptedSwaps    = m.numAcceptedSwaps;
    generation          = m.generation;
    
    
    chains.clear();
    chains.resize(num_chains, NULL);
    for (size_t i = 0; i < num_chains; ++i)
    {
        if ( m.chains[i] != NULL)
        {
            chains[i]   = m.chains[i]->clone();
        }
        
    }
    
    chain_values         = m.chain_values;
    chain_heats          = m.chain_heats;
    
    current_generation   = m.current_generation;
    base_chain           = m.base_chain->clone();
    
}

Mcmcmc::~Mcmcmc(void)
{
    for (size_t i = 0; i < chains.size(); ++i)
    {
        if (chains[i] != NULL)
        {
            delete chains[i];
        }
    }
    chains.clear();
    delete base_chain;
}


void Mcmcmc::addFileMonitorExtension(const std::string &s, bool dir)
{
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        if ( chains[i] != NULL )
        {
            chains[i]->addFileMonitorExtension(s, dir);
        }
    }
    
}


void Mcmcmc::addMonitor(const Monitor &m)
{
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        if ( chains[i] != NULL )
        {
            chains[i]->addMonitor( m );
        }
    }
    
}


void Mcmcmc::disableScreenMonitor( void )
{
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->disableScreenMonitor();
        }
        
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
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->finishMonitors();
        }
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
    
    for (size_t i=0; i<num_chains; ++i)
    {
        if ( chain_heats[i] == 1.0 )
        {
            return chain_values[i];
        }
    }
    
    return RbConstants::Double::neginf;
}

std::string Mcmcmc::getStrategyDescription( void ) const
{
    std::string description = "";
    std::stringstream stream;
    stream << "The MCMCMC simulator runs 1 cold chain and " << (num_chains-1) << " heated chains.\n";
//    stream << chains[ chainsPerProcess[pid][0] ]->getStrategyDescription();
    size_t chain_index = 0;
    while ( chain_index < num_chains && chains[chain_index] == NULL ) ++chain_index;
    
    stream << chains[chain_index]->getStrategyDescription();
    description = stream.str();
    
    return description;
}


void Mcmcmc::initializeSampler( bool priorOnly )
{
    
    // initialize each chain
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->initializeSampler( priorOnly );
        }
    }
    
}


void Mcmcmc::monitor(unsigned long g)
{
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        std::cerr << pid << ":\t\t" << "Monitor("<< i <<") -- ";
        if ( chains[i] != NULL && chains[i]->isChainActive() )
        {
            chains[i]->monitor(g);
            std::cerr << pid << " -- YES -- " << active_PID;
        }
        std::cerr << std::endl;
        
    }
    
}

void Mcmcmc::nextCycle(bool advanceCycle)
{

    // run each chain for this process
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            // advance chain j by a single cycle
            chains[i]->nextCycle( advanceCycle );
        }
        
    } // loop over chains for this process
    
    if ( advanceCycle == true )
    {
        // advance gen counter
        ++current_generation;
    }
    
    if ( current_generation % swap_interval == 0 )
    {
#ifdef RB_MPI
        // wait until all chains complete
        MPI::COMM_WORLD.Barrier();
#endif
        
        // perform chain swap
        for (size_t i = 0; i < num_chains; ++i)
        {
            swapChains();
        }
    }
    
}

void Mcmcmc::printOperatorSummary(void) const
{
    for (size_t i = 0; i < num_chains; ++i)
    {
        if (chains[i] != NULL)
        {
            chains[i]->printOperatorSummary();
        }
        
    }
    
}


void Mcmcmc::removeMonitors( void )
{
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->removeMonitors();
        }
        
    }
    
}


void Mcmcmc::reset( void )
{
    
    // reset counters
    numAcceptedSwaps = 0;
    numAttemptedSwaps = 0;
    
//    /* Reset the monitors */
//    for (size_t i = 0; i < chainsPerProcess[pid].size(); ++i)
//    {
//        RbVector<Monitor>& monitors = chains[ chainsPerProcess[pid][i] ]->getMonitors();
//        for (size_t i=0; i<monitors.size(); ++i)
//        {
//            monitors[i].reset();
//        }
//    }
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->reset();
        }
        
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
    
    for (size_t i = 0; i < num_chains; ++i)
    {
        if (chains[i] != NULL)
        {
            chains[i]->setLikelihoodHeat( h );
        }
        
    }
    
}


/**
  * Set the model by delegating the model to the chains.
  */
void Mcmcmc::setModel( Model *m )
{
    
    // set the models of the chains
    for (size_t i = 0; i < num_chains; ++i)
    {
        if (chains[i] != NULL)
        {
            chains[i]->setModel( m->clone() );
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
    if (num_chains < num_processes)
    {
        num_processes = num_chains;
    }
        
    // initialize container sizes
    for (size_t i = 0; i < chains.size(); ++i)
    {
        if (chains[i] != NULL)
            delete chains[i];
    }
 
    chains.clear();
    chain_values.clear();
    chain_heats.clear();
    
    chains.resize(num_chains);
    chain_values.resize(num_chains, 0.0);
    chain_heats.resize(num_chains, 0.0);
    pid_per_chain.resize(num_chains, 0);
    
    // assign chains to processors, instantiate Mcmc objects
    for (size_t i = 0, j = 0; i < num_chains; ++i, ++j)
    {
        // all chains know heat-order and chain-processor schedules
        heat_ranks.push_back(i);
        if (j >= num_processes)
        {
            j = j % num_processes;
        }
        pid_per_chain[i] = j;
        
        
        // add chain to pid's chain vector (smaller memory footprint)
        if (j == pid)
        {
            // get chain heat
            double b = computeBeta(delta, i);
            
            // create chains
            Mcmc* oneChain = new Mcmc( *base_chain );
            oneChain->setScheduleType( schedule_type );
            oneChain->setChainActive( i == 0 );
            oneChain->setChainPosteriorHeat( b );
            oneChain->setChainIndex( i );
            oneChain->setActivePID( pid );
            oneChain->setNumberOfProcesses( 1 );
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
    for (size_t i = 0; i < num_chains; ++i)
    {
        
        if ( chains[i] != NULL )
        {
            chains[i]->startMonitors( numCycles );
        }
    }
    
}


void Mcmcmc::synchronizeValues(void)
{
    
    // synchronize chain values
    double results[num_chains];
    for (size_t j = 0; j < num_chains; ++j)
    {
        results[j] = 0.0;
    }
    for (size_t j = 0; j < num_chains; ++j)
    {
        
        if ( chains[j] != NULL )
        {
            results[j] = chains[j]->getModelLnProbability();
        }
        
    }
    
#ifdef RB_MPI
    if (active_PID != pid)
    {
        MPI::COMM_WORLD.Send(&results, int(num_chains), MPI::DOUBLE, (int)active_PID, 0);
    }
#endif
    
//    if ( processActive == true )
    if ( active_PID == pid )
    {
#ifdef RB_MPI
        for (size_t i = 0; i < num_processes; ++i)
//            for (size_t i = 0; i < chainsPerProcess[pid].size(); ++i)
        {
            // ignore self
            if (pid == i)
            {
                continue;
            }
            
            double tmp_results[num_chains];
            for (size_t j = 0; j < num_chains; ++j)
            {
                tmp_results[j] = 0.0;
            }
            MPI::COMM_WORLD.Recv(&tmp_results, int(num_chains), MPI::DOUBLE, (int)i, 0);
            for (size_t j = 0; j < num_chains; ++j)
            {
                
                if ( pid_per_chain[j] != i )
                {
                    results[j] = tmp_results[j];
                }
                
            }
        }
#endif
        for (size_t i = 0; i < num_chains; ++i)
        {
            chain_values[i] = results[i];
        }
    }
    
#ifdef RB_MPI
    for (size_t i=0; i<num_chains; ++i)
    {
        MPI::COMM_WORLD.Bcast(&chain_values[i], 1, MPI::DOUBLE, (int)active_PID);
    }
#endif
    
}

void Mcmcmc::synchronizeHeats(void)
{
    
    // synchronize heat values
    double heats[num_chains];
    for (size_t j = 0; j < num_chains; ++j)
    {
        heats[j] = 0.0;
    }
    for (size_t j = 0; j < num_chains; ++j)
    {
        if (chains[j] != NULL)
        {
            heats[j] = chains[j]->getChainPosteriorHeat();
        }
    }
    
#ifdef RB_MPI
    // share the heats accross processes
    if (active_PID != pid)
    {
        MPI::COMM_WORLD.Send(&heats, (int)num_chains, MPI::DOUBLE, (int)active_PID, 0);
    }
    MPI::COMM_WORLD.Barrier();
#endif
    
    if ( active_PID == pid )
    {
#ifdef RB_MPI
        for (size_t i = 0; i < num_processes; ++i)
        {
            if (pid == i)
                continue;
            
            double tmp_heats[num_chains];
            for (size_t j = 0; j < num_chains; ++j)
            {
                tmp_heats[j] = 0.0;
            }
            
            MPI::COMM_WORLD.Recv(&tmp_heats, (int)num_chains, MPI::DOUBLE, (int)i, 0);
            
            for (size_t j = 0; j < num_chains; ++j)
            {
                
                if ( pid_per_chain[j] == i )
                {
                    heats[j] = tmp_heats[j];
                }
                
            }
        }
#endif
        for (size_t i = 0; i < num_chains; ++i)
        {
            chain_heats[i] = heats[i];
        }
    }
#ifdef RB_MPI
    for (size_t i=0; i<num_chains; ++i)
    {
        MPI::COMM_WORLD.Bcast(&chain_heats[i], 1, MPI::DOUBLE, (int)active_PID);
    }
//    MPI::COMM_WORLD.Bcast(&chain_heats, (int)num_chains, MPI::DOUBLE, (int)active_PID);
#endif
    
}


// MJL: allow swapChains to take a swap function -- e.g. pairwise swap for 1..n-1
void Mcmcmc::swapChains(void)
{
    
    // exit if there is only one chain
    if (num_chains < 2)
    {
        return;
    }
    
    // send all chain values to pid 0
    synchronizeValues();
    
    // send all chain heats to pid 0
    synchronizeHeats();
   
    // swap chains
    swapNeighborChains();
//    swapRandomChains();
    


}
//
//
//void Mcmcmc::swapNeighborChains(void)
//{
//    
//    size_t numAccepted = 0;
//    double lnProposalRatio = 0.0;
//    
//    //for (size_t i = 1; i < num_chains; ++i)
//    for (size_t i = num_chains-1; i > 0; i--)
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
//            j = heat_ranks[i-1];
//            k = heat_ranks[i];
//            
//            // compute exchange ratio
//            double bj = chain_heats[j];
//            double bk = chain_heats[k];
//            double lnPj = chain_values[j];
//            double lnPk = chain_values[k];
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
//                heat_ranks[i-1] = k;
//                heat_ranks[i] = j;
//                
//                // swap active chain
//                if (active_chain_index == j)
//                {
//                    active_chain_index = k;
//                }
//                else if (active_chain_index == k)
//                {
//                    active_chain_index = j;
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
    if (num_chains < 2) return;
    
    if ( pid == active_PID )
    {
        j = int(GLOBAL_RNG->uniform01() * (num_chains-1));
        k = j + 1;
//        if (num_chains > 1)
//        {
//            do {
//                k = int(GLOBAL_RNG->uniform01() * num_chains);
//            }
//            while(j == k);
//        }
        
        ++numAttemptedSwaps;
        
        // compute exchange ratio
        double bj = chain_heats[j];
        double bk = chain_heats[k];
        double lnPj = chain_values[j];
        double lnPk = chain_values[k];
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
            if (active_chain_index == j)
            {
                active_chain_index = k;
            }
            else if (active_chain_index == k)
            {
                active_chain_index = j;
            }
            
            chain_heats[j] = bk;
            chain_heats[k] = bj;
            size_t tmp = heat_ranks[j];
            heat_ranks[j] = heat_ranks[k];
            heat_ranks[k] = tmp;
            
            ++numAcceptedSwaps;
        }
        
        
    }
    
#ifdef RB_MPI
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, (int)active_PID);
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
    
    if ( pid == active_PID )
    {
        j = int(GLOBAL_RNG->uniform01() * num_chains);
        if (num_chains > 1)
        {
            do {
                k = int(GLOBAL_RNG->uniform01() * num_chains);
            }
            while(j == k);
        }
        
        ++numAttemptedSwaps;
            
        // compute exchange ratio
        double bj = chain_heats[j];
        double bk = chain_heats[k];
        double lnPj = chain_values[j];
        double lnPk = chain_values[k];
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
            if (active_chain_index == j)
            {
                active_chain_index = k;
            }
            else if (active_chain_index == k)
            {
                active_chain_index = j;
            }
            
            chain_heats[j] = bk;
            chain_heats[k] = bj;
            size_t tmp = heat_ranks[j];
            heat_ranks[j] = heat_ranks[k];
            heat_ranks[k] = tmp;
            
            ++numAcceptedSwaps;
        }
        
        
    }

#ifdef RB_MPI
    MPI::COMM_WORLD.Bcast(&j, 1, MPI_INT, (int)active_PID);
    MPI::COMM_WORLD.Bcast(&k, 1, MPI_INT, (int)active_PID);
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
    if ( pid == active_PID && chains[j] != NULL )
    {
        ; // do nothing
    }
    else if ( pid == active_PID )
    {
        MPI::COMM_WORLD.Send(&chain_heats[j], 1, MPI::DOUBLE, (int)pid_per_chain[j], 0);
    }
    //MPI::COMM_WORLD.Barrier();
    else if ( pid == pid_per_chain[j] )
    {
        MPI::COMM_WORLD.Recv(&chain_heats[j], 1, MPI::DOUBLE, (int)active_PID, 0);
    }
#endif
    
    if ( chains[j] != NULL )
    {
        chains[j]->setChainPosteriorHeat( chain_heats[j] );
    }
    
    for (size_t i=0; i<num_chains; ++i)
    {
        if ( chains[i] != NULL )
        {
//            std::cerr << pid << " -- " << j << " -- " << chain_heats[j] << std::endl;
            chains[i]->setChainActive( chain_heats[i] == 1.0 );
        }
    }
    
}

