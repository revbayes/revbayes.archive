//
//  SequenctialMoveSchedule.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

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


Move* RandomMoveSchedule::nextMove( void ) {
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double u = movesPerIteration * rng->uniform01();
    
    int index = 0;
    while ( weights[index] < u ) {
        u -= weights[index];
        ++index;
    }
    
    return moves[index];
}
