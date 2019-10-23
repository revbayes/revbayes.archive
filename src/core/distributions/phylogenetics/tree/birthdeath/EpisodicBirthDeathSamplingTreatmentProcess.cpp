#include <float.h>
#include <stddef.h>
#include <algorithm>
#include <cmath>
#include <iosfwd>
#include <string>
#include <vector>

#include "DistributionExponential.h"
#include "EpisodicBirthDeathSamplingTreatmentProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "RbMathLogic.h"
#include "AbstractBirthDeathProcess.h"
#include "DagNode.h"
#include "RbException.h"
#include "RbVector.h"
#include "StartingTreeSimulator.h"
#include "TopologyNode.h"
#include "Tree.h"
#include "TypedDagNode.h"

namespace RevBayesCore { class Taxon; }

using namespace RevBayesCore;

/**
 * Constructor.
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    s              Speciation rates.
 * \param[in]    e              Extinction rates.
 * \param[in]    p              Serial sampling rates.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    t              Rate change times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
EpisodicBirthDeathSamplingTreatmentProcess::EpisodicBirthDeathSamplingTreatmentProcess(const TypedDagNode<double> *ra,
                                                                                           const DagNode *in_speciation,
                                                                                           const DagNode *in_extinction,
                                                                                           const DagNode *in_sampling,
                                                                                           const DagNode *in_treatment,
                                                                                           const DagNode *in_event_speciation,
                                                                                           const DagNode *in_event_extinction,
                                                                                           const DagNode *in_event_sampling,
                                                                                           const DagNode *in_event_treatment,
                                                                                           const TypedDagNode< RbVector<double> > *timeline,
                                                                                           const TypedDagNode< RbVector<double> > *speciation_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *extinction_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *sampling_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *treatment_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *event_speciation_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *event_extinction_timeline,
                                                                                           const TypedDagNode< RbVector<double> > *event_sampling_timeline,
                                                                                           const std::string &cdt,
                                                                                           const std::vector<Taxon> &tn,
                                                                                           bool uo) : AbstractBirthDeathProcess( ra, cdt, tn, uo ),
    interval_times_global(timeline),
    offset( 0.0 )
{
    // initialize all the pointers to NULL
    homogeneous_lambda   = NULL;
    homogeneous_mu       = NULL;
    homogeneous_phi      = NULL;
    homogeneous_r        = NULL;
    // homogeneous_Lambda   = NULL;
    // homogeneous_Mu       = NULL;
    homogeneous_Phi      = NULL;
    heterogeneous_lambda = NULL;
    heterogeneous_mu     = NULL;
    heterogeneous_phi    = NULL;
    heterogeneous_r      = NULL;
    heterogeneous_Lambda = NULL;
    heterogeneous_Mu     = NULL;
    heterogeneous_Phi    = NULL;

    std::vector<double> times = timeline->getValue();
    std::vector<double> times_sorted_ascending = times;

    sort(times_sorted_ascending.begin(), times_sorted_ascending.end() );

    if ( times != times_sorted_ascending )
    {
        throw(RbException("Rate change times must be provided in ascending order."));
    }

    addParameter( interval_times_global );

    heterogeneous_lambda = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_speciation);
    homogeneous_lambda = dynamic_cast<const TypedDagNode<double >*>(in_speciation);

    addParameter( homogeneous_lambda );
    addParameter( heterogeneous_lambda );

    heterogeneous_mu = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_extinction);
    homogeneous_mu = dynamic_cast<const TypedDagNode<double >*>(in_extinction);

    addParameter( homogeneous_mu );
    addParameter( heterogeneous_mu );

    heterogeneous_phi = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_sampling);
    homogeneous_phi = dynamic_cast<const TypedDagNode<double >*>(in_sampling);

    addParameter( homogeneous_phi );
    addParameter( heterogeneous_phi );

    heterogeneous_r = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_treatment);
    homogeneous_r = dynamic_cast<const TypedDagNode<double >*>(in_treatment);

    addParameter( homogeneous_r );
    addParameter( heterogeneous_r );

    heterogeneous_Lambda = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_event_speciation);

    addParameter( heterogeneous_Lambda );

    heterogeneous_Mu = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_event_extinction);

    addParameter( heterogeneous_Mu );

    heterogeneous_Phi = dynamic_cast<const TypedDagNode<RbVector<double> >*>(in_event_sampling);
    homogeneous_Phi = dynamic_cast<const TypedDagNode<double >*>(in_event_sampling);

    addParameter( homogeneous_Phi );
    addParameter( heterogeneous_Phi );

    //TODO: make sure the offset is added properly into the computation, need to offset *all* times, including interval times
    //          this means we also need to check that the first interval time is not less than the first tip (which we should probably to anyways)

    //TODO: returning neginf and nan are not currently coherent

    updateVectorParameters();
    prepareProbComputation();


    RbVector<Clade> constr;
    StartingTreeSimulator simulator;
    RevBayesCore::Tree *my_tree = simulator.simulateTree( taxa, constr );

    // store the new value
    delete value;
    value = my_tree;

    countAllNodes();

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
EpisodicBirthDeathSamplingTreatmentProcess* EpisodicBirthDeathSamplingTreatmentProcess::clone( void ) const
{
    return new EpisodicBirthDeathSamplingTreatmentProcess( *this );
}

/**
 * Checks that v1 is the correct size compared to reference vector v2, given the expected size difference.
 * If the sizes are wrong, throws an exception.
 * Uses param_name and is_rate to make a sensible error message
 */
void EpisodicBirthDeathSamplingTreatmentProcess::checkVectorSizes(TypedDagNode<RbVector<double> >* v1, TypedDagNode<RbVector<double> >* v2, int v1_minus_v2, std::string &param_name, bool is_rate) const
{
  if ( v1->getValue().size() - v2->getValue().size() != v1_minus_v2 )
  {
    std::string vec_type = is_rate ? "rates" : "probabilities";
    std::stringstream ss;
    ss << "Number of " << param_name << " " << vec_type << " (" << v1->getValue().size() << ") does not match number of time intervals (" << v2->getValue().size() << ")";
    throw RbException(ss.str());
  }
}

/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double EpisodicBirthDeathSamplingTreatmentProcess::computeLnProbabilityDivergenceTimes( void )
{
    // @TODO @ANDY Need to use big-R for event sampling times
    // update parameter vectors
    updateVectorParameters();

    // Assign nodes to sets
    countAllNodes();

    if ( offset > DBL_EPSILON && phi_event[0] > DBL_EPSILON )
    {
      throw(RbException("Event sampling fraction at the present is non-zero but there are no tips at the present."));
    }

    // precompute A_i, B_i, C_i, E_i(t_i)
    prepareProbComputation();

    // variable declarations and initialization
    double lnProbTimes = computeLnProbabilityTimes();

    return lnProbTimes;
}


/**
 * Compute the log probability of the current value under the current parameter values.
 */
double EpisodicBirthDeathSamplingTreatmentProcess::computeLnProbabilityTimes( void )
{
    // variable declarations and initialization
    double lnProbTimes = 0.0;
    //    double process_time = getOriginAge(); // Sebastian: currently unused.
    size_t num_initial_lineages = 0;
    TopologyNode* root = &value->getRoot();

    if ( use_origin == true )
    {
        // If we are conditioning on survival from the origin,
        // then we must divide by 2 the log survival probability computed by AbstractBirthDeathProcess
        // TODO: Generalize AbstractBirthDeathProcess to allow conditioning on the origin
        num_initial_lineages = 1;
    }
    // if conditioning on root, root node must be a "true" bifurcation event
    else
    {
        if (root->getChild(0).isSampledAncestor() || root->getChild(1).isSampledAncestor())
        {
            return RbConstants::Double::neginf;
        }

        num_initial_lineages = 2;
    }

    // get node/time variables
    size_t num_nodes = value->getNumberOfNodes();


    // add the event-sampling terms (ii)
    for (size_t i = 0; i < global_timeline.size(); ++i)
    {
        // Only compute sampling probability when there is a sampling event
        if (phi_event[i] > DBL_EPSILON)
        {
            if ( RbMath::isFinite(lnProbTimes) == false )
            {
                // throw(RbException("nan-likelihood in (ii)"));
                return RbConstants::Double::nan;
            }

            // Calculate probability of the samples
            double ln_sampling_event_prob = 0.0;
            int R_i = int(event_sampled_ancestor_ages[i].size());
            int N_i = R_i + int(event_tip_ages[i].size());
            int active_lineages_at_t = survivors(global_timeline[i]); //A(t_{\rho_i})

            if (N_i == 0)
            {
                return RbConstants::Double::neginf;
                //std::stringstream ss;
                //ss << "The event sampling rate at timeline[ " << i << "] is > 0, but the tree has no samples at this time.";
                //throw RbException(ss.str());
            }

            // Make sure that we aren't claiming to have sampled all lineages without having sampled all lineages
            if (phi_event[i] >= (1.0 - DBL_EPSILON) && (active_lineages_at_t != N_i) )
            {
                return RbConstants::Double::neginf;
                //std::stringstream ss;
                //ss << "The event sampling rate at timeline[ " << i << "] is one, but the tree has unsampled tips at this time.";
                //throw RbException(ss.str());

            }
            else
            {
                ln_sampling_event_prob += N_i * log(phi_event[i]);
                if ( (active_lineages_at_t - N_i) > 0 )
                {
                    ln_sampling_event_prob += (active_lineages_at_t - N_i) * log(1 - phi_event[i]);
                }
            }

            // Calculate probability of the sampled ancestors
            if ( r[i] >= (1.0 - DBL_EPSILON) && R_i > 0 )
            {
                // Cannot have sampled ancestors if r(t) == 1
                return RbConstants::Double::neginf;
                //std::stringstream ss;
                //ss << "The conditional probability of death on sampling rate in interval " << i << " is one, but the tree has sampled ancesors in this interval.";
                //throw RbException(ss.str());
            }
            if ( global_timeline[i] > DBL_EPSILON )
            {
                // only add these terms for sampling that is not at the present
                ln_sampling_event_prob += R_i * log(1 - r[i]);
                ln_sampling_event_prob += (N_i - R_i) * log(r[i] * (1 - r[i])*E(i,global_timeline[i]));
            }
            lnProbTimes += ln_sampling_event_prob;
        }
    }
    // std::cout << "computed (ii); lnProbability = " << lnProbTimes << std::endl;

    // add the serial tip age terms (iii)
    for (size_t i = 0; i < serial_tip_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }

        double t = serial_tip_ages[i];
        size_t index = findIndex(t);

        // add the log probability for the serial sampling events
        if ( phi[index] == 0.0 )
        {
            return RbConstants::Double::neginf;
            //std::stringstream ss;
            //ss << "The serial sampling rate in interval " << i << " is zero, but the tree has serial sampled tips in this interval.";
            //throw RbException(ss.str());
        }
        else
        {
            double this_prob = r[index] + (1 - r[index]) * E(index,t);
            this_prob *= phi[index];
            // double this_prob = phi[index] * r[index];
            // // Avoid computation in the case of r = 1
            // if (r[t] <= 1 - DBL_EPSILON)
            // {
            //   this_prob += phi[index] * (1 - r[index]) * E(index,t);
            // }
            lnProbTimes += log( this_prob );
        }
    }
// std::cout << "computed (iii); lnProbability = " << lnProbTimes << std::endl;

    // add the serial sampled ancestor terms (iv)
    for (size_t i=0; i < serial_sampled_ancestor_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }

        double t = serial_sampled_ancestor_ages[i];
        size_t index = findIndex(t);

        if ( r[index] > 1.0 - DBL_EPSILON )
        {
            return RbConstants::Double::neginf;
            //std::stringstream ss;
            //ss << "The conditional probability of death on sampling rate in interval " << i << " is one, but the tree has sampled ancesors in this interval.";
            //throw RbException(ss.str());
        }

        lnProbTimes += log(phi[index]) + log(1 - r[index]);

    }

    // add the burst bifurcation age terms (v)
    for (size_t i = 0; i < global_timeline.size(); ++i)
    {
        // Nothing to compute if the burst probability is 0
        if (lambda_event[i] > DBL_EPSILON)
        {
            lnProbTimes += event_bifurcation_times[i].size() * log(lambda_event[i]);
            int active_lineages_at_t = survivors(global_timeline[i]); //A(t_{\rho_i})
            int A_minus_K = active_lineages_at_t - int(event_bifurcation_times[i].size());
            lnProbTimes += log(pow(2*lambda_event[i]*E(i,global_timeline[i]),A_minus_K)+pow(1.0 - lambda_event[i],A_minus_K));
        }
    }

    // add the non-burst bifurcation age terms (vi)
    for (size_t i = 0; i < serial_bifurcation_times.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }

        double t = serial_bifurcation_times[i];
        size_t index = findIndex(t);
        lnProbTimes += log( lambda[index] );
    }

    // Compute probabilities of branch segments on all branches
    for (size_t i=0; i<num_nodes; ++i)
    {
      const TopologyNode& n = value->getNode( i );

      double t = n.getAge();
      size_t index = findIndex(t);
      double this_ln_D = lnD(index,t);
      if ( n.isTip() )
      {
        lnProbTimes -= this_ln_D;
      }
      else
      {
        lnProbTimes += this_ln_D;
      }

//         if ( !n.isRoot() && !n.isSampledAncestor() )
//         {
// //        double t_start = n.getParent().getAge();
// //        double t_end = n.getAge();
//             double t_start = n.getAge();
//             double t_end = n.getParent().getAge();
//
//             size_t interval_t_start = findIndex(t_start);
//             size_t interval_t_end = findIndex(t_end);
//
//             double t_o = t_start;
//             size_t interval_t_o = interval_t_start;
//
//             while ( interval_t_o < interval_t_end )
//             {
//                 double t_y = timeline[interval_t_o+1];
//                 size_t interval_t_y = interval_t_o + 1;
//
//                 lnProbTimes -= lnD(interval_t_o,t_o);
//                 lnProbTimes += lnD(interval_t_y,t_y);
//
//                 t_o = t_y;
//                 interval_t_o = interval_t_y;
//             }
//
//             lnProbTimes -= lnD(interval_t_o,t_o);
//             lnProbTimes += lnD(interval_t_end,t_end);
//             // std::cout << "    computing (vii); t_o = " << t_o << "; t_end = " << t_end << "; lnD(interval_t_o,t_o) = " << lnD(interval_t_o,t_o) << "; lnD(interval_t_end,t_end) = " << lnD(interval_t_end,t_end) << std::endl;
//             // std::cout << "lnProbTimes is now " << lnProbTimes << std::endl;
//         }

      }
      lnProbTimes += lnD(findIndex(value->getRoot().getAge()),value->getRoot().getAge());

// std::cout << "computed (vii); lnProbability = " << lnProbTimes << std::endl;

    // condition on survival
    if ( condition == "survival" )
    {
        double root_age = (&value->getRoot())->getAge();
        // conditioning on survival depends on if we are using the origin or root age
        // origin: we condition on a single lineage surviving to the present and being sampled
        // root: we condition on the above plus the other root child leaving a sampled descendant
        lnProbTimes -= log( pSurvival(root_age,0.0) );
        if ( num_initial_lineages == 2 )
        {
          lnProbTimes -= log( pSampling(root_age) );
        }
    }
    else if ( condition == "sampling" )
    {
        // conditioning on sampling depends on if we are using the origin or root age
        // origin: the conditioning suggested by Stadler 2011 and used by Gavryuskina (2014), sampling at least one lineage
        // root age: sampling at least one descendent from each child of the root
        double root_age = (&value->getRoot())->getAge();
        lnProbTimes -= num_initial_lineages * log( pSampling(root_age) );
    }

    if ( RbMath::isFinite(lnProbTimes) == false )
    {
        return RbConstants::Double::nan;
    }


    return lnProbTimes;
}

/*
 * Counts all nodes in a number of sets
 * 1) bifurcation (birth) events NOT at burst times
 * 2) bifurcation (birth) events at burst times
 * 3) sampled ancestor nodes NOT at sampling events
 * 4) sampled nodes from extinct lineages NOT at sampling events
 * 5) sampled ancestor nodes at sampling events
 * 6) sampled nodes from extinct lineages at sampling events
 *
 * Non-burst trackers (1,3,4) are vectors of times of the samples.
 * All burst trackers (2,5,6) are vectors of vectors of samples, each vector corresponding to an event
 */
void EpisodicBirthDeathSamplingTreatmentProcess::countAllNodes(void)
{
  // get node/time variables
  size_t num_nodes = value->getNumberOfNodes();

  size_t num_extant_taxa = 0;

  // clear current
  serial_tip_ages.clear();
  serial_sampled_ancestor_ages.clear();
  event_tip_ages.clear();
  event_sampled_ancestor_ages.clear();
  serial_bifurcation_times.clear();
  event_bifurcation_times.clear();

  // initialize vectors of vectors
  event_sampled_ancestor_ages = std::vector<std::vector<double> >(global_timeline.size(),std::vector<double>(0,1.0));
  event_tip_ages = std::vector<std::vector<double> >(global_timeline.size(),std::vector<double>(0,1.0));
  event_bifurcation_times = std::vector<std::vector<double> >(global_timeline.size(),std::vector<double>(0,1.0));

  // Assign all node times (bifurcations, sampled ancestors, and tips) to their sets
  for (size_t i = 0; i < num_nodes; i++)
  {
      const TopologyNode& n = value->getNode( i );

      double t = n.getAge();

      if ( n.isTip() && n.isFossil() && n.isSampledAncestor() )
      {
        // node is sampled ancestor
          int at_event = whichIntervalTime(t);

          // If this tip is not at an event time (and specifically at an event time with Phi[i] > 0), it's a serial tip
          if (at_event == -1 || phi_event[at_event] < DBL_EPSILON)
          {
            serial_sampled_ancestor_ages.push_back(t);
          }
          else
          {
            event_sampled_ancestor_ages[i].push_back(t);
          }
      }
      else if ( n.isTip() && n.isFossil() && !n.isSampledAncestor() )
      {
          // node is serial leaf
          int at_event = whichIntervalTime(t);

          // If this tip is not at an event time (and specifically at an event time with Phi[i] > 0), it's a serial tip
          if (at_event == -1 || phi_event[at_event] < DBL_EPSILON)
          {
            serial_tip_ages.push_back(t);
          }
          else
          {
            event_tip_ages[i].push_back(t);
          }
      }
      else if ( n.isTip() && !n.isFossil() )
      {
        // Node is at present, this can happen even if Phi[0] = 0, so we check if there is really a sampling event at the present
        if (phi_event[0] >= DBL_EPSILON)
        {
          // node is extant leaf
          num_extant_taxa++;
          event_tip_ages[0].push_back(0.0);
        }
        else
        {
          serial_tip_ages.push_back(0.0);
        }
      }
      else if ( n.isInternal() && !n.getChild(0).isSampledAncestor() && !n.getChild(1).isSampledAncestor() )
      {
          if ( n.isRoot() == false || use_origin == true )
          {
              // node is bifurcation event (a "true" node)
              int at_event = whichIntervalTime(t);

              // If this bifurcation is not at an event time (and specifically at an event time with Lambda[i] > 0), it's a serial bifurcation
              if (at_event == -1 || lambda_event[at_event] < DBL_EPSILON)
              {
                serial_bifurcation_times.push_back(t);
              }
              else
              {
                event_bifurcation_times[i].push_back(t);
              }
          }
      }
  }
}

// /**
//  * Compute D_i(t)
//  */
// double EpisodicBirthDeathSamplingTreatmentProcess::D(size_t i, double t) const
// {
//   // D(0) = 1
//   if ( t < DBL_EPSILON )
//   {
//     return 1.0;
//   }
//   else
//   {
//     double D_i = 4 * exp(-A_i[i] * (t - timeline[i]));
//     D_i /= pow((1 + B_i[i] + (exp(-A_i[i] * (t - timeline[i]))) * (1 - B_i[i])), 2.0);
//     return D_i;
//   }
// }

/**
 * Compute ln(D_i(t))
 */
double EpisodicBirthDeathSamplingTreatmentProcess::lnD(size_t i, double t) const
{
    // D(0) = 1
    if ( t < DBL_EPSILON )
    {
        // TODO: this can't be right, if phi_event[0] = 0 this will blow up
        // return log(phi_event[0]);
        return phi_event[0] <= DBL_EPSILON ? 0.0 : log(phi_event[0]);
    }
    else
    {
        double s = global_timeline[i];
        double this_lnD_i = 0.0;
        if (i > 0)
        {
            // D <- D * (1-this_p_s) * (1-this_p_d) * (1-this_p_b + 2*this_p_b*E)
            this_lnD_i = lnD_previous[i];
            // std::cout << "this_lnD_i is now " << this_lnD_i << std::endl;
            this_lnD_i += log(1.0-phi_event[i]) + log(1.0-mu_event[i]) + log(1-lambda_event[i]+2*lambda_event[i]*E_previous[i]);
            // std::cout << "this_lnD_i is now " << this_lnD_i << std::endl;
        }
        else
        {
            this_lnD_i = phi_event[0] <= DBL_EPSILON ? 0.0 : log(phi_event[0]);
            // std::cout << "this_lnD_i is now " << this_lnD_i << std::endl;
        }
        // D <- D * 4 * exp(-A*(next_t-current_t))
        // D <- D / ( 1+B+exp(-A*(next_t-current_t))*(1-B) )^2
        this_lnD_i += 2*RbConstants::LN2 + (-A_i[i] * (t - s));
        // std::cout << "this_lnD_i is now " << this_lnD_i << std::endl;
        this_lnD_i -= 2 * log(1 + B_i[i] + exp(-A_i[i] * (t - s)) * (1 - B_i[i]));

        return this_lnD_i;
    }
}

/**
 * Compute E_i(t)
 */
double EpisodicBirthDeathSamplingTreatmentProcess::E(size_t i, double t, bool computeSurvival) const
{
    double E_i;
    double s = global_timeline[i];

    // Are we computing E(t) for survival conditioning?
    if (computeSurvival == true)
    {
      // E <- (b + d - A *(1+B-exp(-A*(next_t-current_t))*(1-B))/(1+B+exp(-A*(next_t-current_t))*(1-B)) ) / (2*b)
      E_i = lambda[i] + mu[i];
      E_i -= A_survival_i[i] * (1 + B_survival_i[i] - exp(-A_survival_i[i] * (t - s)) * (1 - B_survival_i[i])) / (1 + B_survival_i[i] + exp(-A_survival_i[i] * (t - s)) * (1 - B_survival_i[i]));
      E_i /= (2 * lambda[i]);
    }
    else
    {
      // E <- (b + d + s - A *(1+B-exp(-A*(next_t-current_t))*(1-B))/(1+B+exp(-A*(next_t-current_t))*(1-B)) ) / (2*b)
      E_i = lambda[i] + mu[i] + phi[i];
      E_i -= A_i[i] * (1 + B_i[i] - exp(-A_i[i] * (t - s)) * (1 - B_i[i])) / (1 + B_i[i] + exp(-A_i[i] * (t - s)) * (1 - B_i[i]));
      E_i /= (2 * lambda[i]);
    }

    return E_i;

}

/**
 * return the index i so that s_{i-1} <= t < s_i
 * where s_i is the global timeline of events
 * s_0 = 0.0
 * s_l = Inf
 */
size_t EpisodicBirthDeathSamplingTreatmentProcess::findIndex(double t) const
{
    // Linear search for interval because std::lower_bound is not cooperating
    if (global_timeline.size() == 1)
    {
        // If global_timeline.size() is 1, we have 0 break points and are in constant-rate version
        return 0;
    }
    else if (global_timeline.size() == 2)
    {
        return(t < global_timeline[1] ? 0 : 1);
    }
    else
    {
        for (size_t i=0; i < global_timeline.size()-1; ++i)
        {
            if (t >= global_timeline[i] && t < global_timeline[i+1])
            {
                return i;
            }
        }

        return global_timeline.size() - 1;
    }
}

/**
 * return the index i so that x_{i-1} <= t < x_i
 * where x is one of the input vector timelines
 */
size_t EpisodicBirthDeathSamplingTreatmentProcess::findIndex(double t, std::vector<double> &timeline) const
{

    // Linear search for interval because std::lower_bound is not cooperating
    if (timeline.size() == 1)
    {
        return 0;
    }
    else if (timeline.size() == 2)
    {
        return(t < timeline[1] ? 0 : 1);
    }
    else
    {
        for (size_t i=0; i < timeline.size()-1; ++i)
        {
            if (t >= timeline[i] && t < timeline[i+1])
            {
                return i;
            }
        }

        return timeline.size() - 1;
    }
}

// calculate offset so we can set t_0 to time of most recent tip
void EpisodicBirthDeathSamplingTreatmentProcess::getOffset(void) const
{
    // On first pass, there is no tree, so we can't loop over nodes
    // Get taxon ages directly from taxa instead
    if ( value->getNumberOfNodes() == 0 )
    {
        offset = RbConstants::Double::max;
        for (size_t i = 0; i < taxa.size(); i++)
        {
            const Taxon& n = taxa[i];

            if ( n.getAge() < offset )
            {
                offset = n.getAge();
            }
        }
    }
    // On later passes we have the tree, to avoid any issues with tree and taxon age mismatch, get ages from tree
    else
    {
        offset = RbConstants::Double::max;
        for (size_t i = 0; i < value->getNumberOfNodes(); i++)
        {
            const TopologyNode& n = value->getNode( i );

            if ( n.getAge() < offset )
            {
                offset = n.getAge();
            }
        }
    }

}

// double EpisodicBirthDeathSamplingTreatmentProcess::lnProbNumTaxa(size_t n, double start, double end, bool MRCA) const
// {
//   throw(RbException("Cannot compute lnProbNumTaxa."));
//   return RbConstants::Double::neginf;
// }

double EpisodicBirthDeathSamplingTreatmentProcess::lnProbTreeShape(void) const
{
    // the birth death divergence times density is derived for a (ranked) unlabeled oriented tree
    // so we convert to a (ranked) labeled non-oriented tree probability by multiplying by 2^{n+m-1} / n!
    // where n is the number of extant tips, m is the number of extinct tips

    int num_taxa = (int)value->getNumberOfTips();
    int num_extinct = (int)value->getNumberOfExtinctTips();
    int num_sa = (int)value->getNumberOfSampledAncestors();

    // return (num_taxa - num_sa - 1) * RbConstants::LN2 - RbMath::lnFactorial(num_taxa - num_extinct);
    //Gavryushkina (2014) uses the following
    return (num_taxa - num_sa - 1) * RbConstants::LN2 - RbMath::lnFactorial(num_taxa);
}

/**
 * Takes a par.size() < global_timeline.size() vector and makes it the correct size to work with our global timeline.
 * The parameter has its own reference timeline, which we use to find the rate in the global intervals.
 *
 */
void EpisodicBirthDeathSamplingTreatmentProcess::updateNonGlobalParameterVector(std::vector<double> &par, std::vector<double> &par_times)
{
    // Store the original values so we can overwrite the vector
    std::vector<double> old_par = par;

    // For each time in the global timeline, find the rate according to this variable's own timeline
    for (size_t i=0; i<global_timeline.size(); ++i)
    {
      // Where is this global time interval in the variable's timeline?
      size_t idx = findIndex(global_timeline[i],par_times);
      par[i] = old_par[idx];
    }

}

/**
 * Here we ensure that we have a length-l vector of rates and events for all parameters and the timeline.
 * In the case of homogeneous/constant-rate parameters, we fill the vector with this rate.
 * In the case of homogenous/single events, we make all but the first event rates 0.
 *
 */
void EpisodicBirthDeathSamplingTreatmentProcess::updateVectorParameters( void )
{
    // clean and get timeline
    global_timeline.clear();
    global_timeline = interval_times_global->getValue();

    // Add t_0
    getOffset();
    global_timeline.insert(global_timeline.begin(),offset);

    // clean all the sets
    lambda.clear();
    mu.clear();
    phi.clear();
    r.clear();
    lambda_event.clear();
    mu_event.clear();
    phi_event.clear();

    // Get vector of birth rates
    if ( heterogeneous_lambda != NULL )
    {
      lambda = heterogeneous_lambda->getValue();
    }
    else
    {
      lambda = std::vector<double>(global_timeline.size(),homogeneous_lambda->getValue());
    }

    // Get vector of death rates
    if ( heterogeneous_mu != NULL )
    {
      mu = heterogeneous_mu->getValue();
    }
    else
    {
      mu = std::vector<double>(global_timeline.size(),homogeneous_mu->getValue());
    }

    // Get vector of serial sampling rates
    if ( heterogeneous_phi != NULL )
    {
      phi = heterogeneous_phi->getValue();
    }
    else
    {
      phi = std::vector<double>(global_timeline.size(),homogeneous_phi->getValue());
    }

    // Get vector of conditional death upon sampling probabilities
    if ( heterogeneous_r != NULL )
    {
      r = heterogeneous_r->getValue();
    }
    else
    {
      r = std::vector<double>(global_timeline.size(),homogeneous_r->getValue());
    }

    // Get vector of burst birth probabilities
    if ( heterogeneous_Lambda != NULL )
    {
      // User has specified lambda_event_1,...,lambda_event_{l-1}
      lambda_event = heterogeneous_Lambda->getValue();
      // lambda_event_0 must be 0 (there can be no burst at the present)
      lambda_event.insert(lambda_event.begin(),0.0);
    }
    else
    {
      // User specified nothing, there are no birth bursts
      lambda_event = std::vector<double>(global_timeline.size(),0.0);
    }

    // Get vector of burst death (mass extinction) probabilities
    if ( heterogeneous_Mu != NULL )
    {
      // User has specified mu_event_1,...,mu_event_{l-1}
      mu_event = heterogeneous_Mu->getValue();
      // mu_event_0 must be 0 (there can be no burst at the present)
      mu_event.insert(mu_event.begin(),0.0);
    }
    else
    {
      // User specified nothing, there are no birth bursts
      mu_event = std::vector<double>(global_timeline.size(),0.0);
    }

    // Get vector of event sampling probabilities
    if ( heterogeneous_Phi != NULL )
    {
      // User has specified phi_event_0,...,phi_event_{l-1}
      phi_event = heterogeneous_Phi->getValue();
    }
    else
    {
        phi_event = std::vector<double>(global_timeline.size(),0.0);
        if ( homogeneous_Phi != NULL )
        {
            // User specified the sampling fraction at the present
            phi_event[0] = homogeneous_Phi->getValue();
        }
        else
        {
            // set the final sampling to one (for sampling at the present)
            phi_event[0] = 1.0;
      }

    }

}

/*
 * Here we calculate all A_i, B_i, C_i, D_i(s_i), and E_i(s_i) for i = 1,...,l
 *
 */
void EpisodicBirthDeathSamplingTreatmentProcess::prepareProbComputation( void )
{
    // TODO: B and C are producing nan values upon initialization, but not when computing tree probabilities, which are working fine

    // re-initialize all the sets
    A_i = std::vector<double>(global_timeline.size(),0.0);
    B_i = std::vector<double>(global_timeline.size(),0.0);
    C_i = std::vector<double>(global_timeline.size(),0.0);

    // E_previous[i] is E_{i-1}(s_i)
    // lnD_previous[i] is log(D_{i-1}(s_i))
    E_previous      = std::vector<double>(global_timeline.size(),0.0);
    lnD_previous    = std::vector<double>(global_timeline.size(),0.0);

    // timeline[0] == 0.0
    double t = global_timeline[0];

    // Compute all starting at 1
    A_i[0] = sqrt( pow(lambda[0] - mu[0] - phi[0],2.0) + 4 * lambda[0] * phi[0]);

    // At the present, only sampling is allowed, no birth/death bursts
    C_i[0] = (1 - phi_event[0]);

    B_i[0] = (1.0 - 2.0 * C_i[0]) * lambda[0] + mu[0] + phi[0];
    B_i[0] /= A_i[0];

    // E_{i-1}(0) = 1, and our E(i,t) function requires i >= 0, so we hard-code this explicitly
    // Sebastian: This should be the probability of going extinct, which is in this case the probability of non-sampling.
    // Andy: This is not E_0(t_0) this is E_{-1}(t_0)
    // E_previous[0] = (1 - phi_event[0]);
    E_previous[0] = 1.0;

    // we always initialize the probability of observing the lineage at the present with the sampling probability
    // TODO: This can't be right, if there is no event sampling this will blow up
    lnD_previous[0] = 0.0;

    for (size_t i=1; i<global_timeline.size(); ++i)
    {
        t = global_timeline[i];

        // first, we need to compute E and D at the end of the previous interval
        E_previous[i] = E(i-1, t, false);
        lnD_previous[i] = lnD(i-1, t);

        // now we can compute A_i, B_i and C_i at the end of this interval.
        A_i[i] = sqrt( pow(lambda[i] - mu[i] - phi[i],2.0) + 4 * lambda[i] * phi[i]);

        // Only one type of event is allowed
        if (phi_event[i] >= DBL_EPSILON)
        {
          C_i[i] = (1 - phi_event[i]) * E_previous[i];
        } else if ( lambda_event[i] >= DBL_EPSILON )
        {
          C_i[i] =  (1 - lambda_event[i]) * E_previous[i] + lambda_event[i] * E_previous[i] * E_previous[i];
        } else if ( lambda_event[i] >= DBL_EPSILON )
        {
          C_i[i] = (1 - mu_event[i]) * E_previous[i] + mu_event[i];
        } else
        {
          C_i[i] = E_previous[i];
        }

        B_i[i] = (1.0 - 2.0 * C_i[i]) * lambda[i] + mu[i] + phi[i];
        B_i[i] /= A_i[i];

    }

    // if we want to condition on survival we need to track versions of A,B,C,E that set phi = 0 and Phi[-0] to 0
    if ( condition == "survival" )
    {
      // timeline[0] == 0.0
      double t = global_timeline[0];

      A_survival_i = std::vector<double>(global_timeline.size(),0.0);
      B_survival_i = std::vector<double>(global_timeline.size(),0.0);
      C_survival_i = std::vector<double>(global_timeline.size(),0.0);
      E_survival_previous = std::vector<double>(global_timeline.size(),0.0);

      // Compute all starting at 1
      A_survival_i[0] = sqrt( pow(lambda[0] - mu[0],2.0));

      // At the present, only sampling is allowed, no birth/death bursts
      C_survival_i[0] = (1 - phi_event[0]);

      B_survival_i[0] = (1.0 - 2.0 * C_survival_i[0]) * lambda[0] + mu[0];
      B_survival_i[0] /= A_survival_i[0];

      E_survival_previous[0] = 1.0;

      for (size_t i=1; i<global_timeline.size(); ++i)
      {
          t = global_timeline[i];

          // first, we need to compute E and D at the end of the previous interval
          E_survival_previous[i] = E(i-1, t, true);

          // now we can compute A_survival_i, B_survival_i and C_survival_i at the end of this interval.
          A_survival_i[i] = sqrt( pow(lambda[i] - mu[i],2.0));

          // Only one type of event is allowed
          if ( lambda_event[i] >= DBL_EPSILON )
          {
            C_survival_i[i] =  (1 - lambda_event[i]) * E_survival_previous[i] + lambda_event[i] * E_survival_previous[i] * E_survival_previous[i];
          } else if ( lambda_event[i] >= DBL_EPSILON )
          {
            C_survival_i[i] = (1 - mu_event[i]) * E_survival_previous[i] + mu_event[i];
          } else
          {
            C_survival_i[i] = E_survival_previous[i];
          }

          B_survival_i[i] = (1.0 - 2.0 * C_survival_i[i]) * lambda[i] + mu[i];
          B_survival_i[i] /= A_survival_i[i];
      }

    }

}


void EpisodicBirthDeathSamplingTreatmentProcess::prepareTimeline( void )
{
    // @TODO: @ANDY: Fill in the function to assemble the master timeline and all the parameter vectors!!!

    // clean and get timeline
    global_timeline.clear();
    global_timeline = interval_times_global->getValue();

    // Add t_0
    getOffset();
    global_timeline.insert(global_timeline.begin(),offset);

    // clean all the sets
    lambda.clear();
    mu.clear();
    phi.clear();
    r.clear();
    lambda_event.clear();
    mu_event.clear();
    phi_event.clear();
    r_event.clear();

    // @TODO: @ANDY: Check that we cleared all parameters!


    // first, we are checking for the global timeline
    if ( interval_times_global != NULL )
    {
        if ( interval_times_speciation != NULL ||
             interval_times_extinction != NULL ||
             interval_times_sampling != NULL ||
             interval_times_treatment != NULL ||
             interval_times_event_speciation != NULL ||
             interval_times_event_sampling != NULL ||
             interval_times_event_extinction != NULL )
        {
            throw RbException("Both heterogeneous and homogeneous rate change times provided");
        }


        // @TODO: Make sure that times and parameters are stored backwards in time!
        
//        std::vector<double> times = interval_times_global->getValue();
//        std::vector<double> times_sorted_ascending = times;
//        std::vector<double> times_sorted_descending = times;
//
//        sort(times_sorted_ascending.begin(), times_sorted_ascending.end() );
//        sort(times_sorted_descending.rbegin(), times_sorted_descending.rend() );
//        if( times == times_sorted_ascending )
//        {
//            ascending = true;
//        }
//        else if ( times != times_sorted_ascending )
//        {
//            throw RbException("Rate change times must be provided in sorted order");
//        }


        // check that the number of provided parameters matches the global timeline
        if ( heterogeneous_lambda->getValue().size() != interval_times_global->getValue().size() )
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << interval_times_global->getValue().size() << ")";
            throw RbException(ss.str());
        }

        if ( heterogeneous_mu->getValue().size() != interval_times_global->getValue().size() )
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << interval_times_global->getValue().size() << ")";
            throw RbException(ss.str());
        }

        // @TODO: @ANDY: keep on checking for all other parameters!

        // ...

        global_timeline = interval_times_global->getValue();

        // we are done with setting up the timeline (i.e., using the provided global timeline) and checking all dimension of parameters
    }
    else
    {

        // check if speciation rates are provided in the correct form
        if ( heterogeneous_lambda == NULL && homogeneous_lambda == NULL)
        {
            throw RbException("Speciation rate must be of type RealPos or RealPos[]");
        }
        else if ( heterogeneous_lambda != NULL )
        {
            if ( interval_times_speciation == NULL ) throw RbException("No time intervals provided for piecewise constant speciation rates");

            if ( interval_times_speciation != NULL && heterogeneous_lambda->getValue().size() != interval_times_speciation->getValue().size() + 1 )
            {
                std::stringstream ss;
                ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << interval_times_speciation->getValue().size() + 1 << ")";
                throw RbException(ss.str());
            }
        }

        // @TODO: @ANDY: do this check for all parameters too!

        // ...


        // now we start assembling the global timeline by finding the union of unique intervals for all parameters
        global_timeline.clear();
        std::set<double> event_times;

        // @TODO: Do this within a function
        if ( interval_times_speciation != NULL )
        {
            const std::vector<double>& birth_times = interval_times_speciation->getValue();
            for (std::vector<double>::const_iterator it = birth_times.begin(); it != birth_times.end(); ++it)
            {
                event_times.insert( *it );
            }
        }

        if ( interval_times_extinction != NULL )
        {
            const std::vector<double>& death_times = interval_times_extinction->getValue();
            for (std::vector<double>::const_iterator it = death_times.begin(); it != death_times.end(); ++it)
            {
                event_times.insert( *it );
            }
        }

        // @TODO: @ANDY: keep on adding times from all other timelines

        // ...

        // we are done with setting up the timeline (i.e., using the all the provided timeline) and checking all dimension of parameters

    }

    // @TODO: @ANDY: Check about the offset
    global_timeline.push_back( offset );

    // now we fill the parameter vectors for each interval
    for (size_t i = 0; i < global_timeline.size(); i++)
    {
        // @TODO: @ANDY: Check that these functions exist and do what we want!
//        lambda.push_back( getSpeciationRate(i) );
//        mu.push_back( getExtinctionRate(i) );
//        phi.push_back( getSerialSamplingRate(i) );
//        rho.push_back( getTaxonSamplingProbability(i, timeline.size()) );

        // @TODO: @ANDY: Make sure this populates properly all parameter vectors (backwards in time, etc.)
    }

}

double EpisodicBirthDeathSamplingTreatmentProcess::pSampling(double start) const
{
  return (1.0 - E(findIndex(start),start,false));
}

double EpisodicBirthDeathSamplingTreatmentProcess::pSurvival(double start, double end) const
{
  // This computation does not make sense unless there is sampling at the present, and will result in a divide by 0 error
  if ( phi_event[0] < DBL_EPSILON )
  {
    return(RbConstants::Double::neginf);
  }
  return( (1.0 - E(findIndex(start),start,true))/(1.0 - E(findIndex(end),end,true)) );
}

/**
 * Simulate new speciation times.
 */
double EpisodicBirthDeathSamplingTreatmentProcess::simulateDivergenceTime(double origin, double present) const
{
    // incorrect placeholder for constant SSBDP


    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;

    size_t i = findIndex(present);

    // get the parameters
    double age = origin - present;
    double b = lambda[i];
    double d = mu[i];
    double p_e = phi_event[i];


    // get a random draw
    double u = rng->uniform01();

    // compute the time for this draw
    // see Hartmann et al. 2010 and Stadler 2011
    double t = 0.0;
    if ( b > d )
    {
        if( p_e > 0.0 )
        {
            t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(p_e*b+(b*(1-p_e)-d)*exp((d-b)*age) ) ) ) - (b*(1-p_e)-d) ) / (p_e * b) ) )  /  (b-d);
        }
        else
        {
            t = log( 1 - u * (exp(age*(d-b)) - 1) / exp(age*(d-b)) ) / (b-d);
        }
    }
    else
    {
        if( p_e > 0.0 )
        {
            t = ( log( ( (b-d) / (1 - (u)*(1-(b-d)/(p_e*b*exp((b-d)*age)+(b*(1-p_e)-d) ) ) ) - (b*(1-p_e)-d) ) / (p_e * b) ) )  /  (b-d);
        }
        else
        {
            t = log( 1 - u * (1 - exp(age*(b-d)))  ) / (b-d);
        }
    }

    return present + t;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int EpisodicBirthDeathSamplingTreatmentProcess::survivors(double t) const
{

    const std::vector<TopologyNode*>& nodes = value->getNodes();

    int survivors = 0;
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        TopologyNode* n = *it;
        double a = n->getAge();
        if ( (a - t) <= DBL_EPSILON )
        {
            if ( n->isRoot() == true || (n->getParent().getAge() - t) >= -DBL_EPSILON )
            {
                survivors++;
            }
        }
    }

    return survivors;
}

/**
 * Sorts times to run from present to past (0->inf) and orders par to match this.
 */
void EpisodicBirthDeathSamplingTreatmentProcess::sortVectorParameterAndTimes(std::vector<double> &times, std::vector<double> &par)
{
  std::vector<double> times_sorted_ascending = times;
  std::vector<double> times_sorted_descending = times;

  sort(times_sorted_ascending.begin(), times_sorted_ascending.end() );
  sort(times_sorted_descending.rbegin(), times_sorted_descending.rend() );

  // We want times in ascending order, so if they already are we're done here
  if ( times != times_sorted_ascending )
  {
      // If times are sorted in descending order, we just flip the parameter and time vectors
      if ( times == times_sorted_ascending )
      {
        std::reverse(times.begin(),times.end());
        std::reverse(par.begin(),par.end());
      }
      else
      {
        // Pair up the times and the parameter values so we can sort them together
        std::vector<std::pair<double,double> > times_par;
        for (size_t i=0; i<times.size(); ++i)
        {
          times_par.push_back(std::make_pair(times[i],par[i]));
        }

        std::sort(times_par.begin(),times_par.end());

        // Replace times with sorted times
        for (size_t i=0; i<times.size(); ++i)
        {
          times[i] = times_par[i].first;
          par[i] = times_par[i].second;
        }

      }
  }

}

/**
 * For a time t, determine which if any interval (event) time it corresponds to
 *
 * \param[in]    t      time we want to assess
 *
 * \return -1 if time matches no interval, otherwise the interval
 */
int EpisodicBirthDeathSamplingTreatmentProcess::whichIntervalTime(double t) const
{
    // Find the i such that s_i <= t < s_{i+1}
    size_t i = findIndex(t);

    // Check if s_i == t
    if ( t > global_timeline[i] + DBL_EPSILON && t < global_timeline[i] - DBL_EPSILON )
    {
        return (int) i;
    }
    else
    {
        return -1;
    }

}

/**
 * Swap the parameters held by this distribution.
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void EpisodicBirthDeathSamplingTreatmentProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    // Rate parameters
    if (oldP == heterogeneous_lambda)
    {
        heterogeneous_lambda = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_mu)
    {
        heterogeneous_mu = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_phi)
    {
        heterogeneous_phi = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == homogeneous_lambda)
    {
        homogeneous_lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_mu)
    {
        homogeneous_mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_phi)
    {
        homogeneous_phi = static_cast<const TypedDagNode<double>* >( newP );
    }
    // Treatment
    else if (oldP == heterogeneous_r)
    {
        heterogeneous_r = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == homogeneous_r)
    {
        homogeneous_r = static_cast<const TypedDagNode<double>* >( newP );
    }
    // Event probability parameters
    if (oldP == heterogeneous_Lambda)
    {
        heterogeneous_Lambda = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_Mu)
    {
        heterogeneous_Mu = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_Phi)
    {
        heterogeneous_Phi = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    // else if (oldP == homogeneous_Lambda)
    // {
    //     homogeneous_Lambda = static_cast<const TypedDagNode<double>* >( newP );
    // }
    // else if (oldP == homogeneous_Mu)
    // {
    //     homogeneous_Mu = static_cast<const TypedDagNode<double>* >( newP );
    // }
    else if (oldP == homogeneous_Phi)
    {
        homogeneous_Phi = static_cast<const TypedDagNode<double>* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
}
