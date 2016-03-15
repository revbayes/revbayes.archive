#include "RbException.h"
#include "TimeReversibleRateMatrix.h"

using namespace RevBayesCore;

TimeReversibleRateMatrix::TimeReversibleRateMatrix(size_t n) : AbstractRateMatrix(n),
    exchangeabilityRates( std::vector<double>(numStates*(numStates-1)/2,1.0) ),
    stationaryFreqs( std::vector<double>(numStates,1.0/n) )
{
    
}



TimeReversibleRateMatrix::~TimeReversibleRateMatrix(void)
{
    // nothing to do
}


/**
 * Assign the value of m to this instance. This function is our mechanism to call the assignment operator.
 *
 *
 */
TimeReversibleRateMatrix& TimeReversibleRateMatrix::assign(const Assignable &m)
{
    
    const TimeReversibleRateMatrix *rm = dynamic_cast<const TimeReversibleRateMatrix*>(&m);
    if ( rm != NULL )
    {
        return operator=(*rm);
    }
    else
    {
        throw RbException("Could not assign rate matrix.");
    }
}

/** Calculate the average rate for the rate matrix */
double TimeReversibleRateMatrix::averageRate(void) const
{
    
    double ave = 0.0;
    for (size_t i=0; i<numStates; i++)
    {
        ave += -stationaryFreqs[i] * (*theRateMatrix)[i][i];
    }
    return ave;
}


void TimeReversibleRateMatrix::computeOffDiagonal( void )
{
    
    MatrixReal& m = *theRateMatrix;
    
    // set the off-diagonal portions of the rate matrix
    for (size_t i=0, k=0; i<numStates; i++) 
    {
        for (size_t j=i+1; j<numStates; j++) 
        {
            m[i][j] = exchangeabilityRates[k] * stationaryFreqs[j];
            m[j][i] = exchangeabilityRates[k] * stationaryFreqs[i];
            k++;
        }
    }
    
    // set flags
    needsUpdate = true;
}


/** Set the exchangeability rates directly. We assume that we know
 what the exchangeability rates are when this function is called. */
void TimeReversibleRateMatrix::setExchangeabilityRates(const std::vector<double>& er)
{
    
    exchangeabilityRates = er;
    
    // set flags
    needsUpdate = true;
}


/** Set the stationary frequencies directly. We assume that we know
 what the stationary frequencies are when this function is called. */
void TimeReversibleRateMatrix::setStationaryFrequencies(const std::vector<double>& f)
{
    
    stationaryFreqs = f;
    
    // set flags
    needsUpdate = true;
}



std::vector<double> TimeReversibleRateMatrix::getStationaryFrequencies( void ) const
{

    return stationaryFreqs;
}

const std::vector<double>& TimeReversibleRateMatrix::getExchangeabilityRates( void ) const
{
    return exchangeabilityRates;
}
