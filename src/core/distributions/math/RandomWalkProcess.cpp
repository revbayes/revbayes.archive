//
//  RandomWalkProcess.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/5/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RandomWalkProcess.h"


using namespace RevBayesCore;

RandomWalkProcess::RandomWalkProcess( LinearStepFunction *v ) : TypedDistribution<UnivariateFunction>( v ) {
    // nothing to do here
}



RandomWalkProcess::RandomWalkProcess( const RandomWalkProcess &n ) : TypedDistribution<UnivariateFunction>( n ) {
    // nothing to do here
}


RandomWalkProcess::~RandomWalkProcess( void ) {
    // nothing to do here
}
