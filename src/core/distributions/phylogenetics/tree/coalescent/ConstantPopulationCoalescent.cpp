#include "Clade.h"
#include "ConstantPopulationCoalescent.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantPopulationCoalescent::ConstantPopulationCoalescent(const TypedDagNode<double> *N, const std::vector<Taxon> &tn, const std::vector<Clade> &c) :
    AbstractCoalescent( tn, c ),
    Ne( N )
{
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    addParameter( Ne );
    
    simulateTree();
}



ConstantPopulationCoalescent::~ConstantPopulationCoalescent()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantPopulationCoalescent* ConstantPopulationCoalescent::clone( void ) const
{
    
    return new ConstantPopulationCoalescent( *this );
}


/**
 * Compute the log-transformed probability of the current times under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantPopulationCoalescent::computeLnProbabilityTimes( void ) const
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
    
    for (size_t i = 0; i < ages.size(); ++i) 
    {
//        size_t j = num_taxa - i;
//        double theta = 1.0 / (2.0*Ne->getValue());
//        double nPairs = j * (j-1) / 2.0;
//        lnProbTimes += log( nPairs * 2.0 / theta ) - nPairs * 2.0 / theta * ages[i] ;
        
        
        size_t j = num_taxa - i;
        double theta = Ne->getValue();
        double nPairs = j * (j-1) / 2.0;
        
        double prevCoalescentTime = 0.0;
        if ( i > 0 )
        {
            prevCoalescentTime = ages[i-1];
        }
        
        double deltaAge = ages[i] - prevCoalescentTime;

        // CoalescentMCMC
//        lnProbTimes += log( nPairs / theta ) - 2 * nPairs * deltaAge / theta;
        // BEAST:
//        lnProbTimes += log( 1.0 / theta ) - nPairs * deltaAge / theta ;
        // RevBayes"
//        lnProbTimes += log( nPairs / theta ) - nPairs * deltaAge / theta;
        
        
        lnProbTimes += log( 1.0 / theta ) - nPairs * deltaAge / theta;
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
std::vector<double> ConstantPopulationCoalescent::simulateCoalescentAges( size_t n ) const
{
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // now simulate the ages
    
    // allocate the vector for the times
    std::vector<double> coalescentTimes = std::vector<double>(n,0.0);
    
    double theta = 1.0 / (2.0*Ne->getValue());
    // draw a time for each speciation event condition on the time of the process
	for (size_t i = 0; i < n; ++i)
    {
        double prevCoalescentTime = 0.0;
        if ( i > 0 ) 
        {
            prevCoalescentTime = coalescentTimes[i-1];
        }
        
        size_t j = num_taxa - i;
        double nPairs = j * (j-1) / 2.0;
        double lambda = nPairs * theta / 2.0;
        double u = RbStatistics::Exponential::rv( lambda, *rng);
		coalescentTimes[i] = prevCoalescentTime + u;
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
void ConstantPopulationCoalescent::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == Ne)
    {
        Ne = static_cast<const TypedDagNode<double>* >( newP );
    }
}
