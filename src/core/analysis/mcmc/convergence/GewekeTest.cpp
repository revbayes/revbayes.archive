#include "GewekeTest.h"
#include "DistributionNormal.h"

#include <cmath>

using namespace RevBayesCore;
using namespace std;

GewekeTest::GewekeTest(double f, double f1, double f2) : ConvergenceDiagnosticContinuous(),
    frac1( f1 ),
    frac2( f2 ),
    p( f )
{
    
}

bool GewekeTest::assessConvergence(const TraceNumeric& trace)
{
    // get the sample size
    size_t sampleSize = trace.size(true);
    
    // set the indices for start and end of the first window
    size_t startwindow1    = trace.getBurnin();
    size_t endWindow1      = size_t(sampleSize * frac1) + trace.getBurnin();
    
    // get mean and variance of the first window
    trace.computeCorrelation(startwindow1, endWindow1);
    double meanWindow1  = trace.getMean();
    double varWindow1   = trace.getSem()*trace.getSem();
    
    // set the indices for start and end of the second window
    size_t startwindow2    = trace.size() - size_t(sampleSize * frac2);
    size_t endWindow2      = trace.size();
    
    // get mean and variance of the second window
    trace.computeCorrelation(startwindow2, endWindow2);
    double meanWindow2  = trace.getMean();
    double varWindow2   = trace.getSem()*trace.getSem();
    
    // get z
    double z            = (meanWindow1 - meanWindow2)/sqrt(varWindow1 + varWindow2);
    // check if z is standard normally distributed
    double cdf          = RbStatistics::Normal::cdf(z);
    
    return cdf > p/2.0 && cdf < (1.0 - p/2.0);
}
