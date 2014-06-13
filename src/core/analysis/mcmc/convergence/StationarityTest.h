//
//  StationarityTest.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef StationarityTest_H
#define StationarityTest_H

#include <vector>

#include "ConvergenceDiagnosticContinuous.h"

namespace RevBayesCore {
    
    class StationarityTest : public ConvergenceDiagnosticContinuous {
    
    public:
        StationarityTest();
        StationarityTest(double p);
        StationarityTest(size_t nBlocks, double p);
    
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

