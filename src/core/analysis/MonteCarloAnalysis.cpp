#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include "AbstractFileMonitor.h"
#include "DagNode.h"
#include "MonteCarloAnalysis.h"
#include "RlUserInterface.h"

#include <algorithm>
#include <cmath>


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
    
    runs[0] = m;
    resetReplicates();
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


/** Run burnin and auto-tune */
void MonteCarloAnalysis::burnin(size_t generations, size_t tuningInterval, bool underPrior, bool verbose)
{
        
    // Initialize objects needed by chain
    for (size_t i=0; i<replicates; ++i)
    {
     
        if ( runs[i] != NULL )
        {
            runs[i]->initializeSampler(underPrior);
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
    size_t num_stars = 0;
    for (size_t k=1; k<=generations; ++k)
    {
                
        if ( verbose == true && process_active == true)
        {
            size_t progress = 68 * (double) k / (double) generations;
            if ( progress > num_stars )
            {
                
                for ( ; num_stars < progress; ++num_stars )
                {
                    std::cout << "*";
                }
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


void MonteCarloAnalysis::disableScreenMonitors(bool all)
{
    
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {

            return runs[i]->disableScreenMonitor(all, i);
        }
        
    }
    
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


void MonteCarloAnalysis::initializeFromTrace( RbVector<ModelTrace> traces )
{
    size_t n_samples = traces[0].getSamples();
    size_t last_generation = 0;
    size_t n_traces = traces.size();
    
    std::vector<DagNode*> nodes = getModel().getDagNodes();
    
    for ( size_t i = 0; i < n_traces; ++i )
    {
        std::string parameter_name = traces[i].getParameterName();
        
        if (parameter_name == "Iteration")
        {
            last_generation = std::atoi( traces[i].objectAt( n_samples - 1 ).c_str() );
        }
        
        // iterate over all DAG nodes (variables)
        for ( size_t j = 0; j < nodes.size(); ++j )
        {
            if ( nodes[j]->getName() == parameter_name )
            {
                // set the value for the variable with the last sample in the trace
                nodes[j]->setValueFromString( traces[i].objectAt( n_samples - 1 ) );
                break;
            }
        }
    }
    
    for (size_t i = 0; i < replicates; ++i)
    {
        // set iteration num for all runs
        runs[i]->setCurrentGeneration( last_generation );
        
        RbVector<Monitor>& monitors = runs[i]->getMonitors();
        for (size_t j = 0; j < monitors.size(); ++j)
        {
            if ( monitors[j].isFileMonitor() )
            {
                // set file monitors to append
                AbstractFileMonitor* m = dynamic_cast< AbstractFileMonitor *>( &monitors[j] );
                m->setAppend(true);
            }
        }
    }
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


/**
 * Reset the replicates.
 */
void MonteCarloAnalysis::resetReplicates( void )
{

    // free the runs
    MonteCarloSampler *m = NULL;
    for (size_t i = 0; i < replicates; ++i)
    {
        MonteCarloSampler *sampler = runs[i];

        if ( m == NULL )
        {
            m = sampler;
        }
        
        if ( m != sampler )
        {
            delete sampler;
        }
        
        runs[i] = NULL;

    }
    
    
    if ( m == NULL )
    {
        throw RbException("Bug: No template sampler found!");
    }
    
    // create replicate Monte Carlo samplers
    bool no_sampler_set = true;
    for (size_t i = 0; i < replicates; ++i)
    {
        size_t replicate_pid_start = size_t(floor( (double(i)   / replicates ) * num_processes ) ) + active_PID;
        size_t replicate_pid_end   = std::max( int(replicate_pid_start), int(floor( (double(i+1) / replicates ) * num_processes ) ) - 1 + int(active_PID) );
        int number_processes_per_replicate = int(replicate_pid_end) - int(replicate_pid_start) + 1;
        
        if ( pid >= replicate_pid_start && pid <= replicate_pid_end )
        {
            no_sampler_set = false;
            
            if ( i == 0 )
            {
                runs[i] = m;
            }
            else
            {
                runs[i] = m->clone();
            }
            
            runs[i]->setActivePID( replicate_pid_start, number_processes_per_replicate );
            //            runs[i]->setMasterSampler( i == 0 );
        }
        
    }
    if ( no_sampler_set == true )
    {
        runs[0] = m;
    }
    
    // disable the screen monitors for the replicates
    disableScreenMonitors( false );
    
    
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
    
    
    // redraw initial states for replicates
    for (size_t i = 0; i < replicates; ++i)
    {
        RandomNumberGenerator *rng = GLOBAL_RNG;
        for (size_t j=0; j<10; ++j) rng->uniform01();
        

        if ( i > 0 && runs[i] != NULL )
        {
            runs[i]->redrawStartingValues();
        }
        
    }
    
}



#ifdef RB_MPI
void MonteCarloAnalysis::run( size_t kIterations, RbVector<StoppingRule> rules, const MPI_Comm &analysis_comm, size_t tuning_interval, bool verbose )
#else
void MonteCarloAnalysis::run( size_t kIterations, RbVector<StoppingRule> rules, size_t tuning_interval, bool verbose )
#endif
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
    if ( process_active == true && runs[0] != NULL && verbose == true )
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

    // Start monitor(s)
    for (size_t i=0; i<replicates; ++i)
    {
        
        // Sebastian (2016/04/16): We should always reset the monitors so that the ETA starts fresh
//        if ( runs[i] != NULL && runs[i]->getCurrentGeneration() == 0 )
        if ( runs[i] != NULL )
        {
            
            if ( i > 0 )
            {
                runs[i]->disableScreenMonitor(true, i);
            }

            runs[i]->startMonitors( kIterations, runs[i]->getCurrentGeneration() > 0 );
        
        }
        
    }

    // Sebastian: This is very important here!
    // We need to wait first for all processes and chains to have opened the filestreams
    // before we start printing (e.g., the headers) anything.
#ifdef RB_MPI
    // wait until all chains opened the monitor
    MPI_Barrier( analysis_comm );
#endif

    // Write headers and print first line
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL && runs[i]->getCurrentGeneration() == 0 )
        {
            
            runs[i]->writeMonitorHeaders();
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
                
                // check for autotuning
                if ( tuning_interval != 0 && (gen % tuning_interval) == 0 )
                {
                    
                    runs[i]->tune();
                    
                }
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
    
    
#ifdef RB_MPI
    // wait until all replicates complete
    MPI_Barrier( analysis_comm );
#endif
    
    // Monitor
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL )
        {
            runs[i]->finishMonitors( replicates );
        }
        
    }
    
    
#ifdef RB_MPI
    // wait until all replicates complete
    MPI_Barrier( analysis_comm );
#endif
    
}



void MonteCarloAnalysis::runPriorSampler( size_t kIterations, RbVector<StoppingRule> rules )
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
        
        if ( runs[i] != NULL )
        {
            runs[i]->initializeSampler(true);
        }
        
    }
    
    
    // Start monitor(s)
    for (size_t i=0; i<replicates; ++i)
    {
        
        // Sebastian (2016/04/16): We should always reset the monitors so that the ETA starts fresh
        // if ( runs[i] != NULL && runs[i]->getCurrentGeneration() == 0 )
        if ( runs[i] != NULL )
        {
            
            if ( i > 0 )
            {
                runs[i]->disableScreenMonitor(true, i);
            }
            
            runs[i]->startMonitors( kIterations, runs[i]->getCurrentGeneration() > 0 );
            
        }
        
    }
    
    // Sebastian: This is very important here!
    // We need to wait first for all processes and chains to have opened the filestreams
    // before we start printing (e.g., the headers) anything.
#ifdef RB_MPI
    // wait until all chains opened the monitor
<<<<<<< HEAD
    MPI::COMM_WORLD.Barrier();
=======
//    MPI::COMM_WORLD.Barrier();
    MPI_Barrier(MPI_COMM_WORLD);
>>>>>>> development
#endif
    
    // Write headers and print first line
    for (size_t i=0; i<replicates; ++i)
    {
        
        if ( runs[i] != NULL && runs[i]->getCurrentGeneration() == 0 )
        {
            
            runs[i]->writeMonitorHeaders();
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
 * Set the active PID of this specific Monte Carlo analysis.
 */
void MonteCarloAnalysis::setActivePIDSpecialized(size_t a, size_t n)
{

    resetReplicates();
    
}


/**
 * Set the model by delegating the model to the Monte Carlo samplers (replicates).
 */
void MonteCarloAnalysis::setModel(Model *m)
{
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        if ( runs[i] != NULL )
        {
            
            if ( i == 0 )
            {
                runs[0]->setModel( m );
            }
            else
            {
                const Model *old_model = &runs[i]->getModel();
                delete old_model;
                
                runs[i]->setModel( m->clone() );
            }
            
        }
        
    }
    
    resetReplicates();

}
