//
//  GewekeTest.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef GewekeTest_H
#define GewekeTest_H

#include <vector>

#include "ConvergenceDiagnosticContinuous.h"

namespace RevBayesCore {

    class GewekeTest : public ConvergenceDiagnosticContinuous {
    
    public:
        GewekeTest();
        GewekeTest(double p);
        GewekeTest(double p, double f1, double f2);
    
        // implementen functions from convergence diagnostic
        bool        assessConvergenceSingleChain(const std::vector<double>& values, int burnin);
    
        // setters
        void        setFraction1(double f1) { frac1 = f1; }
        void        setFraction2(double f2) { frac2 = f2; }
    
    private:
    
        double      frac1;                                                                                          //!< first window 
        double      frac2;                                                                                          //!< second window
        double      p;                                                                                              //!< the significance level
    
    };

}

#endif
