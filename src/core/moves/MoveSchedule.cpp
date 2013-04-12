//
//  MoveSchedule.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MoveSchedule.h"

using namespace RevBayesCore;

MoveSchedule::MoveSchedule(const std::vector<Move*> &m ) : moves( m ) {
    
}


MoveSchedule::~MoveSchedule() {
    // we own nothing
}
