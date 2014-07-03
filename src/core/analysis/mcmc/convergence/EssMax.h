#ifndef EssMax_H
#define EssMax_H

#include <vector>

#include "BurninEstimatorContinuous.h"

namespace RevBayesCore {

    class EssMax : public BurninEstimatorContinuous {
    
    public:
        EssMax();
        EssMax(size_t blockSize);
    
        // implementen functions from convergence diagnostic
        size_t      estimateBurnin(const std::vector<double>& values);
    
    private:
    
        size_t      blockSize;                                                                                          //!< first window
    
    
    };
    
}

#endif
