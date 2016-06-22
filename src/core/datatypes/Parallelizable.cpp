#include "Parallelizable.h"


#ifdef RB_MPI
#include <mpi.h>
#endif

using namespace RevBayesCore;


/**
 * Default constructor.
 * We initialize the variables used for code parallelization,
 * such as the number of processes, the process ID, etc.
 */
Parallelizable::Parallelizable() :
    active_PID( 0 ),
    num_processes( 1 ),
    pid( 0 ),
    process_active( true )
{
    
    
#ifdef RB_MPI
    num_processes = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
#endif
    
    process_active = ( active_PID == pid );
    
}


/**
 * Standard copy constructor.
 */
Parallelizable::Parallelizable( const Parallelizable &p ) :
    active_PID( p.active_PID ),
    num_processes( p.num_processes ),
    pid( p.pid ),
    process_active( p.process_active )
{
    
}


/**
 * Standard assignment operator.
 */
Parallelizable& Parallelizable::operator=(const Parallelizable &p)
{
    
    // check for self-assignment
    if ( this != &p )
    {
        active_PID      = p.active_PID;
        num_processes   = p.num_processes;
        pid             = p.pid;
        process_active  = p.process_active;
    }
    
    // return reference to myself
    return *this;
}

/**
 * Public method for getting the active PID for this object.
 */
size_t Parallelizable::getActivePID( void ) const
{
    return active_PID;
}

/**
 * Public method for getting the number of available processes for this object.
 */
size_t Parallelizable::getNumberOfProcesses( void ) const
{
    return num_processes;
}


/**
 * Public method for setting the active PID for this object.
 */
void Parallelizable::setActivePID(size_t a, size_t n)
{
    
    active_PID      = a;
    num_processes   = n;
    process_active  = ( active_PID == pid );
    
    // delegate call for derived classes
    setActivePIDSpecialized(a,n);
}


/**
 * Dummy implementation which derived classes can overwrite.
 */
void Parallelizable::setActivePIDSpecialized(size_t a, size_t n )
{
    
    // nothing done here.
}