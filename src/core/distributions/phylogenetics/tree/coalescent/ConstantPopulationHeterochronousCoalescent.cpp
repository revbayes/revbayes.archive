#include "Clade.h"
#include "ConstantPopulationHeterochronousCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantPopulationHeterochronousCoalescent::ConstantPopulationHeterochronousCoalescent(const TypedDagNode<double> *N, const std::vector<Taxon> &tn, const std::vector<Clade> &c) :
    AbstractCoalescent( tn, c ),
    Ne( N )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( Ne );
    
    simulateHeterochronousTree();
}



ConstantPopulationHeterochronousCoalescent::~ConstantPopulationHeterochronousCoalescent()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantPopulationHeterochronousCoalescent* ConstantPopulationHeterochronousCoalescent::clone( void ) const
{
    
    return new ConstantPopulationHeterochronousCoalescent( *this );
}


/**
 * Compute the log-transformed probability of the current times under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantPopulationHeterochronousCoalescent::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    double theta = Ne->getValue();
    
    // retrieve the coalescence times
    std::vector<double> ages;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i)
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double a = n.getAge();
        ages.push_back(a);
    }
    // sort the vector of coalescence times in ascending order
    std::sort(ages.begin(), ages.end());
    
    // retrieve the times of any serially sampled tips
    std::vector<double> serialTimes;
    size_t numTaxaAtPresent = 0;
    for (size_t i = 0; i < value->getNumberOfTips(); ++i)
    {
//        const TopologyNode& n = value->getTipNode( i );
//        double a = n.getAge();
//        if ( a > 0.0 ) {
//            serialTimes.push_back(a);
//        } else {
//            ++numTaxaAtPresent;
//        }
        double a = taxa[i].getAge();
        if ( a > 0.0 ) {
            serialTimes.push_back(a);
        } else {
            ++numTaxaAtPresent;
        }
    }
    
    std::vector<double> combinedEventTimes;
    std::vector<double> combinedEventTypes;

    if (numTaxaAtPresent < num_taxa) {

        // sort the vector of serial sampling times in ascending order
        std::sort(serialTimes.begin(), serialTimes.end());
        
        size_t atAge = 0;
        size_t atSerialTime = 0;
        double nextAge = ages[atAge];
        double nextSerialTime = serialTimes[atSerialTime];
        
        // create master list of event times and types
        // events are either a sample (lineage size up), coalescence (lineage size down), or theta changepoint (lineage size constant)
        do
        {
            nextAge = ages[atAge];
            if (nextSerialTime <= nextAge) {
                // serial sample
                combinedEventTimes.push_back(nextSerialTime);
                combinedEventTypes.push_back(1.0);
                ++atSerialTime;
                if (atSerialTime < serialTimes.size()) {
                    nextSerialTime = serialTimes[atSerialTime];
                } else {
                    nextSerialTime = RbConstants::Double::inf;
                }
            } else {
                // coalescence
                combinedEventTimes.push_back(nextAge);
                combinedEventTypes.push_back(-1.0);
                ++atAge;
            }
        } while (atAge < ages.size());
        
    } else {
        combinedEventTimes = ages;
        combinedEventTypes = std::vector<double>(ages.size(),-1.0);
    }
    
    
    size_t currentInterval = 0;
    size_t j = numTaxaAtPresent;
    double windowStart = 0.0;
    
    for (size_t i = 0; i < combinedEventTimes.size(); ++i)
    {
        double nPairs = j * (j-1) / 2.0;
        
        double deltaAge = combinedEventTimes[i] - windowStart;
        
        if (combinedEventTypes[i] == 1.0) {
            // sampled ancestor
            lnProbTimes -= nPairs * deltaAge / theta ;
            ++j;
        } else {
            // coalescence
            lnProbTimes += log( 1.0 / theta ) - nPairs * deltaAge / theta;
            --j;
        }
        
        windowStart = combinedEventTimes[i];
    }
    
    return lnProbTimes;
    
}

/**
 * Simulate new coalescent times.
 *
 * \param[in]    n      The number of coalescent events to simulate.
 *
 * \return    A vector of the simulated coalescent times.
 */
std::vector<double> ConstantPopulationHeterochronousCoalescent::simulateCoalescentAges( size_t n ) const
{
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // retrieve the times of any serially sampled tips
    std::vector<double> serialTimes;
    size_t numTaxaAtPresent = 0;
    for (size_t i = 0; i < num_taxa; ++i)
    {
        double a = taxa[i].getAge();
        if ( a > 0.0 ) {
            serialTimes.push_back(a);
        } else {
            ++numTaxaAtPresent;
        }
    }
    
    size_t atSerialTime = 0;
    if (numTaxaAtPresent == num_taxa) {
        serialTimes.push_back(RbConstants::Double::inf);
    } else {
        std::sort(serialTimes.begin(), serialTimes.end());
    }
    
    // now simulate the ages
    // we use rejection sampling to guarantee that at least k coalescent events occur after the kth-to-last serial sample
    
    // allocate the vector for the times
    std::vector<double> coalescentTimes = std::vector<double>(n,0.0);
    
    // for rejection sampling
    bool timesAreValid = false;
    
    do
    {
        size_t j = numTaxaAtPresent;
        double theta = Ne->getValue();
        // draw a time for each speciation event condition on the time of the process
        for (size_t i = 0; i < n; ++i)
        {
            double prevCoalescentTime = 0.0;
            if ( i > 0 )
            {
                prevCoalescentTime = coalescentTimes[i-1];
            }
            
            double simAge = prevCoalescentTime;
            bool valid = false;
            do
            {
                double nPairs = j * (j-1) / 2.0;
                double lambda = nPairs / theta;
                double u = RbStatistics::Exponential::rv( lambda, *rng);
                simAge += u;
                valid = simAge < serialTimes[atSerialTime];
                if ( !valid )
                {
                    simAge = serialTimes[atSerialTime];
                    ++atSerialTime;
                    ++j;
                }
                
            } while ( !valid );
            
            coalescentTimes[i] = simAge;
            --j;
        }
        
        // Make sure we have at least k coalescent events after the kth-to-last serial sample
        // \todo this is probably not the best way to do this
        if ( serialTimes[0] == RbConstants::Double::inf )
        {
            // No serial samples, no problem
            timesAreValid = true;
        } else
        {
            size_t nInvalid = 0;
            for (size_t i = serialTimes.size() - 1; i > -1; --i)
            {
                // For serial sample i out of m, simulated age n - (m - i) must be older
                if ( coalescentTimes[n - serialTimes.size() + i] < serialTimes[i] )
                {
                    ++nInvalid;
                }
            }
            timesAreValid = nInvalid < 1;
        }
        
    } while ( !timesAreValid );
    
    return coalescentTimes;
}

/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void ConstantPopulationHeterochronousCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == Ne)
    {
        Ne = static_cast<const TypedDagNode<double>* >( newP );
    }
}
