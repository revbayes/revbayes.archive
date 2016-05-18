//
//  ConstantRateOutgroupBirthDeathProcess.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 5/5/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "ConstantRateOutgroupBirthDeathProcess.h"
#include "DistributionExponential.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"

#include <cmath>
#include <algorithm>


using namespace RevBayesCore;


/**
 * Constructor.
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    ra				Age or the root (=time of the process).
 * \param[in]    s              Speciation rate.
 * \param[in]    e              Extinction rate.
 * \param[in]    r              Sampling probability at present time.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
ConstantRateOutgroupBirthDeathProcess::ConstantRateOutgroupBirthDeathProcess(    const TypedDagNode<double> *ra,
                                                                                 const TypedDagNode<double> *s,
                                                                                 const TypedDagNode<double> *e,
                                                                                 const TypedDagNode<double> *r,
                                                                                 const std::string& cdt,
                                                                                 const std::vector<Taxon> &tn) : AbstractBirthDeathProcess( ra, cdt, tn ),
lambda( s ),
mu( e ),
rho( r )
{
    addParameter( lambda );
    addParameter( mu );
    addParameter( rho );
    
    simulateTree();
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantRateOutgroupBirthDeathProcess* ConstantRateOutgroupBirthDeathProcess::clone( void ) const
{
    
    return new ConstantRateOutgroupBirthDeathProcess( *this );
}

/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 * \return    The log-probability density.
 */
double ConstantRateOutgroupBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    
    double lnProbTimes = 0.0;
    double process_time = 0.0;
    size_t num_initial_lineages = 0;
    TopologyNode* root = &value->getRoot();
    
    // the process starts at the root age with one lineage
    process_time = root->getAge();
    num_initial_lineages = 1;
    
    // variable declarations and initialization
    double birth_rate = lambda->getValue();
    double death_rate = mu->getValue();
    double sampling_prob = rho->getValue();
    
    // get node/time variables
    size_t num_nodes = value->getNumberOfNodes();
    
    // get node ages
    std::vector<double> internal_node_ages = std::vector<double>();
    for (size_t i = 0; i < num_nodes; i++)
    {
        const TopologyNode& n = value->getNode( i );
        
        // drop the root node (the split time caused by the outgroup
        if ( n.isInternal() && !n.isRoot() )
            internal_node_ages.push_back( n.getAge() );
    }
    
    
    // get joint density of reconstructed speciation times
    for(size_t i=0; i<internal_node_ages.size(); i++)
    {
        double t = internal_node_ages[i];
        lnProbTimes += log( birth_rate * pOne(t, birth_rate, death_rate, sampling_prob) );
    }
    
    // condition on survival
    lnProbTimes += num_initial_lineages * log( pZero(process_time, birth_rate, death_rate, sampling_prob) );
    
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
double ConstantRateOutgroupBirthDeathProcess::pSurvival(double start, double end) const
{
    // variable declarations and initialization
    double birth_rate = lambda->getValue();
    double death_rate = mu->getValue();
    double sampling_prob = rho->getValue();
    
    double p0 = pZero(end, birth_rate, death_rate, sampling_prob);
    
    return 1.0 - p0;
}



/**
 * Simulate new speciation times.
 */
double ConstantRateOutgroupBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    
    // incorrect placeholder for constant FBDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda->getValue();
    double d = mu->getValue();
    double r = rho->getValue();
    
    // get a random draw
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
    
    //    return present - t;
    return origin + t;
}


/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void ConstantRateOutgroupBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == lambda)
    {
        lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == mu)
    {
        mu = static_cast<const TypedDagNode<double>* >( newP );
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



double ConstantRateOutgroupBirthDeathProcess::pZero(double t, double b, double d, double r) const
{
    double v = 1.0 - r*(b-d)/(r*b+(b*(1-r)-d)*exp(-(b-d)*t));
    return v;
}


double ConstantRateOutgroupBirthDeathProcess::pOne(double t, double b, double d, double r) const
{

    double v = r * exp(-(b-d)*t) * pow((b-d)/(r*b + (b*(1-r)-d)*exp(-(b-d)*t)), 2);
    return v;
}
