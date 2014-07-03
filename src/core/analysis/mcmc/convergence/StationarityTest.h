#ifndef StationarityTest_H
#define StationarityTest_H

#include <vector>

#include "ConvergenceDiagnosticContinuous.h"

namespace RevBayesCore {
    
    /**
     * @brief Stationarity test statistic for assessing convergence.
     *
     * The stationarity test statistic computes the probability that the samples in a given
     * interval is different from the samples of the entire chain. This is done by comparing
     * the mean values of the two samples. The interval is slided over the entire chain.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2011-04-11
     *
     */
    class StationarityTest : public ConvergenceDiagnosticContinuous {
    
    public:
        StationarityTest(size_t nBlocks=10, double p=0.01);
    
        // implementen functions from convergence diagnostic
        bool        assessConvergenceSingleChain(const std::vector<double>& values, size_t burnin);
        bool        assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<size_t>& burnin);
    
        // setters
        void        setNBlocks(size_t n) { nBlocks = n; }
        void        setP(double f) { this->p = f; }
    
    private:
    
        size_t      nBlocks;                                                                                            //!< number of blocks 
        double      p;                                                                                                  //!< sginificance level
    
    };

}

#endif

