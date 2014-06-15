//
//  EssMax.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/12/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef EssMax_H
#define EssMax_H

#include <vector>

#include "BurninEstimatorContinuous.h"

using namespace RevBayesCore;

class EssMax : public BurninEstimatorContinuous {
    
public:
    EssMax();
    EssMax(size_t blockSize);
    
    // implementen functions from convergence diagnostic
    size_t      estimateBurnin(const std::vector<double>& values);
    
private:
    
    size_t      blockSize;                                                                                          //!< first window
    
    
};

#endif
