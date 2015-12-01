#include "DagNode.h"
#include "FileMonitor.h"
#include "MonteCarloSampler.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbIterator.h"
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

#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;


/**
 * Constructor. We create an independent copy of the model and thus of all DAG nodes.
 * Someone might have wanted to run another MonteCarloSampler with different settings on the same model.
 * Thus we also create our own copies of the monitors and moves.
 *
 * \param[in]    m    The model containing all DAG nodes.
 * \param[in]    mvs  The vector of moves.
 * \param[in]    mons The vector of monitors.
 */
MonteCarloSampler::MonteCarloSampler(void) :
    activePID(0),
    generation(0),
    numProcesses(1),
    pid(0),
    processActive( true )
{
    
#ifdef RB_MPI
    //    numProcesses = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    processActive = (pid == activePID);
    
}


/**
 * Destructor. Nothing to do here.
 */
MonteCarloSampler::~MonteCarloSampler(void)
{
    
}


/**
 * Get the current generation number.
 */
size_t MonteCarloSampler::getCurrentGeneration( void ) const
{
    return generation;
}


void MonteCarloSampler::setActive(bool tf)
{
#ifdef DEBUG_MPI_MCA
    std::cout << pid << " MonteCarloSampler::setActive \n";
#endif
    processActive = tf;
    if ( processActive )
    {
        activePID = 0; //pid;
    }
    
}

void MonteCarloSampler::setNumberOfProcesses(size_t n, size_t offset)
{
    numProcesses = n;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const MonteCarloSampler& x)
{
    o << "MonteCarloSampler";
    
    return o;
}



