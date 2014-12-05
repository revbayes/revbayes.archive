#include "DagNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "RlUserInterface.h"
#include "SequenctialMoveSchedule.h"
#include "SingleRandomMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <typeinfo>

using namespace RevBayesCore;


/**
 * Constructor. We create an independent copy of the model and thus of all DAG nodes.
 * Someone might have wanted to run another MCMC with different settings on the same model.
 * Thus we also create our own copies of the monitors and moves.
 *
 * \param[in]    m    The model containing all DAG nodes.
 * \param[in]    mvs  The vector of moves.
 * \param[in]    mons The vector of monitors.
 */
Mcmc::Mcmc(const Model& m, const RbVector<Move> &mvs, const RbVector<Monitor> &mons) : MonteCarloSampler(m,mvs,mons)
{
    
}



/**
 * Destructor. Nothing to do here
 */
Mcmc::~Mcmc(void) {
    
    // all the magic happens in the base class.
    
}


Mcmc* Mcmc::clone( void ) const
{
    
    return new Mcmc( *this );
}


void Mcmc::run(size_t kIterations)
{
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
        ss << "The simulator uses " << moves.size() << " different moves, with a single move picked randomly per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    else if ( scheduleType == "random" )
    {
        std::stringstream ss;
        ss << "The simulator uses " << moves.size() << " different moves in a random move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    else if ( scheduleType == "sequential" )
    {
        std::stringstream ss;
        ss << "The simulator uses " << moves.size() << " different moves in a sequential move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
        RBOUT( ss.str() );
    }
    
    if ( generation == 0 )
    {
        // Monitor
        startMonitors( kIterations );
        monitor(0);
    }
    
    // reset the counters for the move schedules
    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {
        it->resetCounters();
    }
    
    // Run the chain
    for (int k=1; k<=kIterations; k++) 
    {
        nextCycle(true);
        
        // Monitor
        monitor(generation);
                        
    }
    
    
}



void Mcmc::runPriorSampler(size_t kIterations)
{
    
    /* Let user know what we are doing */
    if ( generation == 0 )
    {
        std::cout << "Running prior simulation for " << kIterations << " iterations" << std::endl;
    }
    else
    {
        std::cout << "Appending " << kIterations << " iterations to previous simulation of " << generation << " iterations" << std::endl;
    }
    
    if ( scheduleType == "single" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves, with a" << std::endl;
        std::cout << "single move picked randomly per iteration" << std::endl;
    }
    else if ( scheduleType == "random" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a random" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    else if ( scheduleType == "sequential" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a sequential" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    
    // Initialize objects used in run
    initializeChain( true );
    initializeMonitors();
    
    if ( generation == 0 )
    {
        // Monitor
        startMonitors( kIterations );
        monitor(0);
    }
    
    // reset the counters for the move schedules
    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {
        it->resetCounters();
    }
    
    // Run the chain
    for (int k=1; k<=kIterations; k++)
    {
        nextCycle(true);
        
        // Monitor
        monitor(generation);
        
    }
    
    
}


