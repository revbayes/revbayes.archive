//
//  TraceAnalysisCategorical.cpp
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "TraceAnalysisCategorical.h"

#include "DistNormal.h"
#include "RbConstants.h"

#include <cmath>
#include <math.h>



/**
 * 
 */
TraceAnalysisCategorical::TraceAnalysisCategorical() {
    // initialize data to invalid values
//    invalidateTraceStatistics();
    //    burnin               = new int;
    burnin              = RbConstants::Integer::nan;
    
    
}

void TraceAnalysisCategorical::analyseMean(const std::vector<bool>& values) {
    // make sure the burnin is valid
    int burnin = 0;
    
    analyseMean(values, burnin);
}

void TraceAnalysisCategorical::analyseMean(const std::vector<bool>& values, int burnin) {
    double m = 0;
    int size = (int)values.size();
    for (int i=burnin; i<size; i++) {
        if (values.at(i)) {
            m++;
        }
    }
    
    mean = m/(size-burnin);
}

void TraceAnalysisCategorical::analyseMean(const std::vector<std::vector<bool> >& values, const std::vector<int>& burnins) {
    double m = 0;
    int sampleSize = 0;
    // get the number of chains
    int chains = (int)values.size();
    // iterate over all chains
    for (int j=0; j<chains; j++) {
        // get the chain
        const std::vector<bool>& chain = values.at(j);
        int burnin = burnins.at(j);
        int chainSize = (int) chain.size() - burnin;
        // add this chain size to the total sample size
        sampleSize += chainSize;
        for (int i=burnin; i<chainSize; i++) {
            if (chain.at(i)) {
                m++;
            }
        }
    }
    
    mean = m/sampleSize;
}

void TraceAnalysisCategorical::analyseMean(const std::vector<bool>& values, int begin, int end) {
    double m = 0;
    for (int i=begin; i<end; i++) {
        m += values.at(i);
    }
    
    mean = m/(end-begin);
}

/**
 * Analyze trace
 *
 */
void TraceAnalysisCategorical::analyseCorrelation(const std::vector<bool>& values) {
    // make sure the burnin is valid
    int burnin = 0;
    
    analyseCorrelation(values,burnin);
}

/**
 * Analyze trace
 *
 * @param burnin   the number of sampes to discard
 *
 * @attention This method assumes that the mean was either made invalid before execution or is calculated apropriately for this burnin.
 */
void TraceAnalysisCategorical::analyseCorrelation(const std::vector<bool>& values, int burnin) {
    // if we have not yet calculated the mean, do this now
    if (mean == RbConstants::Double::max) {
        analyseMean(values,burnin);
    }
    
    int samples = (int) values.size() - burnin;
    
    // calcualte the expected time before switching the state
    double transitionTime = 1.0 / (1.0-mean);
    
    // calculate the observed time spent on average in the active state before flipping
    int numberVisited   = 0;
    int numberLeft      = 0;
    for (int i=burnin; i<values.size(); i++) {
        if (values.at(i)) {
            numberVisited++;
            if (i < values.size()-1 && !values.at(i+1)) {
                numberLeft++;
            }
        }
    }
    
    double observedTransitionTime = numberVisited / (double) numberLeft;
    
    // auto correlation time
    act = observedTransitionTime / transitionTime;
    
    // effective sample size
    ess = samples / act;
    
    // standard error of mean
//    sem = sqrt(varStat / samples);
    
    // standard deviation of autocorrelation time
    //    seAct = (2.0 * sqrt(2.0 * (2.0 * (double) (maxLag + 1)) / samples) * (varStat / gammaStat[0]) * stepSize);
    
}

/**
 * Analyze trace
 *
 * @param begin     begin index for analysis
 * @param end       end index for analysis
 *
 * @attention This method assumes that the mean was either made invalid before execution or is calculated apropriately for this burnin.
 */
void TraceAnalysisCategorical::analyseCorrelation(const std::vector<bool>& values, int begin, int end) {
    // if we have not yet calculated the mean, do this now
    if (mean == RbConstants::Double::max) {
        analyseMean(values,burnin);
    }
    
    int samples = end - begin;
   
    
    
    // standard error of mean
//    sem = sqrt(varStat / samples);
    
    // auto correlation time
//    act = varStat / gammaStat[0];
    
    // effective sample size
//    ess = samples / act;
    
    // standard deviation of autocorrelation time
    //    seAct = (2.0 * sqrt(2.0 * (2.0 * (double) (maxLag + 1)) / samples) * (varStat / gammaStat[0]) * stepSize);
    
//    delete[] gammaStat;
}

//void TraceAnalysisCategorical::invalidateTraceStatistics() {
    
    // initialize data to invalid values
//    act                 = RbConstants::Double::max;
//    ess                 = RbConstants::Double::max;
//    mean                = RbConstants::Double::max;
//    sem                 = RbConstants::Double::max;
//}


/**
 * @return the mean
 */
double TraceAnalysisCategorical::getMean() {
    if (mean == RbConstants::Double::max) {
        // throw an error that the ACT needs recalculation
        //analyseMean(values);
    }
    
    return mean;
}

/**
 * @return the standard error of the mean
 */
double TraceAnalysisCategorical::getStdErrorOfMean() {
    if (sem == RbConstants::Double::max) {
        // throw an error that the ACT needs recalculation
        //analyseCorrelation(values);
    }
    
    return sem;
}

/**
 * @return the autocorrelation time
 */
double TraceAnalysisCategorical::getAct() {
    if (act == RbConstants::Double::max) {
        // throw an error that the ACT needs recalculation
        //analyseCorrelation(values);
    }
    
    return act;
}

/**
 * @return the eSS
 */
double TraceAnalysisCategorical::getEss() {
    if (ess == RbConstants::Double::max) {
        // throw an error that the ACT needs recalculation
        //        analyseCorrelation(values);
    }
    
    return ess;
}

