#include "DistributionExponential.h"
#include "PiecewiseConstantMorphospeciationProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathLogic.h"
#include "StochasticNode.h"
#include "TypedDistribution.h"

#include <algorithm>
#include <cmath>

#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;

/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    s              Speciation rates.
 * \param[in]    e              Extinction rates.
 * \param[in]    p              Fossil sampling rates.
 * \param[in]    c              Fossil observation counts.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    t              Rate change times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 */
PiecewiseConstantMorphospeciationProcess::PiecewiseConstantMorphospeciationProcess(const TypedDagNode<double> *ra,
                                                                                   const DagNode *inspeciation,
                                                                                   const DagNode *inextinction,
                                                                                   const DagNode *inpsi,
                                                                                   const DagNode *incounts,
                                                                                   const TypedDagNode<double> *inrho,
                                                                                   const DagNode *inlambda_a,
                                                                                   const DagNode *inbeta,
                                                                                   const TypedDagNode< RbVector<double> > *intimes,
                                                                                   const std::string &incondition,
                                                                                   const std::vector<Taxon> &intaxa,
                                                                                   bool uo,
                                                                                   bool pa,
                                                                                   bool ex,
                                                                                   size_t n,
                                                                                   size_t k) :
    PiecewiseConstantFossilizedBirthDeathProcess(ra,inspeciation,inextinction,inpsi,incounts,inrho,inlambda_a,inbeta,intimes,incondition,intaxa,uo,pa,ex),
    num_sites(n),
    num_states(k),
    num_site_rates(1),
    transition_prob_matrices(),
    active_likelihood( std::vector<bool>(5, 0) ),
    changed_nodes( std::vector<bool>(5, false) ),
    dirty_nodes( std::vector<bool>(5, true) ),
    node_partial_likelihoods( std::vector<std::vector<std::vector<std::vector<std::vector<double> > > > >(1, std::vector<std::vector<std::vector<std::vector<double> > > >(2, std::vector<std::vector<std::vector<double> > >(1, std::vector<std::vector<double> >(num_sites, std::vector<double>(num_states, 0.0))))) ),
    scaling_factors( std::vector<std::vector< std::vector<double> > >(1, std::vector<std::vector<double> >(2, std::vector<double>(num_sites, 0.0) ) ) ),
    site_rates(NULL),
    speciation_chars(std::vector<size_t>(num_sites,0)),
    rate_variation_across_sites(false),
    NUM_TIME_SLICES( 500.0 )
{
    // set the length of the time slices used by the ODE for numerical integration
    dt = process_age->getValue() / NUM_TIME_SLICES * 10.0;
}


PiecewiseConstantMorphospeciationProcess* PiecewiseConstantMorphospeciationProcess::clone( void ) const
{
    return new PiecewiseConstantMorphospeciationProcess( *this );
}


void PiecewiseConstantMorphospeciationProcess::updateTransitionProbabilities(size_t node_idx)
{
    const TopologyNode* node = this->value->getNodes()[node_idx];

    // second, get the clock rate for the branch
    double end_age = node->getAge();

    // if the tree is not a time tree, then the age will be not a number
    if ( RbMath::isFinite(end_age) == false )
    {
        // we assume by default that the end is at time 0
        end_age = 0.0;
    }
    double start_age = end_age + node->getBranchLength();

    // first, get the rate matrix for this branch
    for (size_t j = 0; j < this->num_site_rates; ++j)
    {
        double r = 1.0;
        if ( this->rate_variation_across_sites == true )
        {
            r = this->site_rates->getValue()[j];
        }

        for( size_t from = 0; from < this->num_states; from++)
        {
            numericallyIntegrateProcess( start_age, end_age, r, this->transition_prob_matrices[j][from]);
        }
    }
}


/**
 * Wrapper function for the ODE time stepper function.
 */
void PiecewiseConstantMorphospeciationProcess::numericallyIntegrateProcess(double begin_age, double end_age, double rate, state_type &transition_probs) const
{
    //    double dt = root_age->getValue() / NUM_TIME_SLICES * 10;
    typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > stepper_type;
    boost::numeric::odeint::integrate_adaptive( make_controlled( 1E-7, 1E-7, stepper_type() ) , *this , transition_probs, begin_age , end_age , dt );

    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    for (size_t i = 0; i < this->num_states; ++i)
    {
        // Sebastian: The likelihoods here are probability densities (not log-transformed).
        // These are densities because they are multiplied by the probability density of the speciation event happening.
        transition_probs[i] = ( transition_probs[i] < 0.0 ? 0.0 : transition_probs[i] );

    }

}


void PiecewiseConstantMorphospeciationProcess::operator()(const state_type &x, state_type &dxdt, const double t)
{
    size_t index = l(t);

    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    state_type safe_x = x;
    for (size_t i = 0; i < num_states; ++i)
    {
        safe_x[i] = ( x[i] < 0.0 ? 0.0 : x[i] );
    }

    double age = 0.0;
    for (size_t i = 0; i < num_states; ++i)
    {

        // no event
        double no_event_rate = getSpeciationRate(index) + getAnageneticSpeciationRate(index) + getExtinctionRate(index) + getFossilizationRate(index);

        dxdt[i] = - no_event_rate * safe_x[i];

        dxdt[i] += getSpeciationRate(index) * p(index, t) * safe_x[i];

        // anagenetic state change
        for (size_t j = 0; j < num_states; ++j)
        {
            double total = 0.0;

            if ( i != j )
            {
                total += safe_x[j];
            }

            dxdt[i] += total * (getSpeciationRate(index) * p(index, t) + getAnageneticSpeciationRate(index)) / double(num_states - 1);
        }

        dxdt[i] /= double(num_sites);

    } // end for num_states

}
