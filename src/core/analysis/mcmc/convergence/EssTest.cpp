#include "EssTest.h"

using namespace RevBayesCore;


EssTest::EssTest(double t) : ConvergenceDiagnosticContinuous(),
    k( t )
{
    
}


bool EssTest::assessConvergenceSingleChain(const std::vector<double> &values, size_t burnin)
{
    
    // make mean invalid for recalculation
    analysis.analyseMean(values, burnin);
    
    // analyse trace for this burnin
    analysis.analyseCorrelation(values,burnin);
    
    double ess = analysis.getEss();
    
    return  ess > k;
}

