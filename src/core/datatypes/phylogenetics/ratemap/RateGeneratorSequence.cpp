#include "MatrixReal.h"
#include "RateGeneratorSequence.h"
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
RateGeneratorSequence::RateGeneratorSequence(size_t ns, size_t nc)
{
    
    numStates            = ns;
    numCharacters        = nc;
    
    needsUpdate          = true;
    
}


/** Destructor */
RateGeneratorSequence::~RateGeneratorSequence(void)
{
    
    
}

void RateGeneratorSequence::calculateTransitionProbabilities(TransitionProbabilityMatrix& P, double age) const
{
    calculateTransitionProbabilities(age, 0.0, 1.0, P);
}



size_t RateGeneratorSequence::getNumberOfStates( void ) const
{
    return numStates;
}

size_t RateGeneratorSequence::getNumberOfCharacters( void ) const
{
    return numCharacters;
}

size_t RateGeneratorSequence::size( void ) const
{
    return numStates;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateGeneratorSequence& x)
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}


void RateGeneratorSequence::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...
        
        needsUpdate = false;
    }
}


//double RateGeneratorSequence::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age) const
//{
//    size_t fromState = from[ to->getSiteIndex() ]->getState();
//    size_t toState = to->getState();
//    
//    double rate = 0.0;
//    const RateGenerator* rm;
//    if (branchHeterogeneousRateMatrices)
//    {
//        rm = &heterogeneousRateMatrices[node.getIndex()];
//    }
//    else
//    {
//        rm = homogeneousRateMatrix;
//    }
//    
//    rate = rm->getRate(fromState, toState, age, 1.0);
//    
//    if (branchHeterogeneousClockRates)
//    {
//        rate *= heterogeneousClockRates[node.getIndex()];
//    }
//    else
//    {
//        rate *= homogeneousClockRate;
//    }
//    
//    return rate;
//    
//}
//
//double RateGeneratorSequence::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
//{
//    
//    double rate = 0.0;
//    const RateGenerator* rm;
//    if (branchHeterogeneousRateMatrices)
//    {
//        rm = &heterogeneousRateMatrices[node.getIndex()];
//    }
//    else
//    {
//        rm = homogeneousRateMatrix;
//    }
//    
//    rate = rm->getRate(from->getState(), to->getState(), age, 1.0);
//    
//    if (branchHeterogeneousClockRates)
//    {
//        rate *= heterogeneousClockRates[node.getIndex()];
//    }
//    else
//    {
//        rate *= homogeneousClockRate;
//    }
//    
//    return rate;
//}
//
//double RateGeneratorSequence::getSiteRate(const TopologyNode& node, size_t from, size_t to, size_t charIdx, double age) const
//{
//    
//    double rate = 0.0;
//    const RateGenerator* rm;
//    if (branchHeterogeneousRateMatrices)
//    {
//        rm = &heterogeneousRateMatrices[node.getIndex()];
//    }
//    else
//    {
//        rm = homogeneousRateMatrix;
//    }
//    
//    rate = rm->getRate(from, to, age, 1.0);
//    
//    if (branchHeterogeneousClockRates)
//    {
//        rate *= heterogeneousClockRates[node.getIndex()];
//    }
//    else
//    {
//        rate *= homogeneousClockRate;
//    }
//    
//    return rate;
//}


