#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include <iostream>

using namespace RevBayesCore;

RandomMoveSchedule::RandomMoveSchedule(const std::vector<Move*> &s) : MoveSchedule( s ) {
    
    movesPerIteration = 0.0;
    for (std::vector<Move*>::const_iterator it = moves.begin(); it != moves.end(); ++it) {
        movesPerIteration += (*it)->getUpdateWeight();
        weights.push_back( (*it)->getUpdateWeight() );
    }
}


RandomMoveSchedule::~RandomMoveSchedule() {
    // we own nothing
}


RandomMoveSchedule* RandomMoveSchedule::clone( void ) const {
    return new RandomMoveSchedule(*this);
}

double RandomMoveSchedule::getNumberMovesPerIteration( void ) const {
    return movesPerIteration;
}


Move* RandomMoveSchedule::nextMove( unsigned long gen ) {
    
    movesPerIteration = 0.0;
    for (size_t i = 0; i < weights.size(); ++i)
    {
        if ( moves[i]->isActive( gen ) )
        {
            movesPerIteration += weights[i];
        }
    }
    
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double u = movesPerIteration * rng->uniform01();
    
    int index = 0;
    // only if the move is inactive or the weight of the move is smaller than u
    while ( !moves[index]->isActive(gen) || weights[index] <= u ) 
    {
        // check if this move is active
        // if not, then we just subtract the weight of this move
        if ( moves[index]->isActive( gen ) )
        {
            u -= weights[index];
        }
        ++index;
    }
    
    return moves[index];
}
