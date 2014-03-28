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
    }
    
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
void AbstractMove::perform( void )
{
    // increment the tries counter
    numTried++;
    
    // delegate to derived class
    performMove();
    
}




