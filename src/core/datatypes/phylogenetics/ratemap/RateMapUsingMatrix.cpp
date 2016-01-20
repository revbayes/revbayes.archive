#include "MatrixReal.h"
#include "RateMapUsingMatrix.h"
#include "RateMatrix.h"
#include "RateMatrix_JC.h"
#include "RbVector.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMapUsingMatrix::RateMapUsingMatrix(size_t ns, size_t nc) : RateMap(ns, nc)
{
    
    rateMatrix = new RateMatrix_JC(ns);
    
}



/** Copy constructor */
RateMapUsingMatrix::RateMapUsingMatrix(const RateMapUsingMatrix& m) : RateMap( m )
{
    
    rateMatrix = m.rateMatrix->clone();
    
}


/** Destructor */
RateMapUsingMatrix::~RateMapUsingMatrix(void)
{
    
    delete rateMatrix;
    
}


RateMapUsingMatrix& RateMapUsingMatrix::operator=(const RateMapUsingMatrix &r)
{
    
    RateMap::operator=( r );
    
    if (this != &r)
    {
        
        delete rateMatrix;
        
        rateMatrix = r.rateMatrix->clone();
        
    }
    
    return *this;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMapUsingMatrix& x)
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}



void RateMapUsingMatrix::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix &P) const
{
    const RateGenerator* rm = rateMatrix;
    
    rm->calculateTransitionProbabilities(startAge, endAge, rate, P);
}


RateMapUsingMatrix* RateMapUsingMatrix::clone(void) const
{
    return new RateMapUsingMatrix( *this );
}

double RateMapUsingMatrix::getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, double rate, unsigned* counts, double age) const
{
    size_t fromState = from[ to->getCharacterIndex() ]->getState();
    size_t toState = to->getState();
    
    const RateGenerator* rm = rateMatrix;
    
    double r = rm->getRate(fromState, toState, age, rate);
    
    return r;
    
}

double RateMapUsingMatrix::getSiteRate(CharacterEvent* from, CharacterEvent* to, double r, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm = rateMatrix;
    
    rate = rm->getRate(from->getState(), to->getState(), age, r);
    
    return rate;
}

double RateMapUsingMatrix::getSiteRate(size_t from, size_t to, size_t charIdx, double r, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm = rateMatrix;
    
    rate = rm->getRate(from, to, age, 1.0);
    
    return rate;
}

double RateMapUsingMatrix::getSumOfRates(std::vector<CharacterEvent*> from, unsigned* counts, double rate, double age) const
{
    
    // get characters in each state
    if (counts == NULL)
    {
        
        // need dynamic allocation
        unsigned tmpCounts[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
        counts = tmpCounts;
        for (size_t i = 0; i < from.size(); i++)
        {
            counts[ from[i]->getState() ] += 1;
        }
        
    }
    
    
    // get rate matrix
    const RateGenerator* rm = rateMatrix;
    
    // get the rate of leaving the sequence-state
    double sum = 0.0;
    for (size_t i = 0; i < numStates; i++)
    {
        //        std::cout << i << " "<< counts[i] << "\n";
        sum += -rm->getRate(i, i, age, 1.0) * counts[i];
    }
    
    // apply rate for branch
    sum *= rate;
    
    return sum;
}

double RateMapUsingMatrix::getSumOfRates(std::vector<CharacterEvent*> from, double rate, double age) const
{
    
    // need dynamic allocation
    unsigned counts[20] = { 0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0  };
    
    for (size_t i = 0; i < from.size(); i++)
    {
        counts[ from[i]->getState() ] += 1;
    }
    
    return getSumOfRates( from, counts, rate, age);
}


void RateMapUsingMatrix::setRateMatrix(const RateGenerator* r)
{
    
    if (r != rateMatrix)
    {
        delete rateMatrix;
        rateMatrix = r->clone();
    }
    
}


void RateMapUsingMatrix::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...
        
        needsUpdate = false;
    }
}

