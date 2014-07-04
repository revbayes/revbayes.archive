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

bool GewekeTest::assessConvergenceSingleChain(const std::vector<double>& values, size_t burnin)
{
    // get the number of values
    size_t valueCount = values.size();
    
    // get the sample size
    size_t sampleSize = valueCount - burnin;
    
    // set the indices for start and end of the first window
    size_t startwindow1    = burnin;
    size_t endWindow1      = size_t(sampleSize * frac1) + burnin;
    
    // get mean and variance of the first window
    analysis.analyseMean(values, startwindow1, endWindow1);
    double meanWindow1  = analysis.getMean();
    analysis.analyseCorrelation(values, startwindow1, endWindow1);
    double varWindow1   = analysis.getStdErrorOfMean()*analysis.getStdErrorOfMean();
    
    // set the indices for start and end of the second window
    size_t startwindow2    = valueCount - size_t(sampleSize * frac2);
    size_t endWindow2      = valueCount;
    
    // get mean and variance of the second window
    analysis.analyseMean(values, startwindow2, endWindow2);
    double meanWindow2  = analysis.getMean();
    analysis.analyseCorrelation(values, startwindow2, endWindow2);
    double varWindow2   = analysis.getStdErrorOfMean()*analysis.getStdErrorOfMean();
    
    // get z
    double z            = (meanWindow1 - meanWindow2)/sqrt(varWindow1 + varWindow2);
    // check if z is standard normally distributed
    double cdf          = RbStatistics::Normal::cdf(z);
    
    return cdf > p/2.0 && cdf < (1.0 - p/2.0);
}
