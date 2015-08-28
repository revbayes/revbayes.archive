//
//  EssMax.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "EssMax.h"
#include "RbConstants.h"
#include "RbMathLogic.h"

#include <cmath>

using namespace RevBayesCore;
using namespace std;

EssMax::EssMax(size_t b, double f) :
    blockSize( b ),
    frac( f )
{

}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
EssMax* EssMax::clone( void ) const
{
    
    return new EssMax( *this );
}

size_t EssMax::estimateBurnin(const std::vector<double>& values)
{
    
    // init
    double  max_ess     = 0;
    size_t  best_burnin = 0;
    
    // iterate over possible burnins
    for (size_t i=0; i<(frac*values.size()); i+=blockSize) {
        // make mean invalid for recalculation
        analysis.analyseMean(values, i);
        // analyse trace for this burnin
        analysis.analyseCorrelation(values,i);
        
        // check if the new ess is better than any previous ones
        if (RbMath::isFinite(analysis.getEss()) && max_ess < analysis.getEss()) {
            max_ess = analysis.getEss();
            best_burnin = i;
        }
    }
    
    // return the best burnin
    return best_burnin;
}
