
#include "Clade.h"
#include "PiecewiseConstantCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

PiecewiseConstantCoalescent::PiecewiseConstantCoalescent(const TypedDagNode<RbVector<double> > *N, const TypedDagNode<RbVector<double> > *i, const std::vector<Taxon> &tn, const std::vector<Clade> &c) :
    AbstractCoalescent( tn, c ),
    Nes( N ),
    intervalStarts( i )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( Nes );
    addParameter( intervalStarts );
    
    simulateTree();
}



PiecewiseConstantCoalescent::~PiecewiseConstantCoalescent()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
PiecewiseConstantCoalescent* PiecewiseConstantCoalescent::clone( void ) const
{
    
    return new PiecewiseConstantCoalescent( *this );
}


/**
 * Compute the log-transformed probability of the current times under the current parameter values.
 *
 * \return    The log-probability density.
 */
double PiecewiseConstantCoalescent::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    
    // retrieved the speciation times
    std::vector<double> ages;
    for (size_t i = 0; i < value->getNumberOfInteriorNodes()+1; ++i)
    {
        const TopologyNode& n = value->getInteriorNode( i );
        double a = n.getAge();
        ages.push_back(a);
    }
    // sort the vector of times in ascending order
    std::sort(ages.begin(), ages.end());
    
    const RbVector<double> &popSizes  = Nes->getValue();
    const RbVector<double> &intervals = intervalStarts->getValue();
    size_t currentInterval = 0;
    
    for (size_t i = 0; i < ages.size(); ++i)
    {
        size_t j = numTaxa - i;
        double theta = 0.0;
        double nPairs = j * (j-1) / 2.0;
        
        double prevCoalescentTime = 0.0;
        if ( i > 0 )
        {
            prevCoalescentTime = ages[i-1];
        }
        
        double deltaAge = ages[i];
        bool valid = false;
        do
        {
            theta = popSizes[currentInterval];
            double age = ages[i];
            double max = (prevCoalescentTime < intervals[currentInterval]) ? intervals[currentInterval] : prevCoalescentTime;
            
            deltaAge = age - max;
            valid = currentInterval >= intervals.size() || age < intervals[currentInterval];
            if ( !valid )
            {
                ++currentInterval;
                
                lnProbTimes -= nPairs * deltaAge / theta ;
            }
            
        } while ( !valid );
        
        lnProbTimes += log( nPairs / theta ) - nPairs * deltaAge / theta ;
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
std::vector<double> PiecewiseConstantCoalescent::simulateCoalescentTime( size_t n ) const
{
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // now simulate the ages
    
    // allocate the vector for the times
    std::vector<double> coalescentTimes = std::vector<double>(n,0.0);
    
    
    const RbVector<double> &popSizes  = Nes->getValue();
    const RbVector<double> &intervals = intervalStarts->getValue();
    size_t currentInterval = 0;
    // draw a time for each speciation event condition on the time of the process
    for (size_t i = 0; i < numTaxa; ++i)
    {
        double prevCoalescentTime = 0.0;
        if ( i > 0 )
        {
            prevCoalescentTime = coalescentTimes[i-1];
        }
        
        size_t j = numTaxa - i;
        double nPairs = j * (j-1) / 2.0;
        
        double simAge = 0.0;
        bool valid = false;
        do
        {
            double theta = 1.0 / (2.0*popSizes[currentInterval]);
            double lambda = nPairs * theta / 2.0;
            double u = RbStatistics::Exponential::rv( lambda, *rng);
            simAge = prevCoalescentTime + u;
            valid = currentInterval >= intervals.size() || simAge < intervals[currentInterval];
            if ( !valid )
            {
                ++currentInterval;
            }
            
        } while ( !valid );
        
        coalescentTimes[i] = simAge;
    }
    
    return coalescentTimes;
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == Nes)
    {
        Nes = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    else if (oldP == intervalStarts)
    {
        intervalStarts = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
}
