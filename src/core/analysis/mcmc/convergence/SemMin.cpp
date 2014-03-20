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

SemMin::SemMin() {
    blockSize = 10;
}

SemMin::SemMin(int b) {
    this->blockSize     = b;
}

int SemMin::estimateBurnin(const std::vector<double>& values) {
    // init
    double  min_sem     = RbConstants::Double::max;
    int     best_burnin = 0;
    
    // iterate over possible burnins
    for (int i=0; i<(int)values.size(); i+=blockSize) {
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
