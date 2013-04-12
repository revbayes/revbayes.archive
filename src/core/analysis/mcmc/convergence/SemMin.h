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
        SemMin(int blockSize);
    
        // implementen functions from convergence diagnostic
        int         estimateBurnin(const std::vector<double>& values);
    
    private:
    
        int         blockSize;                                                                                          //!< first window 
    
    };
    
}

#endif
