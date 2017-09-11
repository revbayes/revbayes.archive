#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbIterator.h"

#include <iostream>

using namespace RevBayesCore;

RandomMoveSchedule::RandomMoveSchedule(RbVector<Move> *s) : MoveSchedule( s )
{
    
    movesPerIteration = 0.0;
    for (RbIterator<Move> it = moves->begin(); it != moves->end(); ++it)
    {
        movesPerIteration += it->getUpdateWeight();
    }
}


RandomMoveSchedule::~RandomMoveSchedule()
{
    // we own nothing
}


RandomMoveSchedule* RandomMoveSchedule::clone( void ) const
{
    return new RandomMoveSchedule(*this);
}


double RandomMoveSchedule::getNumberMovesPerIteration( void ) const
{
    return movesPerIteration;
}


Move& RandomMoveSchedule::nextMove( unsigned long gen )
{
    
    movesPerIteration = 0.0;
    for (RbIterator<Move> it = moves->begin(); it != moves->end(); ++it)
    {
        if ( it->isActive( gen ) )
        {
            movesPerIteration += it->getUpdateWeight();
        }
    }
    
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double u = movesPerIteration * rng->uniform01();
    
    size_t index = 0;
    // only if the move is inactive or the weight of the move is smaller than u
    while ( !(*moves)[index].isActive(gen) || (*moves)[index].getUpdateWeight() <= u )
    {
        // check if this move is active
        // if not, then we just subtract the weight of this move
        if ( (*moves)[index].isActive( gen ) )
        {
            u -= (*moves)[index].getUpdateWeight();
        }
        ++index;
    }

    if (index >= moves->size())
    {
        index = moves->size() - 1;
    }
        
    return (*moves)[index];
}
