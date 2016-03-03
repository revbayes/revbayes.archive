#include "DistributionExponential.h"
#include "ConstantRateSerialSampledBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"

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
                                                                       const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( o, ra, cdt, tn ),
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
    double sampling_prob     = rho->getValue();
    
    
    // present time
    double ra = value->getRoot().getAge();
    double presentTime = 0.0;
    
    size_t num_initial_lineages = 1;
    
    // test that the time of the process is larger or equal to the present time
    if ( starts_at_root == false )
    {
        double org = origin->getValue();
        presentTime = org;
        
    }
    else
    {
        presentTime = ra;
        num_initial_lineages = 2;
    }
    
    // retrieved the speciation times
    std::vector<double>* agesInternalNodes  = getAgesOfInternalNodesFromMostRecentSample();
    std::vector<double>* agesTips           = getAgesOfTipsFromMostRecentSample();
    
    // multiply the probabilities for the tip ages
    for (size_t i = 0; i < agesTips->size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesTips)[i];
//        if ( t == 0.0 && r > 0.0 )
        if ( t < 0.1 && sampling_prob > 0.0 )
        {
            lnProbTimes += log( 4.0 * sampling_prob );
        }
        else
        {
            lnProbTimes += log( p ) + logQ( t + timeSinceLastSample );
        }
        
    }
    
    for (size_t i = 0; i < num_taxa-num_initial_lineages; ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = (*agesInternalNodes)[i];
        lnProbTimes += logQ(t+timeSinceLastSample) + log( birth );
    }
    
    lnProbTimes += num_initial_lineages * logQ( presentTime );
    
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
    double birth_rate     = lambda->getValue();
    double death_rate      = mu->getValue();
    double fossil_rate     = psi->getValue();
    double sampling_prob   = rho->getValue();
    
    double a = (birth_rate - death_rate - fossil_rate);
    double c1 = sqrt( a*a + 4*birth_rate*fossil_rate );
    double c2 = - (a-2.0*birth_rate*sampling_prob)/c1;
    
    double oneMinusC2 = 1.0-c2;
    double onePlusC2  = 1.0+c2;
    
    double e = exp(-c1*t);
    
    double p0 = ( birth_rate + death_rate + sampling_prob + c1 * ( e * oneMinusC2 - onePlusC2 ) / ( e * oneMinusC2 + onePlusC2 ) ) / (2.0 * birth_rate);
    
    return 1.0 - p0;
}



/**
 *
 */
double ConstantRateSerialSampledBirthDeathProcess::logQ( double t ) const
{
    
    // get the parameters
    double birth_rate   = lambda->getValue();
    double death_rate   = mu->getValue();
    double fossil_rate  = psi->getValue();
    double r     = rho->getValue();
    
    double a = (birth_rate - death_rate - fossil_rate);
    double c1 = sqrt( a*a + 4*birth_rate*fossil_rate );
    double c2 = - (a-2.0*birth_rate*r)/c1;
    
    double oneMinusC2 = 1.0-c2;
    double onePlusC2  = 1.0+c2;
    
    double ct = -c1*t;
    
    double b1 = onePlusC2+oneMinusC2*exp(ct);
    double result1 = ct - 2.0 * log(b1);
    
    double b2 = onePlusC2*exp(-ct)+oneMinusC2;
    double result2 = - ct - 2.0 * log(b2);
    
    if ( ct < 0)
    {
        return result1;
    }
    else
    {
        return result2;
    }
    
}


/**
 * Simulate new speciation times.
 */
double ConstantRateSerialSampledBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{

    // incorrect placeholder for constant BDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda->getValue();
    double d = mu->getValue();
    //double p     = psi->getValue();
    double r     = rho->getValue();
    
    
    double u = rng->uniform01();
    
    
    // compute the time for this draw
    double t = 0.0;
    if ( b > d )
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(r*b+(b*(1-r)-d)*exp((d-b)*age) ) ) ) - (b*(1-r)-d) ) / (r * b) ) + (d-b)*age )  /  (d-b);
    }
    else
    {
        t = ( log( ( (b-d) / (1 - (u)*(1-(b-d)/(r*b*exp((b-d)*age)+(b*(1-r)-d) ) ) ) - (b*(1-r)-d) ) / (r * b) ) + (d-b)*age )  /  (d-b);
    }
    
    return present - t;
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
