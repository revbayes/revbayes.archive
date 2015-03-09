#include "MonteCarloAnalysis.h"
#include "RlUserInterface.h"

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * \param[in]    m    The monte carlo sampler.
 */
MonteCarloAnalysis::MonteCarloAnalysis(MonteCarloSampler *m, size_t r) : Cloneable(),
    replicates( r )
{
    
    // add a clone of the original sampler to our vector of runs
    runs.push_back( m );
    
    if ( replicates > 1 )
    {
        // create replicate Monte Carlo samplers
        for (size_t i = 1; i < replicates; ++i)
        {
            runs.push_back( m->clone() );
        }
        
        for (size_t i = 0; i < replicates; ++i)
        {
            runs[i]->setReplicateIndex( i+1 );
        }
    }
    
}


MonteCarloAnalysis::MonteCarloAnalysis(const MonteCarloAnalysis &m) : Cloneable(),
replicates( m.replicates )
{
    
    // create replicate Monte Carlo samplers
    for (size_t i=0; i < replicates; ++i)
    {
        runs.push_back( m.runs[i]->clone() );
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
MonteCarloAnalysis& MonteCarloAnalysis::operator=(const MonteCarloAnalysis &m)
{
    
    if ( this != &m )
    {
        
        // free the runs
        for (size_t i = 0; i < replicates; ++i)
        {
            MonteCarloSampler *sampler = runs[i];
            delete sampler;
        }
        
        replicates = m.replicates;
        // create replicate Monte Carlo samplers
        for (size_t i=0; i < replicates; ++i)
        {
            runs.push_back( m.runs[i]->clone() );
        }

    }
    
    return *this;
}


/** Run burnin and autotune */
void MonteCarloAnalysis::burnin(size_t generations, size_t tuningInterval)
{
    
    // Initialize objects needed by chain
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->initializeSampler();
    }
    
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->reset();
    }
    
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
    
    
    // Run the chain
    size_t numStars = 0;
    for (size_t k=1; k<=generations; k++)
    {
        size_t progress = 68 * (double) k / (double) generations;
        if ( progress > numStars )
        {
            for ( ;  numStars < progress; ++numStars )
                std::cout << "*";
            std::cout.flush();
        }
        
        for (size_t i=0; i<replicates; ++i)
        {

            runs[i]->nextCycle(false);
        
            // check for autotuning
            if ( k % tuningInterval == 0 && k != generations )
            {
            
                runs[i]->tune();
            }
        }
        
    }
    
    std::cout << std::endl;
    
}



MonteCarloAnalysis* MonteCarloAnalysis::clone( void ) const
{
    
    return new MonteCarloAnalysis( *this );
}


size_t MonteCarloAnalysis::getCurrentGeneration( void ) const
{
    
    return runs[0]->getCurrentGeneration();
}


/**
 * Print out a summary of the current performance.
 */
void MonteCarloAnalysis::printPerformanceSummary( void ) const
{
    runs[0]->printOperatorSummary();
}


void MonteCarloAnalysis::run( RbVector<StoppingRule> rules )
{
    /* Let user know what we are doing */
    std::stringstream ss;
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
    
    if ( runs[0]->getCurrentGeneration() == 0 )
    {
        // Monitor
        for (size_t i=0; i<replicates; ++i)
        {
            runs[i]->startMonitors();
            runs[i]->monitor(0);
        }
    }
    
    /* Reset the monitors */
//    for (size_t i=0; i<replicates; ++i)
//    {
//        for (size_t j=0; i<runs[i].getMonitors().size(); i++)
//        {
//            runs[i].getMonitors()[j].reset( kIterations);
//        }
//    }
    
    // reset the counters for the move schedules
    for (size_t i=0; i<replicates; ++i)
    {
        runs[i]->reset();
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



void MonteCarloAnalysis::runPriorSampler( RbVector<StoppingRule> rules )
{
    
    /* Let user know what we are doing */
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
            runs[i]->startMonitors();
            runs[i]->monitor(0);
        }
    }
    
    /* Reset the monitors */
    //    for (size_t i=0; i<replicates; ++i)
    //    {
    //        for (size_t j=0; i<runs[i].getMonitors().size(); i++)
    //        {
    //            runs[i].getMonitors()[j].reset( kIterations);
    //        }
    //    }
    
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


