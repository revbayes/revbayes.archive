#include "AbstractMove.h"


using namespace RevBayesCore;


/** 
 * Constructor
 *
 * Here we simply allocate and initialize the move object.
 *
 * \param[in]    w   The weight how often the proposal will be used (per iteration).
 * \param[in]    t   If auto tuning should be used.
 */
AbstractMove::AbstractMove( double w, bool t ) : 
    weight( w ), 
    autoTuning( t ) 
{
    
}



/**
 * Basic destructor doing nothing.
 */
AbstractMove::~AbstractMove( void )
{
    
}


/**
 * Automatically tune the move.
 * This will delegate the call to the derived class but only if autotuning was enabled.
 */
void AbstractMove::autoTune( void )
{
    
    // only call tuning if it is enabled for this move.
    if ( autoTuning ) 
    {
        tune();
        
        // we need to reset the counters so that the next tuning only uses the new acceptance rate.
        resetCounters();
    }
    
}


/**
 * Is the move active at the given generation?
 * The move itself will determine if it was switched off before or after
 * some given iteration.
 */
bool AbstractMove::isActive(unsigned long gen) const
{
    return true;
}


/**
 * Get the update weight of how often the move should be used.
 *
 * \return    The update weight.
 */
double AbstractMove::getUpdateWeight( void ) const 
{
    return weight;
}



/**
 * Perform the move. 
 * Here we store some info and delegate to performMove.
 */
void AbstractMove::perform( double lHeat, double pHeat )
{
    // increment the tries counter
    numTried++;
    
    // delegate to derived class
    performMove(lHeat, pHeat);
    
}



/**
 * Reset the counters. Here we reset the counters stored in the base class
 * and delegate the call to the potentially overwritten method from the 
 * derived classes.
 *
 */
void AbstractMove::resetCounters( void )
{
    numTried = 0;
    
    // delegate call
    resetMoveCounters();
}

/**
 * Reset the move counters. This method is only a hook for derived classes 
 * and here we provide only a dummy implementation.
 *
 */
void AbstractMove::resetMoveCounters( void )
{
    // dummy implementation
}






