//
//  BurninEstimator.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef BurninEstimatorContinuous_H
#define BurninEstimatorContinuous_H

#include "TraceAnalysisContinuous.h"

#include <vector>

namespace RevBayesCore {
    
    class BurninEstimatorContinuous {
    
    public:
        BurninEstimatorContinuous();
        virtual                    ~BurninEstimatorContinuous() {}
    
        virtual size_t              estimateBurnin(const std::vector<double>& values) = 0;
    
    protected:
    
        TraceAnalysisContinuous     analysis;
    
    };
    
}

#endif
