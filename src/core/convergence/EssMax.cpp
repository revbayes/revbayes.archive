//
//  EssMax.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "EssMax.h"

#include "RbConstants.h"

#include <cmath>

EssMax::EssMax() {
    blockSize = 10;
}

EssMax::EssMax(int blockSize) {
    this->blockSize     = blockSize;
}

int EssMax::estimateBurnin(const std::vector<double>& values) {
    // init
    double  max_ess     = 0;
    int     best_burnin = 0;
    
    // iterate over possible burnins
    for (int i=0; i<(int)values.size(); i+=blockSize) {
        // make mean invalid for recalculation
        analysis.analyseMean(values, i);
        // analyse trace for this burnin
        analysis.analyseCorrelation(values,i);
        
        // check if the new ess is better than any previous ones
        //! @Sebastian: finite not available in MS compiler
#if 0
        if (finite(analysis.getEss()) && max_ess < analysis.getEss()) {
            max_ess = analysis.getEss();
            best_burnin = i;
        }
#endif
    }
    
    // return the best burnin
    return best_burnin;
}