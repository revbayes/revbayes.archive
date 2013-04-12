//
//  ConvergenceDiagnostic.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef ConvergenceDiagnosticContinuous_H
#define ConvergenceDiagnosticContinuous_H

#include "TraceAnalysisContinuous.h"

#include <vector>

namespace RevBayesCore {
    
    class ConvergenceDiagnosticContinuous {
    
    public:
        ConvergenceDiagnosticContinuous();
        virtual                    ~ConvergenceDiagnosticContinuous(void) {}
    
        virtual bool                assessConvergenceSingleChain(const std::vector<double>& values, int burnin) { return false; }
        virtual bool                assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<int>& burnin) { return false; }
    
    protected:
    
        TraceAnalysisContinuous     analysis;
        
    };

}

#endif
