//
//  GelmanRubinTest.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/11/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "GelmanRubinTest.h"
#include "DistributionNormal.h"

#include <cmath>

using namespace RevBayesCore;

GelmanRubinTest::GelmanRubinTest() : ConvergenceDiagnosticContinuous() {
    
    R           = 1.001;
    nBatches    = 10;
}

GelmanRubinTest::GelmanRubinTest(double r) : ConvergenceDiagnosticContinuous() {
    
    this->R     = r;
    nBatches    = 10;
}

GelmanRubinTest::GelmanRubinTest(double r, int n) : ConvergenceDiagnosticContinuous() {
    
    this->R     = r;
    nBatches    = n;
}

bool GelmanRubinTest::assessConvergenceSingleChain(const std::vector<double>& values, int burnin) {
    double  withinBatchVariance             = 0;
    double  betweenBatchVariance            = 0;
    
    int     totalSampleSize                 = (int)values.size() - burnin;
    double  batchSize                       = totalSampleSize / (double)nBatches;
    
    // get the mean between all traces
    analysis.analyseMean(values,burnin);
    double totalMean = analysis.getMean();
    
    // get a mean and standard error for each block
    double* batchMeans  = new double[nBatches];
    for (int i=0; i<nBatches; i++) {
        analysis.analyseMean(values,int(i*batchSize+burnin),int((i+1)*batchSize+burnin));
        batchMeans[i]                       = analysis.getMean();
        
        // iterate over all samples from the chains
        for (int j=int(i*batchSize+burnin); j<int((i+1)*batchSize+burnin); j++) {
            withinBatchVariance             += ( (values.at(j) - batchMeans[i])*(values.at(j) - batchMeans[i]) );
            betweenBatchVariance            += ( (values.at(j) - totalMean)*(values.at(i) - totalMean) );
        }
    }
    
    double psrf                             = ((totalSampleSize-nBatches) / (totalSampleSize-1.0)) * (betweenBatchVariance/withinBatchVariance);
    
    
    delete[] batchMeans;
    
    return psrf < R;
}

bool GelmanRubinTest::assessConvergenceMultipleChains(const std::vector<std::vector<double> >& values, const std::vector<int>& burnins) {
    double  withinChainVariance             = 0;
    double  betweenChainVariance            = 0;
    int     totalSampleSize                 = 0;
    
    // get number of chains
    int nChains = (int) values.size();
    
    // get the mean between all traces
    analysis.analyseMean(values,burnins);
    double totalMean = analysis.getMean();
    
    // get a mean and standard error for each block
    double* chainMeans  = new double[nChains];
    for (int i=0; i<nChains; i++) {
        const std::vector<double>& chain    = values.at(i);
        int burnin                          = burnins.at(i);
        analysis.analyseMean(chain,burnin);
        chainMeans[i]                       = analysis.getMean();
    }
    // iterate over all chains
    for (int i=0; i<nChains; i++) {
        const std::vector<double>& chain    = values.at(i);
        int burnin                          = burnins.at(i);
        
        int chainSize                       = (int)chain.size() - burnin;
        // add this chain size to the total sample size
        totalSampleSize                     += chainSize;
        // iterate over all samples from the chains
        for (int j=burnin; j<chainSize; j++) {
            withinChainVariance             += ( (chain.at(j) - chainMeans[i])*(chain.at(j) - chainMeans[i]) );
            betweenChainVariance            += ( (chain.at(j) - totalMean)*(chain.at(j) - totalMean) );
        }
    }
    
    double psrf                             = ((totalSampleSize-nChains) / (totalSampleSize-1.0)) * (betweenChainVariance/withinChainVariance);
    
    
    delete[] chainMeans;
    
    return psrf < R;
}
