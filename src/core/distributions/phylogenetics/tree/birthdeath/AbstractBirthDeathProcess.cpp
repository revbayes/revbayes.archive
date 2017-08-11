#include "Clade.h"
#include "AbstractBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 *
 * \param[in]    o         Origin or time of the process.
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
AbstractBirthDeathProcess::AbstractBirthDeathProcess(const TypedDagNode<double> *ra, const std::string &cdt, const std::vector<Taxon> &tn) : AbstractRootedTreeDistribution( ra, tn ),
    condition( cdt )
{
    
}


AbstractBirthDeathProcess::~AbstractBirthDeathProcess(void)
{
    
}



/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double AbstractBirthDeathProcess::computeLnProbabilityDivergenceTimes( void ) const
{
    // prepare the probability computation
    prepareProbComputation();
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // present time
    double present_time = value->getRoot().getAge();
    
    // what do we condition on?
    // did we condition on survival?
    if ( condition == "survival" )
    {
        lnProbTimes = - log( pSurvival(0, present_time) );
        
        // we started at the root thus we square the survival prob
        lnProbTimes *= 2.0;
        
    }
    else if ( condition == "nTaxa" )
    {
        lnProbTimes = -lnProbNumTaxa( value->getNumberOfTips(), 0, present_time, true );
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityTimes();
    
    return lnProbTimes;
}


double AbstractBirthDeathProcess::lnProbTreeShape(void) const
{
    // the birth death density is derived for a (ranked) unlabeled oriented tree
    // so we convert to a (ranked) labeled non-oriented tree probability by multiplying by 2^{n-1} / n!
    // see Gernhard 2008

    return (value->getNumberOfTips() - 1) * RbConstants::LN2 - RbMath::lnFactorial(value->getNumberOfTips());
}


/**
 * Prepare the probability computation. Here we can pre-calculate some values for more
 * efficient probability calculation. The derived classes may want to do something ...
 */
void AbstractBirthDeathProcess::prepareProbComputation( void ) const
{
    
}


std::vector<double>* AbstractBirthDeathProcess::simulateDivergenceTimes(size_t n, double origin, double end, double present) const
{

    std::vector<double>* times = new std::vector<double>(n, 0.0);
    
    for (size_t i = 0; i < n; ++i)
    {
        double t = 0.0;
        
        do {
            t = simulateDivergenceTime(origin, present);
        } while ( t > end );
        
        (*times)[i] = t;
    }
    
    // finally sort the times
    std::sort(times->begin(), times->end());
    
    return times;
}

