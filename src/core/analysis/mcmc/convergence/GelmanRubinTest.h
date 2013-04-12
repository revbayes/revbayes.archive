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
        GelmanRubinTest(double R, int n);
    
        // implementen functions from convergence diagnostic
        bool        assessConvergenceSingleChain(const std::vector<double>& values, int burnin);
        bool        assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<int>& burnin);
    
    private:
    
        double      R;                                                                                                  //!< threshold value for potential scale reduction factor
        int         nBatches;
    
    };

}

#endif
