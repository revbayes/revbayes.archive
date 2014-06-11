//
//  GelmanRubinTest.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef GelmanRubinTest_H
#define GelmanRubinTest_H

#include <vector>

#include "ConvergenceDiagnosticContinuous.h"

namespace RevBayesCore {

    class GelmanRubinTest : public ConvergenceDiagnosticContinuous {
    
    public:
        GelmanRubinTest();
        GelmanRubinTest(double R);
        GelmanRubinTest(double R, size_t n);
    
        // implementen functions from convergence diagnostic
        bool        assessConvergenceSingleChain(const std::vector<double>& values, size_t burnin);
        bool        assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<size_t>& burnin);
    
    private:
    
        double      R;                                                                                                  //!< threshold value for potential scale reduction factor
        size_t      nBatches;
    
    };

}

#endif
