#include "Clade.h"
#include "EpisodicBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathLogic.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

EpisodicBirthDeathProcess::EpisodicBirthDeathProcess(const TypedDagNode<double> *org, const TypedDagNode<double> *ra,
                                                     const TypedDagNode<RbVector<double> > *sr, const TypedDagNode<RbVector<double> > *st,
                                                     const TypedDagNode<RbVector<double> > *er, const TypedDagNode<RbVector<double> > *et,
                                                     const TypedDagNode<double> *r, const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
                                                     const std::vector<Taxon> &tn) : BirthDeathProcess( org, ra, r, ss, ic, cdt, tn ),
    lambda_rates( sr ),
    lambda_times( st ),
    mu_rates( er ),
    mu_times( et )
{
    addParameter( lambda_rates );
    addParameter( lambda_times );
    addParameter( mu_rates );
    addParameter( mu_times );
    
    simulateTree();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
EpisodicBirthDeathProcess* EpisodicBirthDeathProcess::clone( void ) const
{
    
    return new EpisodicBirthDeathProcess( *this );
}



double EpisodicBirthDeathProcess::lnSpeciationRate(double t) const
{
    size_t index = lower_index(t);
    double ln_lambda = log( birth[ index ] );
    
    return ln_lambda;
}


/**
 *
 *
 */
size_t EpisodicBirthDeathProcess::lower_index(double t) const
{
    return lower_index(t,0,rate_change_times.size());
}




/**
 *
 *
 */
size_t EpisodicBirthDeathProcess::lower_index(double t, size_t min, size_t max) const
{
    size_t middle = min + (max - min) / 2;
    if ( min == middle )
    {
        return ( rate_change_times.size() > 0 && rate_change_times[max-1] < t ) ? max : min;
    }
    
    if ( rate_change_times[middle-1] > t )
    {
        return lower_index(t,min,middle);
    }
    else
    {
        return lower_index(t,middle,max);
    }
    
}




/**
 *
 *
 */
void EpisodicBirthDeathProcess::prepareProbComputation( void ) const
{
    
    // clean all the sets
    rate_change_times.clear();
    birth.clear();
    death.clear();
    
    double present_time = root_age->getValue();
    
    std::set<double> event_times;
    
    const std::vector<double>& birth_times = lambda_times->getValue();
    for (std::vector<double>::const_iterator it = birth_times.begin(); it != birth_times.end(); ++it)
    {
        event_times.insert( *it );
    }
    
    const std::vector<double>& death_times = mu_times->getValue();
    for (std::vector<double>::const_iterator it = death_times.begin(); it != death_times.end(); ++it)
    {
        event_times.insert( *it );
    }
    
    const std::vector<double> &b = lambda_rates->getValue();
    const std::vector<double> &d = mu_rates->getValue();
    
    size_t index_birth = b.size()-1;
    size_t index_death = d.size()-1;
    
    birth.push_back(    b[index_birth] );
    death.push_back(    d[index_death] );
    
    size_t pos = 0;
    for (std::set<double>::reverse_iterator it = event_times.rbegin(); it != event_times.rend(); ++it)
    {
        double t = *it;
        
        // add the time to our vector
        rate_change_times.push_back( present_time - t );
        
        // add the speciation rate at the rate-change event t
        pos = size_t( find(birth_times.begin(), birth_times.end(), t) - birth_times.begin() );
        if ( pos != birth_times.size() )
        {
            index_birth = pos;
            birth.push_back( b[index_birth] );
        }
        else
        {
            birth.push_back( b[index_birth] );
        }
        
        
        // add the extinction rate at the rate-change event t
        pos = size_t( find(death_times.begin(), death_times.end(), t) - death_times.begin() );
        if ( pos != death_times.size() )
        {
            index_death = pos;
            death.push_back( d[index_death] );
        }
        else
        {
            death.push_back( d[index_death] );
        }
        
    }
    
}


double EpisodicBirthDeathProcess::pSurvival(double start, double end) const
{
    // do the integration of int_{start}^{end} ( mu(s) exp(rate(t,s)) ds )
    // where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx ) - sum_{for all t < m_i < s in massExtinctionTimes }( log(massExtinctionSurvivalProbability[i]) )
    //
    // we compute the integral stepwise for each epoch
    
    double accummulated_rate_time = 0.0;
    double prev_time = start;
    double den = 1.0;
    
    size_t num_episodes = rate_change_times.size();
    double rate = 0.0;
    for ( size_t j=0; j<num_episodes; ++j )
    {
        // compute the rate
        rate = death[j] - birth[j];
        
        if ( (start < rate_change_times[j]) && (end >= rate_change_times[j]) )
        {
            // compute the integral for this time episode until the mass-extinction event
        
            den += ( exp(-rate*prev_time) * death[j] / rate * exp( accummulated_rate_time ) * ( exp(rate* rate_change_times[j]) - exp(rate*prev_time)));
            accummulated_rate_time +=  (rate*(rate_change_times[j]-prev_time));
            // store the current time so that we remember from which episode we need to integrate next
            prev_time = rate_change_times[j];
            // integrate over the tiny time interval of the mass-extinction event itself and add it to the integral
        
        }
        
    }
    
    size_t index = 0;
    if ( rate_change_times.size() > 0 )
    {
        index = lower_index(end);
    }
    rate = death[index] - birth[index];
    
    // add the integral of the final epoch until the present time
    den = den + exp(-rate*prev_time) * exp( accummulated_rate_time ) * death[index] / rate * ( exp(rate*end) - exp(rate*prev_time));
        
    double res = 1.0 / den;
    
//    if ( RbMath::isFinite( res ) == false || res == 0.0 )
//    {
//        pSurvival(start, end);
//    }
    
    return res;

}



double EpisodicBirthDeathProcess::rateIntegral(double start, double end) const
{
    
    double accummulated_rate_time = 0.0;
    double prev_time = start;
    
    size_t num_episodes = rate_change_times.size();
    double rate = 0.0;
    for ( size_t j=0; j<num_episodes; ++j )
    {
        // compute the rate
        rate = death[j] - birth[j];
        
        if ( (start < rate_change_times[j]) && (end >= rate_change_times[j]) )
        {
            // compute the integral for this time episode until the mass-extinction event
            
            accummulated_rate_time +=  (rate*(rate_change_times[j]-prev_time));
            // store the current time so that we remember from which episode we need to integrate next
            prev_time = rate_change_times[j];
            // integrate over the tiny time interval of the mass-extinction event itself and add it to the integral
            
        }
        
    }
    
    size_t index = 0;
    if ( rate_change_times.size() > 0 )
    {
        index = lower_index(end);
    }
    rate = death[index] - birth[index];
    accummulated_rate_time +=  (rate*(end-prev_time));
    
    return accummulated_rate_time;
}



double EpisodicBirthDeathProcess::simulateDivergenceTime(double origin, double present, double rho) const
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda_rates->getValue()[0];
    double d = mu_rates->getValue()[0];
    
    
    // get a random draw
    double u = rng->uniform01();
    
    // compute the time for this draw
    double t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(rho*b+(b*(1-rho)-d)*exp((d-b)*age) ) ) ) - (b*(1-rho)-d) ) / (rho * b) ) + (d-b)*age )  /  (d-b);
    
    
    return present - t;
}


/** Swap a parameter of the distribution */
void EpisodicBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == lambda_rates )
    {
        lambda_rates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if ( oldP == lambda_times )
    {
        lambda_times = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if ( oldP == mu_rates )
    {
        mu_rates = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if ( oldP == mu_times )
    {
        mu_times = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        // delegate the super-class
        BirthDeathProcess::swapParameterInternal(oldP, newP);
    }
    
}
