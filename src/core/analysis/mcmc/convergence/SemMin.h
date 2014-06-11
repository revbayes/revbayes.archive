//
//  SemMin.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef SemMin_H
#define SemMin_H

#include <vector>

#include "BurninEstimatorContinuous.h"

namespace RevBayesCore {
    
    class SemMin : public BurninEstimatorContinuous {
    
    public:
        SemMin();
        SemMin(size_t blockSize);
    
        // implementen functions from convergence diagnostic
        size_t     estimateBurnin(const std::vector<double>& values);
    
    private:
    
        size_t      blockSize;                                                                                          //!< first window
    
    };
    
}

#endif
