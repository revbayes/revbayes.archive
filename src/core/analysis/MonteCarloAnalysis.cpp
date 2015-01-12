#include "MonteCarloAnalysis.h"
#include "RlUserInterface.h"

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * \param[in]    m    The monte carlo sampler.
 */
MonteCarloAnalysis::MonteCarloAnalysis(const MonteCarloSampler &m, size_t r) : Cloneable(),
    replicates( r )
{
    
    // add a clone of the original sampler to our vector of runs
    runs.push_back( m );
    
    if ( replicates > 1 )
    {
        // create replicate Monte Carlo samplers
        for (size_t i = 1; i < replicates; ++i)
        {
            runs.push_back( m );
        }
    }
    
}



/**
 * Destructor. Nothing to do here
 */
MonteCarloAnalysis::~MonteCarloAnalysis(void)
{
    
    // all the magic happens in the RbVector class.
    
}


MonteCarloAnalysis* MonteCarloAnalysis::clone( void ) const
{
    
    return new MonteCarloAnalysis( *this );
}


void MonteCarloAnalysis::run(void)
{
    /* Let user know what we are doing */
    if ( runs[0].getCurrentGeneration() == 0 )
    {
        std::stringstream ss;
        ss << "\nRunning MCMC simulation";
        RBOUT( ss.str() );
    }
    else
    {
        std::stringstream ss;
        ss << "Appending to previous MCMC simulation of " << runs[0].getCurrentGeneration() << " iterations";
        RBOUT( ss.str() );
    }
    
    if ( runs[0].getScheduleType() == "single" )
    {
        std::stringstream ss;
        ss << "The simulator uses " << runs[0].getMoves().size() << " different moves, with a single move picked randomly per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    else if ( runs[0].getScheduleType() == "random" )
    {
        std::stringstream ss;
        ss << "The simulator uses " << runs[0].getMoves().size() << " different moves in a random move schedule with " << runs[0].getSchedule().getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    else if ( runs[0].getScheduleType() == "sequential" )
    {
        std::stringstream ss;
        ss << "The simulator uses " << runs[0].getMoves().size() << " different moves in a sequential move schedule with " << runs[0].getSchedule().getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    
    if ( runs[0].getCurrentGeneration() == 0 )
    {
        // Monitor
        for (size_t i=0; i<replicates; ++i)
        {
            runs[i].startMonitors();
            runs[i].monitor(0);
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
        RbVector<Move> &moves = runs[i].getMoves();
        for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
        {
            it->resetCounters();
        }
    }
    
    // Run the chain
    bool finished = false;
    size_t gen = runs[0].getCurrentGeneration();
    do {
        ++gen;
        for (size_t i=0; i<replicates; ++i)
        {
            runs[i].nextCycle(true);
            
            // Monitor
            runs[i].monitor(gen);
            
        }
        
        // do the stopping test
        
    } while ( finished == false );
    
    
    
}



void MonteCarloAnalysis::runPriorSampler(size_t kIterations)
{
    
//    /* Let user know what we are doing */
//    if ( generation == 0 )
//    {
//        std::cout << "Running prior simulation for " << kIterations << " iterations" << std::endl;
//    }
//    else
//    {
//        std::cout << "Appending " << kIterations << " iterations to previous simulation of " << generation << " iterations" << std::endl;
//    }
//    
//    if ( scheduleType == "single" )
//    {
//        std::cout << "The simulator uses " << moves.size() << " different moves, with a" << std::endl;
//        std::cout << "single move picked randomly per iteration" << std::endl;
//    }
//    else if ( scheduleType == "random" )
//    {
//        std::cout << "The simulator uses " << moves.size() << " different moves in a random" << std::endl;
//        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
//    }
//    else if ( scheduleType == "sequential" )
//    {
//        std::cout << "The simulator uses " << moves.size() << " different moves in a sequential" << std::endl;
//        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
//    }
//    
//    // Initialize objects used in run
//    initializeChain( true );
//    initializeMonitors();
//    
//    if ( generation == 0 )
//    {
//        // Monitor
//        startMonitors( kIterations );
//        monitor(0);
//    }
//    
//    /* Reset the monitors */
//    for (size_t i=0; i<monitors.size(); i++)
//    {
//        monitors[i].reset( kIterations );
//    }
//    
//    // reset the counters for the move schedules
//    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
//    {
//        it->resetCounters();
//    }
//    
//    // Run the chain
//    for (int k=1; k<=kIterations; k++)
//    {
//        nextCycle(true);
//        
//        // Monitor
//        monitor(generation);
//        
//    }
    
    
}


