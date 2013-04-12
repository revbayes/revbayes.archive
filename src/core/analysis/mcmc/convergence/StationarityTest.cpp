//
//  StationarityTest.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "StationarityTest.h"
#include "DistributionNormal.h"

#include <cmath>

using namespace RevBayesCore;

StationarityTest::StationarityTest() : ConvergenceDiagnosticContinuous() {
    
    nBlocks = 10;
    p       = 0.01;
}

StationarityTest::StationarityTest(double f) : ConvergenceDiagnosticContinuous() {
    
    nBlocks = 10;
    p       = f;
}

StationarityTest::StationarityTest(int b, double f) : ConvergenceDiagnosticContinuous() {
    nBlocks   = b;
    p         = f;
}

bool StationarityTest::assessConvergenceSingleChain(const std::vector<double>& values, int burnin) {
    // calculate the block size
    double blockSize = ((int)values.size()-burnin) / nBlocks;
    
    // use correction for multiple sampling
    double p_corrected = pow(1.0-p, 1.0/nBlocks);
    
    // get the mean for the trace
    analysis.analyseMean(values,burnin);
    double traceMean = analysis.getMean();
    
    // get a mean and standard error for each block
    double* blockMeans = new double[nBlocks];
    double* blockSem = new double[nBlocks];
    for (int i=0; i<nBlocks; i++) {
        analysis.analyseMean(values,int(i*blockSize+burnin),int((i+1)*blockSize+burnin));
        blockMeans[i]   = analysis.getMean();
        
        analysis.analyseCorrelation(values,int(i*blockSize+burnin),int((i+1)*blockSize+burnin));
        blockSem[i]     = analysis.getStdErrorOfMean();
        
        // get the quantile of a normal with mu=0, var=sem and p=(1-p_corrected)/2
        double quantile = RbStatistics::Normal::quantile(0.0, blockSem[i], p_corrected);
        // check if the trace mean is outside this confidence interval
        if (blockMeans[i]-quantile > traceMean || blockMeans[i]+quantile < traceMean) {
            // the mean of the whole trace falls out of the confidence interval for this block and hence we cannot reject with p-confidence that the trace has not converged
            delete[] blockMeans;
            delete[] blockSem;
            
            return false;
        }
    }
    
    delete[] blockMeans;
    delete[] blockSem;
    
    return true;
}

bool StationarityTest::assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<int>& burnins) {
    // get number of chains
    int nChains = (int) values.size();
    // use correction for multiple sampling
    double p_corrected = pow(1.0-p, 1.0/nChains);
    
    // get the mean between all traces
    analysis.analyseMean(values,burnins);
    double totalMean = analysis.getMean();
    
    // get a mean and standard error for each block
    double* chainMeans  = new double[nChains];
    double* chainSem    = new double[nChains];
    for (int i=0; i<nChains; i++) {
        const std::vector<double>& chain    = values.at(i);
        int burnin                          = burnins.at(i);
        analysis.analyseMean(chain,burnin);
        chainMeans[i]                       = analysis.getMean();
        
        analysis.analyseCorrelation(chain,burnin);
        chainSem[i]                         = analysis.getStdErrorOfMean();
        
        // get the quantile of a normal with mu=0, var=sem and p=(1-p_corrected)/2
        double quantile = RbStatistics::Normal::quantile(0.0, chainSem[i], p_corrected);
        // check if the trace mean is outside this confidence interval
        if (chainMeans[i]-quantile > totalMean || chainMeans[i]+quantile < totalMean) {
            // the mean of the whole trace falls out of the confidence interval for this block and hence we cannot reject with p-confidence that the trace has not converged
            delete[] chainMeans;
            delete[] chainSem;
            
            return false;
        }
    }
    
    delete[] chainMeans;
    delete[] chainSem;
    
    return true;
}
