//
//  RateGenerator.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/17/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "RateGenerator.h"
#include "RbException.h"
#include "RbMathMatrix.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateGenerator::RateGenerator(size_t n) :
    numStates( n )
{
    ; // do nothing
}

RateGenerator::~RateGenerator(void)
{
    ; // do nothing
}


RateGenerator& RateGenerator::assign(const Assignable &m)
{
    const RateGenerator *rm = dynamic_cast<const RateGenerator*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

void RateGenerator::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const
{
    calculateTransitionProbabilities(t, 0.0, 1.0, P);
}

size_t RateGenerator::getNumberOfStates( void ) const
{
    return numStates;
}



size_t RateGenerator::size( void ) const
{
    return numStates;
}