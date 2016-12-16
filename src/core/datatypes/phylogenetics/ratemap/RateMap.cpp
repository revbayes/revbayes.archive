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
    
    homogeneous_clock_rate = 1.0;
    homogeneous_rate_matrix = new RateMatrix_JC(ns);
    root_frequencies = std::vector<double>(ns,1.0/ns);
    
    needs_update          = true;
    branch_heterogeneous_clock_rates = false;
    branchHeterogeneousRateMatrices = false;
    
}



/** Copy constructor */
RateMap::RateMap(const RateMap& m)
{
    
    homogeneous_clock_rate            = m.homogeneous_clock_rate;
    heterogeneous_clock_rates         = m.heterogeneous_clock_rates;
    homogeneous_rate_matrix           = m.homogeneous_rate_matrix->clone();
    heterogeneous_rate_matrices       = m.heterogeneous_rate_matrices;
    root_frequencies                 = m.root_frequencies;
    
    num_states                       = m.num_states;
    numCharacters                   = m.numCharacters;
    needs_update                     = m.needs_update;
    branchHeterogeneousRateMatrices = m.branchHeterogeneousRateMatrices;
    branch_heterogeneous_clock_rates   = m.branch_heterogeneous_clock_rates;
    
}


/** Destructor */
RateMap::~RateMap(void) {
    
    delete homogeneous_rate_matrix;
    
}


RateMap& RateMap::operator=(const RateMap &r)
{
    
    if (this != &r) {
        
        num_states           = r.num_states;
        numCharacters       = r.numCharacters;
        needs_update         = true;
        
        delete homogeneous_rate_matrix;
        
        homogeneous_rate_matrix = r.homogeneous_rate_matrix->clone();
        
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
    return homogeneous_rate_matrix;
}

void RateMap::setHomogeneousRateMatrix(const RateGenerator* r)
{
    branchHeterogeneousRateMatrices = false;
    
    if (r != homogeneous_rate_matrix)
        delete homogeneous_rate_matrix;
    
    homogeneous_rate_matrix = r->clone();
}

const RbVector<RateGenerator>& RateMap::getHeterogeneousRateMatrices(void) const
{
    return heterogeneous_rate_matrices;
}

void RateMap::setHeterogeneousRateMatrices(const RbVector<RateGenerator>& r)
{
    branchHeterogeneousRateMatrices = true;
    heterogeneous_rate_matrices = r;
}

double RateMap::getHomogeneousClockRate(void) const
{
    return homogeneous_clock_rate;
}

void RateMap::setHomogeneousClockRate(double r)
{
    branch_heterogeneous_clock_rates = false;
    homogeneous_clock_rate = r;
}

void RateMap::setRootFrequencies(const RevBayesCore::RbVector<double>& r)
{
    root_frequencies = r;
}

const std::vector<double>& RateMap::getRootFrequencies(void) const
{
    return root_frequencies;
}

const std::vector<double>& RateMap::getHeterogeneousClockRates(void) const
{
    return heterogeneous_clock_rates;
}

void RateMap::setHeterogeneousClockRates(const std::vector<double> &r)
{
    branch_heterogeneous_clock_rates = true;
    heterogeneous_clock_rates = r;
}


void RateMap::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P) const
{
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    else
        rm = homogeneous_rate_matrix;
    
    double t = node.getBranchLength();
    if (branch_heterogeneous_clock_rates)
        t *= heterogeneous_clock_rates[node.getIndex()];
    else
        t *= homogeneous_clock_rate;
    
    rm->calculateTransitionProbabilities(t, P);
}

void RateMap::calculateTransitionProbabilities(const TopologyNode& node, TransitionProbabilityMatrix &P, size_t charIdx) const
{
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    else
        rm = homogeneous_rate_matrix;
    
    double t = node.getBranchLength();
    if (branch_heterogeneous_clock_rates)
        t *= heterogeneous_clock_rates[node.getIndex()];
    else
        t *= homogeneous_clock_rate;
    
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
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    }
    else
    {
        rm = homogeneous_rate_matrix;
    }
    
    rate = rm->getRate(fromState, toState, age, 1.0);
    
    if (branch_heterogeneous_clock_rates)
    {
        rate *= heterogeneous_clock_rates[node.getIndex()];
    }
    else
    {
        rate *= homogeneous_clock_rate;
    }
    
    return rate;
    
}

double RateMap::getSiteRate(const TopologyNode& node, CharacterEvent* from, CharacterEvent* to, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
    {
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    }
    else
    {
        rm = homogeneous_rate_matrix;
    }
    
    rate = rm->getRate(from->getState(), to->getState(), age, 1.0);
    
    if (branch_heterogeneous_clock_rates)
    {
        rate *= heterogeneous_clock_rates[node.getIndex()];
    }
    else
    {
        rate *= homogeneous_clock_rate;
    }
    
    return rate;
}

double RateMap::getSiteRate(const TopologyNode& node, size_t from, size_t to, size_t charIdx, double age) const
{
    
    double rate = 0.0;
    const RateGenerator* rm;
    if (branchHeterogeneousRateMatrices)
    {
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    }
    else
    {
        rm = homogeneous_rate_matrix;
    }
    
    rate = rm->getRate(from, to, age, 1.0);
    
    if (branch_heterogeneous_clock_rates)
    {
        rate *= heterogeneous_clock_rates[node.getIndex()];
    }
    else
    {
        rate *= homogeneous_clock_rate;
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
        rm = &heterogeneous_rate_matrices[node.getIndex()];
    else
        rm = homogeneous_rate_matrix;
    
    // get the rate of leaving the sequence-state
    double sum = 0.0;
    for (size_t i = 0; i < num_states; i++)
    {
//        std::cout << i << " "<< counts[i] << "\n";
        sum += -rm->getRate(i, i, age, 1.0) * counts[i];
    }
    
    // apply rate for branch
    if (branch_heterogeneous_clock_rates)
    {
        sum *= heterogeneous_clock_rates[node.getIndex()];
    }
    else
    {
        sum *= homogeneous_clock_rate;
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

