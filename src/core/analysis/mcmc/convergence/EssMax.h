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
    EssMax(int blockSize);
    
    // implementen functions from convergence diagnostic
    int         estimateBurnin(const std::vector<double>& values);
    
private:
    
    int         blockSize;                                                                                          //!< first window 
    
    
};

#endif
