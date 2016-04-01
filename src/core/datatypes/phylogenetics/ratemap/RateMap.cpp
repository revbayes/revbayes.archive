#include "MatrixReal.h"
#include "RateMap.h"
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
RateMap::RateMap(size_t ns, size_t nc)
{
    
    num_states            = ns;
    numCharacters        = nc;
    
    homogeneousClockRate = 1.0;
    homogeneousRateMatrix = new RateMatrix_JC(ns);
    rootFrequencies = std::vector<double>(ns,1.0/ns);
    
    needs_update          = true;
    branchHeterogeneousClockRates = false;
    branchHeterogeneousRateMatrices = false;
    
}



/** Copy constructor */
RateMap::RateMap(const RateMap& m)
{
    
    homogeneousClockRate            = m.homogeneousClockRate;
    heterogeneousClockRates         = m.heterogeneousClockRates;
    homogeneousRateMatrix           = m.homogeneousRateMatrix->clone();
    heterogeneousRateMatrices       = m.heterogeneousRateMatrices;
    rootFrequencies                 = m.rootFrequencies;
    
    num_states                       = m.num_states;
    numCharacters                   = m.numCharacters;
    needs_update                     = m.needs_update;
    branchHeterogeneousRateMatrices = m.branchHeterogeneousRateMatrices;
    branchHeterogeneousClockRates   = m.branchHeterogeneousClockRates;
    
}


/** Destructor */
RateMap::~RateMap(void) {
    
    delete homogeneousRateMatrix;
    
}


RateMap& RateMap::operator=(const RateMap &r)
{
    
    if (this != &r) {
        
        num_states           = r.num_states;
        numCharacters       = r.numCharacters;
        needs_update         = true;
        
        delete homogeneousRateMatrix;
        
        homogeneousRateMatrix = r.homogeneousRateMatrix->clone();
        
    }
    
    return *this;
}

size_t RateMap::getNumberOfStates( void ) const
{
    return num_states;
}

size_t RateMap::getNumberOfCharacters( void ) const
{
    return numCharacters;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMap& x)
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}

const RateGenerator* RateMap::getHomogeneousRateMatrix(void) const
{
    return homogeneousRateMatrix;
}

void RateMap::setHomogeneousRateMatrix(const RateGenerator* r)
{
    branchHeterogeneousRateMatrices = false;
    
    if (r != homogeneousRateMatrix)
        delete homogeneousRateMatrix;
    
    homogeneousRateMatrix = r->clone();
}

const RbVector<RateGenerator>& RateMap::getHeterogeneousRateMatrices(void) const
{
    return heterogeneousRateMatrices;
}

void RateMap::setHeterogeneousRateMatrices(const RbVector<RateGenerator>& r)
{
    branchHeterogeneousRateMatrices = true;
    heterogeneousRateMatrices = r;
}

double RateMap::getHomogeneousClockRate(void) const
{
    return homogeneousClockRate;
}

void RateMap::setHomogeneousClockRate(double r)
{
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
}

void RateMap::setRootFrequencies(const RevBayesCore::RbVector<double>& r)
{
    rootFrequencies = r;
}

const std::vector<double>& RateMap::getRootFrequencies(void) const
{
    return rootFrequencies;
}

const std::vector<double>& RateMap::getHeterogeneousClockRates(void) const
{
    return heterogeneousClockRates;
}

void RateMap::setHeterogeneousClockRates(const std::vector<double> &r)
{
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
}


void RateMap::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
        rm = &heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    double t = node.getBranchLength();
    if (branchHeterogeneousClockRates)
        t *= heterogeneousClockRates[node.getIndex()];
    else
        t *= homogeneousClockRate;
    
    rm->calculateTransitionProbabilities(t, P);
}

void RateMap::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P, size_t charIdx) const
{
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
        rm = &heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    double t = node.getBranchLength();
    if (branchHeterogeneousClockRates)
        t *= heterogeneousClockRates[node.getIndex()];
    else
        t *= homogeneousClockRate;
    
    rm->calculateTransitionProbabilities(t, P);
}

RateMap* RateMap::clone(void) const
{
    return new RateMap( *this );
}

double RateMap::getRate(const TopologyNode& node, std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double age) const
{
    size_t fromState = from[ to->getCharacterIndex() ]->getState();
    size_t toState = to->getState();
    
    double rate = 0.0;
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
    {
        rm = &heterogeneousRateMatrices[node.getIndex()];
    }
    else
    {
        rm = homogeneousRateMatrix;
    }
    
    rate = rm->getRate(fromState, toState, age, 1.0);
    
    if (branchHeterogeneousClockRates)
    {
        rate *= heterogeneousClockRates[node.getIndex()];
    }
    else
    {
        rate *= homogeneousClockRate;
    }
    
    return rate;
    
}

double RateMap::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
    {
        rm = &heterogeneousRateMatrices[node.getIndex()];
    }
    else
    {
        rm = homogeneousRateMatrix;
    }
    
    rate = rm->getRate(from->getState(), to->getState(), age, 1.0);
    
    if (branchHeterogeneousClockRates)
    {
        rate *= heterogeneousClockRates[node.getIndex()];
    }
    else
    {
        rate *= homogeneousClockRate;
    }
    
    return rate;
}

double RateMap::getSiteRate(const TopologyNode& node, size_t from, size_t to, size_t charIdx, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
    {
        rm = &heterogeneousRateMatrices[node.getIndex()];
    }
    else
    {
        rm = homogeneousRateMatrix;
    }
    
    rate = rm->getRate(from, to, age, 1.0);
    
    if (branchHeterogeneousClockRates)
    {
        rate *= heterogeneousClockRates[node.getIndex()];
    }
    else
    {
        rate *= homogeneousClockRate;
    }
    
    return rate;
}

double RateMap::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, unsigned* counts, double age) const
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
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
        rm = &heterogeneousRateMatrices[node.getIndex()];
    else
        rm = homogeneousRateMatrix;
    
    // get the rate of leaving the sequence-state
    double sum = 0.0;
    for (size_t i = 0; i < num_states; i++)
    {
//        std::cout << i << " "<< counts[i] << "\n";
        sum += -rm->getRate(i, i, age, 1.0) * counts[i];
    }
    
    // apply rate for branch
    if (branchHeterogeneousClockRates)
    {
        sum *= heterogeneousClockRates[node.getIndex()];
    }
    else
    {
        sum *= homogeneousClockRate;
    }
    
    return sum;
}

double RateMap::getSumOfRates(const TopologyNode& node, std::vector<CharacterEvent*> from, double age) const
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
    
    return getSumOfRates(node, from, counts, age);
}

void RateMap::updateMap(void)
{
    if (needs_update)
    {
        ; // do nothing ...
        
        needs_update = false;
    }
}

