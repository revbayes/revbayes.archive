//
//  LevyJumpMove.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 4/4/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#include "LevyJumpMove.h"

#include "ContinuousStochasticNode.h"
#include "DistributionNormal.h"
#include "LevyJumpMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "RbUtil.h"

#include <cmath>

using namespace RevBayesCore;

LevyJumpMove::LevyJumpMove(ContinuousStochasticNode* v, double d, bool t, double w) : SimpleMove( v, w, t ), variable( v ), delta( d ) {
    
}


/** Clone object */
LevyJumpMove* LevyJumpMove::clone( void ) const {
    
    return new LevyJumpMove( *this );
}



const std::string& LevyJumpMove::getMoveName( void ) const {
    static std::string name = "LevyJumpMove";
    
    return name;
}



/** Perform the move */
double LevyJumpMove::performSimpleMove( void )
{
    
    // store variable for restoration
    double sv = variable->getValue();
    storedValue = sv;

    // turn on/off variable
    double u = GLOBAL_RNG->uniform01();
    double p = 0.5;
    double off = double( u > p );
    
    // propose new value
    double dv = RbStatistics::Normal::rv(0.0, delta, *GLOBAL_RNG);
    double nv = off * (sv + dv);
    variable->setValue( new double(nv) );
    
    // MH ratio
    double probFwd = 1.0;
    double probRev = 1.0;
    if (off == 0.0 && sv != 0.0)
    {
        probRev = 0 + (1-p)*RbStatistics::Normal::pdf(0.0, delta, sv);
        probFwd = p + (1-p)*RbStatistics::Normal::pdf(sv, delta, 0.0);
    }
    if (off == 1.0 && sv == 0.0)
    {
        probRev = p + (1-p)*RbStatistics::Normal::pdf(nv, delta, sv);
        probFwd = 0 + (1-p)*RbStatistics::Normal::pdf(sv, delta, nv);
    }
    
	return log( probRev / probFwd );
}


void LevyJumpMove::printParameterSummary(std::ostream &o) const
{
    o << "delta = " << delta;
}


void LevyJumpMove::rejectSimpleMove( void )
{
    // swap current value and stored value
    variable->setValue( new double(storedValue) );
}


void LevyJumpMove::swapNode(DagNode *oldN, DagNode *newN)
{
    // call the parent method
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<ContinuousStochasticNode* >( newN );
}


void LevyJumpMove::tune( void ) {
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.234 ) {
        delta /= (1.0 + ((rate-0.234)/0.766) );
    }
    else {
        delta *= (2.0 - rate/0.234 );
    }
}
