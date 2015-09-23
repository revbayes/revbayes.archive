#include "Clade.h"
#include "BirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"

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
 * \param[in]    ra        Age or the root (=time of the process).
 * \param[in]    r         Sampling probability of a species at present.
 * \param[in]    ss        The sampling strategy (uniform/diversified).
 * \param[in]    cdt       The condition of the process (time/survival/nTaxa)
 * \param[in]    nTaxa     Number of taxa (used for initialization during simulation).
 * \param[in]    tn        Taxon names used during initialization.
 * \param[in]    c         Clade constraints.
 */
BirthDeathProcess::BirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const TypedDagNode<double> *rh,
                                     const std::string& ss, const std::string &cdt,
                                     const std::vector<Taxon> &tn, const std::vector<Clade> &c) : AbstractBirthDeathProcess( o, ra, cdt, tn, c ),
        rho( rh ),
        samplingStrategy( ss )
{
    
    addParameter( rho );
        
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return   The log-transformed probability density.
 */
double BirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // retrieved the speciation times
    std::vector<double>* times = divergenceTimesSinceOrigin();
    
    double samplingProbability = 1.0;
    if ( samplingStrategy == "uniform" ) 
    {
        samplingProbability = rho->getValue();
    }
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = 0.0;

    // test that the time of the process is larger or equal to the present time
    if ( startsAtRoot == false )
    {
        double org = origin->getValue();
        presentTime = org;
        
    }
    else
    {
        presentTime = ra;
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += lnP1(0,presentTime,samplingProbability);
    
    // add the survival of a second species if we condition on the MRCA
    size_t numInitialSpecies = 1;
    
    // if we started at the root then we square the survival prob
    if ( startsAtRoot == true )
    {
        ++numInitialSpecies;
        lnProbTimes *= 2.0;
    }
    
    for (size_t i = (numInitialSpecies-1); i < numTaxa-1; ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            delete times;
            return RbConstants::Double::nan;
        }
         
        lnProbTimes += lnSpeciationRate((*times)[i]) + lnP1((*times)[i],presentTime,samplingProbability);
    }
    
    // if we assume diversified sampling, we need to multiply with the probability that all missing species happened after the last speciation event
    if ( samplingStrategy == "diversified" ) 
    {
        // We use equation (5) of Hoehna et al. "Inferring Speciation and Extinction Rates under Different Sampling Schemes"
        double lastEvent = (*times)[times->size()-2];
        
        double p_0_T = 1.0 - pSurvival(0,presentTime,1.0) * exp( rateIntegral(0,presentTime) );
        double p_0_t = (1.0 - pSurvival(lastEvent,presentTime,1.0) * exp( rateIntegral(lastEvent,presentTime) ));
        double F_t = p_0_t / p_0_T;
        
        // get an estimate of the actual number of taxa
        double m = round(numTaxa / rho->getValue());     
        lnProbTimes += (m-numTaxa) * log(F_t) + log(RbMath::choose(m,numTaxa));
    }
    
    delete times;
    
    return lnProbTimes;
    
}


double BirthDeathProcess::lnP1(double t, double T, double r) const
{
    
    // get the survival probability
    double a = log( pSurvival(t,T,r) );
    double b = rateIntegral(t, T) - log(r);
    
    // compute the probability of observing/sampling exactly one lineage
    double p = 2.0 * a + b;
    
    return p;
    
}


/**
 * Compute the probabililty of survival (no extinction) of the process including uniform taxon sampling at the present time.
 * The probability of survival is given by
 * [1 + int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds ) ]^{-1}
 * and can be simplified to
 * [1 + int_{t_low}^{t_high} ( mu'(s) exp(rate'(t,s)) ds ) - (r-1)/r*exp(rate'(t_low,t_high)) ]^{-1}
 * where mu' and rate' are the diversification rate function without incomplete taxon sampling.
 * Therefore we can just call pSurvival without incomplete taxon sampling that will be computed in the derived classes,
 * and add the sampling here so that sampling will be available for all models :)
 * For more information please read Hoehna, S. 2014. The time-dependent reconstructed evolutionary process with a key-role for mass-extinction events.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 * \param[in]    r          Sampling probability.
 *
 * \return The probability of survival of the process.
 */
double BirthDeathProcess::pSurvival(double start, double end, double r) const
{
    double rate = rateIntegral(start, end);
    double ps = 1.0 / pSurvival(start, end);
    
    return 1.0 / (ps - (r-1.0)/r * exp(rate) );
}



std::vector<double>* BirthDeathProcess::simSpeciations(size_t n, double origin) const
{
    
    if ( samplingStrategy == "uniform" )
    {
        return simSpeciations(n, origin, rho->getValue() );
    }
    else
    {
        std::vector<double>* all = simSpeciations(round(n/rho->getValue()), origin, 1.0 );
        all->resize(n);
        return all;
    }
    
}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void BirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == rho ) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }

}
