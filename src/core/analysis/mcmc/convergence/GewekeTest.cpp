//
//  GewekeTest.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "GewekeTest.h"
#include "DistributionNormal.h"

#include <cmath>

using namespace RevBayesCore;

GewekeTest::GewekeTest() : ConvergenceDiagnosticContinuous() {
    
    p       = 0.01;
    frac1   = 0.1;
    frac2   = 0.5;
}

GewekeTest::GewekeTest(double f) : ConvergenceDiagnosticContinuous() {
    this->p         = f;
    this->frac1     = 0.1;
    this->frac2     = 0.5;
}

GewekeTest::GewekeTest(double f, double f1, double f2) : ConvergenceDiagnosticContinuous() {
    this->p         = f;
    this->frac1     = f1;
    this->frac2     = f2;
}

bool GewekeTest::assessConvergenceSingleChain(const std::vector<double>& values, size_t burnin) {
    // get the sample size
    size_t sampleSize = values.size() - burnin;
    
    // set the indices for start and end of the first window
    size_t startwindow1    = burnin;
    size_t endWindow1      = size_t(sampleSize * frac1) + burnin;
    
    // get mean and variance of the first window
    analysis.analyseMean(values, startwindow1, endWindow1);
    double meanWindow1  = analysis.getMean();
    analysis.analyseCorrelation(values, startwindow1, endWindow1);
    double varWindow1   = analysis.getStdErrorOfMean()*analysis.getStdErrorOfMean();
    
    // set the indices for start and end of the second window
    size_t startwindow2    = values.size() - size_t(sampleSize * frac2);
    size_t endWindow2      = values.size();
    
    // get mean and variance of the second window
    analysis.analyseMean(values, startwindow2, endWindow2);
    double meanWindow2  = analysis.getMean();
    analysis.analyseCorrelation(values, startwindow2, endWindow2);
    double varWindow2   = analysis.getStdErrorOfMean()*analysis.getStdErrorOfMean();
    
    // get z
    double z            = (meanWindow1 - meanWindow2)/sqrt(varWindow1 + varWindow2);
    // check if z is standard normally distributed
//    double quantile     = RbStatistics::Normal::quantile(p/2.0);
    double cdf          = RbStatistics::Normal::cdf(z);
//    bool passed1        = (z > quantile); 
//    bool passed2        = z < (-quantile);
//    bool passed         = (passed1 && passed2);
//    bool test           = ( z > quantile && z < -quantile );
    
    return cdf > p/2.0 && cdf < (1.0 - p/2.0);
}
