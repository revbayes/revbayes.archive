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
        StationarityTest(int nBlocks, double p);
    
        // implementen functions from convergence diagnostic
        bool        assessConvergenceSingleChain(const std::vector<double>& values, int burnin);
        bool        assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<int>& burnin);
    
        // setters
        void        setNBlocks(int n) { nBlocks = n; }
        void        setP(double f) { this->p = f; }
    
    private:
    
        int         nBlocks;                                                                                            //!< number of blocks 
        double      p;                                                                                                  //!< sginificance level
    
    };

}

#endif

