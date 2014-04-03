//
//  RateMatrix_Biogeography.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMatrix_Biogeography.h"
#include "RateFunctionMatrix.h"
#include "RbException.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Biogeography::RateMatrix_Biogeography(size_t ns, size_t nc) : RateFunctionMatrix( ns, nc ), distancePower(0.0), gainLossRates(  std::vector<double>(2,0.5) ) {
    
    updateMatrix();
}


/** Copy constructor */
RateMatrix_Biogeography::RateMatrix_Biogeography(const RateMatrix_Biogeography& m) : RateFunctionMatrix( m ), distancePower( m.distancePower ), gainLossRates(m.gainLossRates) {
    
}


/** Destructor */
RateMatrix_Biogeography::~RateMatrix_Biogeography(void) {
    
}


RateMatrix_Biogeography& RateMatrix_Biogeography::operator=(const RateMatrix_Biogeography &r) {
    
    if (this != &r)
    {
        RateFunctionMatrix::operator=( r );
        gainLossRates = r.gainLossRates;
        distancePower = r.distancePower;
    }
    
    return *this;
}

RateMatrix_Biogeography* RateMatrix_Biogeography::clone( void ) const {
    return new RateMatrix_Biogeography( *this );
}

double RateMatrix_Biogeography::getRate(DiscreteCharacterState* fromState, DiscreteCharacterState* toState) const
{
    return 1.0;
}

double RateMatrix_Biogeography::getSumOfRates(DiscreteCharacterState* fromState) const
{
    return 1.0;
}

void RateMatrix_Biogeography::setDistancePower(double d)
{
    distancePower = d;
    needsUpdate = true;
}

void RateMatrix_Biogeography::setGainLossRates(const std::vector<double> &r)
{
    gainLossRates = r;
    needsUpdate = true;
}


void RateMatrix_Biogeography::updateMatrix( void ) {
    
    if ( needsUpdate )
    {
        // update local variables accordingly...
        
        // maybe recompute the eff. geography distances??
        
        // clean flags
        needsUpdate = false;
    }
}

