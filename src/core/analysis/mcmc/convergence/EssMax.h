#ifndef EssMax_H
#define EssMax_H

#include <vector>

#include "BurninEstimatorContinuous.h"

namespace RevBayesCore {

    class EssMax : public BurninEstimatorContinuous {
    
    public:
        EssMax(size_t b=10, double f=0.5);
    
        // implementen functions from convergence diagnostic
        size_t      estimateBurnin(const std::vector<double>& values);
    
    private:
    
        size_t      blockSize;                                                                                          //!< first window
        double      frac;
    
    };
    
}

#endif
