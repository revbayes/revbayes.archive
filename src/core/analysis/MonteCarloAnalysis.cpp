#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include "MonteCarloAnalysis.h"
#include "RlUserInterface.h"

#include <algorithm>
#include <cmath>

#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * \param[in]    m    The monte carlo sampler.
 */
MonteCarloAnalysis::MonteCarloAnalysis(MonteCarloSampler *m, size_t r) : Cloneable(), Parallelizable(),
    replicates( r ),
    runs(r,NULL)
{
    
    // create replicate Monte Carlo samplers
    for (size_t i = 0; i < replicates; ++i)
    {
        size_t replicate_pid_start = size_t(floor( (double(i)   / replicates ) * num_processes ) );
        size_t replicate_pid_end   = std::max( int(replicate_pid_start), int(floor( (double(i+1) / replicates ) * num_processes ) ) - 1);
        int number_processes_per_replicate = int(replicate_pid_end) - int(replicate_pid_start) + 1;

        if ( pid >= replicate_pid_start && pid <= replicate_pid_end)
        {
            if ( i == 0 )
            {
                runs[i] = m;
            }
            else
            {
                runs[i] = m->clone();
            }

            runs[i]->setActivePID( replicate_pid_start );
            runs[i]->setNumberOfProcesses( number_processes_per_replicate );
            
            if ( process_active == false || i != 0 )
            {
                runs[i]->disableScreenMonitor();
            }
            
        }
        
    }
    
    
    // we only need to tell the MonteCarloSamplers which replicate index they are if there is more than one replicate
    if ( replicates > 1 )
    {
        for (size_t i = 0; i < replicates; ++i)
        {
            
            if ( runs[i] != NULL )
            {
            
                std::stringstream ss;
                ss << "_run_" << (i+1);
                runs[i]->addFileMonitorExtension( ss.str(), false);
                
            }
            
        }
        
    }
    
    
}


MonteCarloAnalysis::MonteCarloAnalysis(const MonteCarloAnalysis &a) : Cloneable(), Parallelizable(a),
    replicates( a.replicates ),
    runs(a.replicates,NULL)
{
    
    
    // create replicate Monte Carlo samplers
    for (size_t i=0; i < replicates; ++i)
    {
        if ( a.runs[i] != NULL )
        {
            runs[i] = a.runs[i]->clone();
        }
        
    }
    
}




/**
 * Destructor. Nothing to do here
 */
MonteCarloAnalysis::~MonteCarloAnalysis(void)
{
    
    // free the runs
    for (size_t i = 0; i < replicates; ++i)
    {
        MonteCarloSampler *sampler = runs[i];
        delete sampler;
    }
    
    
}


/**
 * Overloaded assignment operator.
 * We need to keep track of the MonteCarloSamplers
 */
MonteCarloAnalysis& MonteCarloAnalysis::operator=(const MonteCarloAnalysis &a)
{
    Parallelizable::operator=(a);
    
    if ( this != &a )
    {
        
        // free the runs
        for (size_t i = 0; i < replicates; ++i)
        {
            MonteCarloSampler *sampler = runs[i];
            delete sampler;
        }
        runs = std::vector<MonteCarloSampler*>(a.replicates,NULL);
        
        replicates      = a.replicates;
        
        // create replicate Monte Carlo samplers
        for (size_t i=0; i < replicates; ++i)
        {
            if ( a.runs[i] != NULL )
            {
                runs[i] = a.runs[i]->clone();
            }
            
        }

    }
    
    return *this;
}


/**
 * Set the model by delegating the model to the Monte Carlo samplers (replicates).
 */
void MonteCarloAnalysis::addFileMonitorExtension(const std::string &s, bool dir)
{
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->addFileMonitorExtension(s, dir);
    }
    
}


/**
 * Add the monitors.
 */
void MonteCarloAnalysis::addMonitor(const Monitor &m)
{
    
    // remove the monitors for each replicate
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->addMonitor( m );
    }
    
}


/** Run burnin and autotune */
void MonteCarloAnalysis::burnin(size_t generations, size_t tuningInterval, bool verbose)
{
        
    // Initialize objects needed by chain
    for (size_t i=0; i<replicates; ++i)
    {
     
        if ( runs[i] != NULL )
        {
            runs[i]->initializeSampler();
        }
        
    }
    
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            runs[i]->reset();
        }
        
    }
    
    if ( verbose == true && runs[0] != NULL && process_active == true )
    {
        // Let user know what we are doing
        std::stringstream ss;
        ss << "\n";
        ss << "Running burn-in phase of Monte Carlo sampler for " << generations << " iterations.\n";
        ss << "This simulation runs " << replicates << " independent replicate" << (replicates > 1 ? "s" : "") << ".\n";
        ss << runs[0]->getStrategyDescription();
        RBOUT( ss.str() );
        
        // Print progress bar (68 characters wide)
        std::cout << std::endl;
        std::cout << "Progress:" << std::endl;
        std::cout << "0---------------25---------------50---------------75--------------100" << std::endl;
        std::cout.flush();
    }
    
    
    // Run the chain
    size_t numStars = 0;
    for (size_t k=1; k<=generations; k++)
    {
        if ( verbose == true && process_active == true)
        {
            size_t progress = 68 * (double) k / (double) generations;
            if ( progress > numStars )
            {
                for ( ; numStars < progress; ++numStars )
                    std::cout << "*";
                std::cout.flush();
            }
        }
        
        for (size_t i=0; i<replicates; ++i)
        {

            if ( runs[i] != NULL )
            {
                runs[i]->nextCycle(false);
        
                // check for autotuning
                if ( k % tuningInterval == 0 && k != generations )
                {
            
                    runs[i]->tune();
                }
                
            }
            
        }
        
    }
    
    if ( verbose == true && process_active == true )
    {
        std::cout << std::endl;
    }
    
}



MonteCarloAnalysis* MonteCarloAnalysis::clone( void ) const
{
    
    return new MonteCarloAnalysis( *this );
}


size_t MonteCarloAnalysis::getCurrentGeneration( void ) const
{
    
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            return runs[i]->getCurrentGeneration();
        }
        
    }
    
    return 0;
}


const Model& MonteCarloAnalysis::getModel( void ) const
{
    
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            return runs[i]->getModel();
        }
        
    }
    
    return runs[0]->getModel();
}


/**
 * Print out a summary of the current performance.
 */
void MonteCarloAnalysis::printPerformanceSummary( void ) const
{
    
    if ( runs[0] != NULL )
    {
        runs[0]->printOperatorSummary();
    }
    
}


/**
 * Remove the monitors.
 */
void MonteCarloAnalysis::removeMonitors( void )
{
    
    // remove the monitors for each replicate
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            runs[i]->removeMonitors();
        }
        
    }
    
}


void MonteCarloAnalysis::run( size_t kIterations, RbVector<StoppingRule> rules, bool verbose )
{
    
    // get the current generation
    size_t gen = 0;
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            gen = runs[i]->getCurrentGeneration();
        }
        
    }
    
    // Let user know what we are doing
    std::stringstream ss;
    if ( runs[0] != NULL && verbose == true )
    {
        if ( runs[0]->getCurrentGeneration() == 0 )
        {
            ss << "\n";
            ss << "Running MCMC simulation\n";
        }
        else
        {
            ss << "Appending to previous MCMC simulation of " << runs[0]->getCurrentGeneration() << " iterations\n";
        }
        ss << "This simulation runs " << replicates << " independent replicate" << (replicates > 1 ? "s" : "") << ".\n";
        ss << runs[0]->getStrategyDescription();
        RBOUT( ss.str() );
    }
    
    // Monitor
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL && runs[i]->getCurrentGeneration() == 0 )
        {
            runs[i]->startMonitors( kIterations );
            runs[i]->monitor(0);
        }
        
    }
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            runs[i]->reset();
        }
        
    }
    
    // reset the stopping rules
    for (size_t i=0; i<rules.size(); ++i)
    {
     
        rules[i].setNumberOfRuns( replicates );
        rules[i].runStarted();
        
    }

    
    // Run the chain
    bool finished = false;
    bool converged = false;
    do {
        ++gen;
        for (size_t i=0; i<replicates; ++i)
        {
            
            if ( runs[i] != NULL )
            {
                runs[i]->nextCycle(true);
            
                // Monitor
                runs[i]->monitor(gen);
            }
            
        }
        
        converged = true;
        size_t numConvergenceRules = 0;
        // do the stopping test
        for (size_t i=0; i<rules.size() && converged; ++i)
        {
            
            if ( rules[i].isConvergenceRule() )
            {
                converged &= rules[i].checkAtIteration(gen) && rules[i].stop( gen );
                ++numConvergenceRules;
            }
            else
            {
                if ( rules[i].checkAtIteration(gen) && rules[i].stop( gen ) )
                {
                    finished = true;
                    break;
                }
            }
            
        }
        converged &= numConvergenceRules > 0;
        
    } while ( finished == false && converged == false);
    
    // Monitor
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            runs[i]->finishMonitors();
        }
        
    }
    
}



void MonteCarloAnalysis::runPriorSampler( size_t kIterations , RbVector<StoppingRule> rules )
{
    
    // Let user know what we are doing
    std::stringstream ss;
    if ( runs[0]->getCurrentGeneration() == 0 )
    {
        ss << "\n";
        ss << "Running prior MCMC simulation\n";
    }
    else
    {
        ss << "Appending to previous MCMC simulation of " << runs[0]->getCurrentGeneration() << " iterations\n";
    }
    ss << "This simulation runs " << replicates << " independent replicate" << (replicates > 1 ? "s" : "") << ".\n";
    ss << runs[0]->getStrategyDescription();
    RBOUT( ss.str() );
    
    // Initialize objects needed by chain
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->initializeSampler(true);
    }
    
    if ( runs[0]->getCurrentGeneration() == 0 )
    {
        // Monitor
        for (size_t i=0; i<replicates; ++i)
        {
            runs[i]->startMonitors( kIterations );
            runs[i]->monitor(0);
        }
    }
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->reset();
    }
    
    // reset the stopping rules
    for (size_t i=0; i<rules.size(); ++i)
    {
        rules[i].runStarted();
    }
    
    
    // Run the chain
    bool finished = false;
    bool converged = false;
    size_t gen = runs[0]->getCurrentGeneration();
    do {
        ++gen;
        for (size_t i=0; i<replicates; ++i)
        {
            runs[i]->nextCycle(true);
            
            // Monitor
            runs[i]->monitor(gen);
            
        }
        
        converged = true;
        size_t numConvergenceRules = 0;
        // do the stopping test
        for (size_t i=0; i<rules.size(); ++i)
        {
            
            if ( rules[i].isConvergenceRule() )
            {
                converged &= rules[i].checkAtIteration(gen) && rules[i].stop( gen );
                ++numConvergenceRules;
            }
            else
            {
                if ( rules[i].checkAtIteration(gen) && rules[i].stop( gen ) )
                {
                    finished = true;
                    break;
                }
            }
            
        }
        converged &= numConvergenceRules > 0;
        
    } while ( finished == false && converged == false);
    
}


/**
 * Set the model by delegating the model to the Monte Carlo samplers (replicates).
 */
void MonteCarloAnalysis::setModel(Model *m)
{
    
    // reset the counters for the move schedules
    runs[0]->setModel( m );
    for (size_t i=1; i<replicates; ++i)
    {
        runs[i]->setModel( m->clone() );
    }
    
}

