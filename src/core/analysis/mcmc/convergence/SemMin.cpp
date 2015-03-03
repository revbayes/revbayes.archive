//
//  SemMin.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SemMin.h"
#include "RbConstants.h"
#include "RbMathLogic.h"

#include <cmath>

using namespace RevBayesCore;
using namespace std;

SemMin::SemMin()
{
    blockSize = 10;
}

SemMin::SemMin(size_t b)
{
    this->blockSize     = b;
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
SemMin* SemMin::clone( void ) const
{
    
    return new SemMin( *this );
}


size_t SemMin::estimateBurnin(const std::vector<double>& values) {
    // init
    double  min_sem     = RbConstants::Double::max;
    size_t  best_burnin = 0;
    
    // iterate over possible burnins
    for (size_t i=0; i<values.size(); i+=blockSize) {
        // make mean invalid for recalculation
        analysis.analyseMean(values, i);
        // analyse trace for this burnin
        analysis.analyseCorrelation(values,i);
        
        // check if the new ess is better than any previous ones
        if (RbMath::isFinite(analysis.getStdErrorOfMean()) && analysis.getStdErrorOfMean() > 0 && min_sem > analysis.getStdErrorOfMean()) {
            min_sem = analysis.getStdErrorOfMean();
            best_burnin = i;
        }
    }
    
    // return the best burnin
    return best_burnin;
}
