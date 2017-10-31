#include "TraceNumeric.h"
#include "DistributionNormal.h"
#include "RbConstants.h"
#include "RbMathLogic.h"

#include "GewekeTest.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "StationarityTest.h"

#include <cmath>
#include <math.h>

using namespace RevBayesCore;
using namespace std;

#define MAX_LAG 1000

/**
 * 
 */
TraceNumeric::TraceNumeric() :
    stats_dirty( true )
{
    
}


/** Clone function */
TraceNumeric* TraceNumeric::clone() const
{

    return new TraceNumeric(*this);
}


void TraceNumeric::computeStatistics( void )
{

    computeCorrelation();


    // test stationarity within chain
    size_t nBlocks = 10;
    StationarityTest testS = StationarityTest(nBlocks, 0.01);
    passedStationarityTest = testS.assessConvergence(*this);

    // Geweke's test for convergence within a chain
    GewekeTest testG = GewekeTest(0.01);
    passedGewekeTest = testG.assessConvergence(*this);

}


void TraceNumeric::computeMean() const
{
    if( isDirty() == false ) return;

    double m = 0;
    size_t size = values.size();
    for (size_t i=burnin; i<size; i++)
    {
        m += values.at(i);
    }
    
    mean = m/double(size-burnin);

    dirty = false;

    stats_dirty = true;
}


void TraceNumeric::computeMean(size_t begin, size_t end) const
{
    double m = 0;
    for (size_t i=begin; i<end; i++)
    {
        m += values.at(i);
    }
    
    mean = m/(end-begin);

    dirty = true;
}


/**
 * Analyze trace
 *
 * @param burnin   the number of samples to discard
 *
 * @attention This method assumes that the mean was either made invalid before execution or is calculated appropriately for this burnin.
 */
void TraceNumeric::computeCorrelation() const
{
    // if we have not yet calculated the mean, do this now
    
    computeMean();

    if( stats_dirty == false ) return;

    size_t samples = values.size() - burnin;
    size_t maxLag = (samples - 1 < MAX_LAG ? samples - 1 : MAX_LAG);
    
    double* gammaStat = new double[maxLag];
    // setting values to 0
    for (size_t i=0; i<maxLag; i++)
    {
        gammaStat[i] = 0;
    }
    double varStat = 0.0;
    
    for (size_t lag = 0; lag < maxLag; lag++) {
        for (size_t j = 0; j < samples - lag; j++) {
            double del1 = values.at(burnin + j) - mean;
            double del2 = values.at(burnin + j + lag) - mean;
            gammaStat[lag] += (del1 * del2);
        }
        
        gammaStat[lag] /= ((double) (samples - lag));
        
        if (lag == 0) {
            varStat = gammaStat[0];
        } else if (lag % 2 == 0) {
            // fancy stopping criterion :)
            if (gammaStat[lag - 1] + gammaStat[lag] > 0) {
                varStat += 2.0 * (gammaStat[lag - 1] + gammaStat[lag]);
            }
            // stop
            else
                maxLag = lag;
        }
    }
    
    // standard error of mean
    sem = sqrt(varStat / samples);
    
    // auto correlation time
    act = varStat / gammaStat[0];
    
    // effective sample size
    ess = samples / act;
    
    stats_dirty = false;

    delete[] gammaStat;
}

/**
 * Analyze trace
 *
 * @param begin     begin index for analysis
 * @param end       end index for analysis
 *
 * @attention This method assumes that the mean was either made invalid before execution or is calculated apropriately for this burnin.
 */
void TraceNumeric::computeCorrelation(size_t begin, size_t end) const {

    // if we have not yet calculated the mean, do this now
    computeMean(begin,end);
    
    size_t samples = end - begin;
    size_t maxLag = (samples - 1 < MAX_LAG ? samples - 1 : MAX_LAG);
    
    double* gammaStat = new double[maxLag];
    // setting values to 0
    for (size_t i=0; i<maxLag; i++) {
        gammaStat[i] = 0;
    }
    double varStat = 0.0;
    
    for (size_t lag = 0; lag < maxLag; lag++) {
        for (size_t j = 0; j < samples - lag; j++) {
            double del1 = values.at(begin + j) - mean;
            double del2 = values.at(begin + j + lag) - mean;
            gammaStat[lag] += (del1 * del2);
        }
        
        gammaStat[lag] /= ((double) (samples - lag));
        
        if (lag == 0) {
            varStat = gammaStat[0];
        } else if (lag % 2 == 0) {
            // fancy stopping criterion :)
            if (gammaStat[lag - 1] + gammaStat[lag] > 0) {
                varStat += 2.0 * (gammaStat[lag - 1] + gammaStat[lag]);
            }
            // stop
            else
                maxLag = lag;
        }
    }
    
    // standard error of mean
    sem = sqrt(varStat / samples);
    
    // auto correlation time
    act = varStat / gammaStat[0];
    
    // effective sample size
    ess = samples / act;
    
    delete[] gammaStat;
}


/**
 * @return the autocorrelation time
 */
double TraceNumeric::getAct() const
{
    //computeCorrelation();
    
    return act;
}


/**
 * @return the eSS
 */
double TraceNumeric::getEss() const
{
    //computeCorrelation();
    
    return ess;
}


/**
 * @return the mean
 */
double TraceNumeric::getMean() const
{
    //computeMean();
    
    return mean;
}


/**
 * @return the standard error of the mean
 */
double TraceNumeric::getSem() const
{
    //computeCorrelation();
    
    return sem;
}

