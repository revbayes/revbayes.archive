//
//  AbstractOldMove.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 6/11/14.
//  Copyright (c) 2014 hoehna. All rights reserved.
//

#include "AbstractOldMove.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

#include <cmath>

using namespace RevBayesCore;


AbstractOldMove::AbstractOldMove() {
    
    
}


AbstractOldMove::~AbstractOldMove() {
    
}


void AbstractOldMove::perform( double heat ) {
    
    if ( isGibbs() )
    {
        // do Gibbs proposal
        performGibbs();
        // theMove->accept(); // Not necessary, because Gibbs samplers are automatically accepted.
    }
    else
    {
        // do a Metropolois-Hastings proposal
        
        // Propose a new value
        double lnProbabilityRatio;
        double lnHastingsRatio = performOld(lnProbabilityRatio);
        
        
        // Calculate acceptance ratio
        double lnR = heat * (lnProbabilityRatio) + lnHastingsRatio;
        
        if (lnR >= 0.0)
        {
            accept();
        }
        else if (lnR < -300.0)
        {
            ;
            reject();
        }
        else
        {
            double r = exp(lnR);
            // Accept or reject the move
            double u = GLOBAL_RNG->uniform01();
            if (u < r)
            {
                accept();
            }
            else
            {
                reject();
            }
        }
    }
    


}
