#include "StationarityTest.h"
#include "DistributionNormal.h"

#include <cmath>

using namespace RevBayesCore;
using namespace std;


StationarityTest::StationarityTest(size_t b, double f) : ConvergenceDiagnosticContinuous(),
    nBlocks( b ),
    p( f )
{

}

bool StationarityTest::assessConvergenceSingleChain(const std::vector<double>& values, size_t burnin)
{
    // calculate the block size
    size_t blockSize = (values.size()-burnin) / nBlocks;
    
    // use correction for multiple sampling
    double p_corrected = pow(1.0-p, 1.0/nBlocks);
    
    // get the mean for the trace
    analysis.analyseMean(values,burnin);
    double traceMean = analysis.getMean();
    
    // get a mean and standard error for each block
    std::vector<double> blockMeans =  std::vector<double>(nBlocks,0.0);
    std::vector<double> blockSem =  std::vector<double>(nBlocks,0.0);
    for (size_t i=0; i<nBlocks; i++)
    {
        analysis.analyseMean(values,i*blockSize+burnin,(i+1)*blockSize+burnin);
        blockMeans[i]   = analysis.getMean();
        
        analysis.analyseCorrelation(values,i*blockSize+burnin,(i+1)*blockSize+burnin);
        blockSem[i]     = analysis.getStdErrorOfMean();
        
        // get the quantile of a normal with mu=0, var=sem and p=(1-p_corrected)/2
        double quantile = RbStatistics::Normal::quantile(0.0, blockSem[i], p_corrected);
        // check if the trace mean is outside this confidence interval
        if (blockMeans[i]-quantile > traceMean || blockMeans[i]+quantile < traceMean)
        {
            // the mean of the whole trace falls out of the confidence interval for this block and hence we cannot reject with p-confidence that the trace has not converged
            return false;
        }
    }
    
    return true;
}

bool StationarityTest::assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<size_t>& burnins)
{
    
    // get number of chains
    size_t nChains = values.size();
    // use correction for multiple sampling
    double p_corrected = pow(1.0-p, 1.0/nChains);
    
    // get the mean between all traces
    analysis.analyseMean(values,burnins);
    double totalMean = analysis.getMean();
    
    // get a mean and standard error for each block
    std::vector<double> chainMeans =  std::vector<double>(nChains,0.0);
    std::vector<double> chainSem =  std::vector<double>(nChains,0.0);
    for (size_t i=0; i<nChains; i++)
    {
        const std::vector<double>& chain    = values.at(i);
        size_t burnin                       = burnins.at(i);
        analysis.analyseMean(chain,burnin);
        chainMeans[i]                       = analysis.getMean();
        
        analysis.analyseCorrelation(chain,burnin);
        chainSem[i]                         = analysis.getStdErrorOfMean();
        
        // get the quantile of a normal with mu=0, var=sem and p=(1-p_corrected)/2
        double quantile = RbStatistics::Normal::quantile(0.0, chainSem[i], p_corrected);
        // check if the trace mean is outside this confidence interval
        if (chainMeans[i]-quantile > totalMean || chainMeans[i]+quantile < totalMean)
        {
            // the mean of the whole trace falls out of the confidence interval for this block and hence we cannot reject with p-confidence that the trace has not converged
            return false;
        }
    }
    
    return true;
}
