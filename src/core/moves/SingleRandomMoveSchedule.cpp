//
//  SingleRandomMoveSchedule.cpp
//  revbayes_mlandis
//
//  Created by Michael Landis on 2/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "SingleRandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbIterator.h"

using namespace RevBayesCore;

SingleRandomMoveSchedule::SingleRandomMoveSchedule(RbVector<Move> *s) : MoveSchedule( s ) {
    
    movesPerIteration = 0.0;
    for (RbIterator<Move> it = moves->begin(); it != moves->end(); ++it)
    {
        movesPerIteration+= it->getUpdateWeight();
    }
}


SingleRandomMoveSchedule::~SingleRandomMoveSchedule()
{
    // we own nothing
}


SingleRandomMoveSchedule* SingleRandomMoveSchedule::clone( void ) const
{

    return new SingleRandomMoveSchedule(*this);
}

double SingleRandomMoveSchedule::getNumberMovesPerIteration( void ) const
{
    return 1.0;
}


Move& SingleRandomMoveSchedule::nextMove( unsigned long gen ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    movesPerIteration = 0.0;
    for (RbIterator<Move> it = moves->begin(); it != moves->end(); ++it)
    {
        if ( it->isActive( gen ) )
        {
            movesPerIteration += it->getUpdateWeight();
        }
    }

    double u = movesPerIteration * rng->uniform01();
    
    size_t index = 0;
    while ( (*moves)[index].getUpdateWeight() < u || !(*moves)[index].isActive( gen ) )
    {
        u -= (*moves)[index].getUpdateWeight();
        ++index;
    }
    
    return (*moves)[index];
}
