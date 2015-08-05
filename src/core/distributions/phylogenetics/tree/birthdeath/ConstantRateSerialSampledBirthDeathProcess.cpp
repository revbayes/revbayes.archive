#include "DistributionExponential.h"
#include "ConstantRateSerialSampledBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>


using namespace RevBayesCore;


/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    o              Time of the origin/present/length of the process.
 * \param[in]    s              Speciation rate.
 * \param[in]    e              Extinction rate.
 * \param[in]    p              Extinction sampling rate.
 * \param[in]    r              Sampling probability at present time.
 * \param[in]    tLastSample    time between now and the last obtained sample.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
ConstantRateSerialSampledBirthDeathProcess::ConstantRateSerialSampledBirthDeathProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
                                                                       const TypedDagNode<double> *p, const TypedDagNode<double> *r, double tLastSample, const std::string &cdt, 
                                                                       const std::vector<Taxon> &tn, const std::vector<Clade> &c) : AbstractBirthDeathProcess( o, ra, cdt, tn, c ),
    lambda( s ), 
    mu( e ), 
    psi( p ), 
    rho( r ),
    timeSinceLastSample( tLastSample )
{
    addParameter( lambda );
    addParameter( mu );
    addParameter( psi );
    addParameter( rho );
    
    simulateTree();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
ConstantRateSerialSampledBirthDeathProcess* ConstantRateSerialSampledBirthDeathProcess::clone( void ) const 
{
    
    return new ConstantRateSerialSampledBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantRateSerialSampledBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // get the parameters
    double birth = lambda->getValue();
    double p     = psi->getValue();
    double r     = rho->getValue();
    
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = 0.0;
    
    size_t numInitialLineages = 1;
    
    // test that the time of the process is larger or equal to the present time
    if ( startsAtRoot == false )
    {
        double org = origin->getValue();
        presentTime = org;
        
    }
    else
    {
        presentTime = ra;
        numInitialLineages = 2;
    }
    
    // retrieved the speciation times
    std::vector<double>* agesInternalNodes  = getAgesOfInternalNodesFromMostRecentSample();
    std::vector<double>* agesTips           = getAgesOfTipsFromMostRecentSample();
    
    // multiply the probabilities for the tip ages
    for (size_t i = 0; i < numTaxa; ++i) 
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesTips)[i];
//        if ( t == 0.0 && r > 0.0 )
        if (t < 1e-3 && r > 0.0)
        {
            lnProbTimes += log( 4.0 * r );
        }
        else
        {
            lnProbTimes += log( p ) - log( q( t + timeSinceLastSample) );
        }
        
    }
    
    for (size_t i = 0; i < numTaxa-numInitialLineages; ++i)
    {
        if ( lnProbTimes == RbConstants::Double::nan || 
            lnProbTimes == RbConstants::Double::inf || 
            lnProbTimes == RbConstants::Double::neginf ) 
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesInternalNodes)[i];
        lnProbTimes += log( q(t+timeSinceLastSample) * birth );
    }
    
    lnProbTimes += numInitialLineages * log( q( presentTime ) );
    
    delete agesInternalNodes;
    delete agesTips;
    
    return lnProbTimes;
    
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 * 
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Speciation rate at time t. 
 */
double ConstantRateSerialSampledBirthDeathProcess::pSurvival(double start, double end) const 
{
    
    double t = end;
    // get the parameters
    double birth = lambda->getValue();
    double death = mu->getValue();
    double p     = psi->getValue();
    double r     = rho->getValue();
    
    double a = (birth-death-p);
    double c1 = sqrt( a*a + 4*birth*p );
    double c2 = - (a-2.0*birth*r)/c1;
    
    double oneMinusC2 = 1.0-c2;
    double onePlusC2  = 1.0+c2;
    
    double e = exp(-c1*t);
    
    double p0 = ( birth + death + p + c1 * ( e * oneMinusC2 - onePlusC2 ) / ( e * oneMinusC2 + onePlusC2 ) ) / (2.0 * birth);
    
    return 1.0 - p0;
}



/**
 *
 */
double ConstantRateSerialSampledBirthDeathProcess::q( double t ) const
{
    
    // get the parameters
    double birth = lambda->getValue();
    double death = mu->getValue();
    double p     = psi->getValue();
    double r     = rho->getValue();
    
    double a = (birth-death-p);
    double c1 = sqrt( a*a + 4*birth*p );
    double c2 = - (a-2.0*birth*r)/c1;
    
    double oneMinusC2 = 1.0-c2;
    double onePlusC2  = 1.0+c2;
    
    double ct = -c1*t;
    double b = onePlusC2+oneMinusC2*exp(ct);
    double tmp = exp( ct ) / (b*b);
    
    return  tmp;    
}


/**
 * Simulate new speciation times.
 */
std::vector<double>* ConstantRateSerialSampledBirthDeathProcess::simSpeciations(size_t n, double origin) const
{

    // incorrect placeholder for constant BDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double birth = lambda->getValue();
    double death = mu->getValue();
    //double p     = psi->getValue();
    double r     = rho->getValue();
    
    std::vector<double>* times = new std::vector<double>(n, 0.0);
    
    for (size_t i = 0; i < n; ++i)
    {
        double u = rng->uniform01();
        
        // get the parameters
        double sp = birth*r;
        double ex = death - birth*(1.0-r);
        double div = sp - ex;
        
        double t = 1.0/div * log((sp - ex * exp((-div)*origin) - ex * (1.0 - exp((-div) * origin)) * u )/(sp - ex * exp((-div) * origin) - sp * (1.0 - exp(( -div ) * origin)) * u ) );
        
        (*times)[i] = t;
    }
    
    // finally sort the times
    std::sort(times->begin(), times->end());
    
    return times;
}


/**
 * Swap the parameters held by this distribution.
 *
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void ConstantRateSerialSampledBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == lambda) 
    {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == mu) 
    {
        mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == psi) 
    {
        psi = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == rho) 
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else 
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
    
}
