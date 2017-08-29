#include "DistributionExponential.h"
#include "PiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"
#include "RbMathCombinatorialFunctions.h"
#include "StochasticNode.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

/**
 * Constructor. 
 * We delegate most parameters to the base class and initialize the members.
 *
 * \param[in]    s              Speciation rates.
 * \param[in]    e              Extinction rates.
 * \param[in]    p              Fossil sampling rates.
 * \param[in]    r              Instantaneous sampling probabilities.
 * \param[in]    t              Rate change times.
 * \param[in]    cdt            Condition of the process (none/survival/#Taxa).
 * \param[in]    tn             Taxa.
 * \param[in]    c              Clades conditioned to be present.
 */
PiecewiseConstantSerialSampledBirthDeathProcess::PiecewiseConstantSerialSampledBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                                           const DagNode *l,
                                                                                           const DagNode *m,
                                                                                           const DagNode *p,
                                                                                           const DagNode *r,
                                                                                           const TypedDagNode< RbVector<double> > *ht,
                                                                                           const TypedDagNode< RbVector<double> > *lt,
                                                                                           const TypedDagNode< RbVector<double> > *mt,
                                                                                           const TypedDagNode< RbVector<double> > *pt,
                                                                                           const TypedDagNode< RbVector<double> > *rt,
                                                                                           const std::string &cdt,
                                                                                           const std::vector<Taxon> &tn,
                                                                                           bool uo ) : AbstractBirthDeathProcess( ra, cdt, tn, uo ),
    homogeneous_timeline(ht),
    lambda_timeline(lt),
    mu_timeline(mt),
    psi_timeline(pt),
    rho_timeline(rt)
{
    // initialize all the pointers to NULL
    homogeneous_lambda   = NULL;
    homogeneous_mu       = NULL;
    homogeneous_psi      = NULL;
    homogeneous_rho      = NULL;
    heterogeneous_lambda = NULL;
    heterogeneous_mu     = NULL;
    heterogeneous_psi    = NULL;
    heterogeneous_rho    = NULL;

    if( homogeneous_timeline != NULL )
    {
        if( lambda_timeline != NULL || mu_timeline != NULL || psi_timeline != NULL || rho_timeline != NULL )
        {
            throw(RbException("Both heterogeneous and homogeneous rate change times provided"));
        }

        std::vector<double> t = homogeneous_timeline->getValue();
        std::vector<double> rt = t;
        std::sort(rt.rbegin(), rt.rend());

        if(rt != t)
        {
            throw(RbException("Rate change times must be provided in descending order"));
        }

        addParameter( homogeneous_timeline );
    }

    const TypedDagNode<RbVector<double> > *tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(l);
    const TypedDagNode<double> *tmp_c = dynamic_cast<const TypedDagNode<double >*>(l);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Speciation rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_lambda = tmp_c;
        addParameter( homogeneous_lambda );
    }
    else
    {
        heterogeneous_lambda = tmp_v;
        if( lambda_timeline != NULL && heterogeneous_lambda->getValue().size() != lambda_timeline->getValue().size() + 1 )
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << lambda_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        else if(homogeneous_timeline != NULL && heterogeneous_lambda->getValue().size() != homogeneous_timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << homogeneous_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        if(lambda_timeline != NULL)
        {
            std::vector<double> t = lambda_timeline->getValue();
            std::vector<double> rt = t;
            std::sort(rt.rbegin(), rt.rend());

            if(rt != t)
            {
                throw(RbException("Speciation rate change times must be provided in descending order"));
            }
        }

        addParameter( heterogeneous_lambda );
        addParameter( lambda_timeline );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(m);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(m);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Extinction rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_mu = tmp_c;
        addParameter( homogeneous_mu );
    }
    else
    {
        heterogeneous_mu = tmp_v;
        if( mu_timeline != NULL && heterogeneous_mu->getValue().size() != mu_timeline->getValue().size() + 1 )
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << mu_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        else if(homogeneous_timeline != NULL && heterogeneous_mu->getValue().size() != homogeneous_timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << homogeneous_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        if(mu_timeline != NULL)
        {
            std::vector<double> t = mu_timeline->getValue();
            std::vector<double> rt = t;
            std::sort(rt.rbegin(), rt.rend());

            if(rt != t)
            {
                throw(RbException("Extinction rate change times must be provided in descending order"));
            }
        }

        addParameter( heterogeneous_mu );
        addParameter( mu_timeline );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(p);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(p);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Serial sampling rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_psi = tmp_c;
        addParameter( homogeneous_psi );
    }
    else
    {
        heterogeneous_psi = tmp_v;
        if( psi_timeline != NULL && heterogeneous_psi->getValue().size() != psi_timeline->getValue().size() + 1 )
        {
            std::stringstream ss;
            ss << "Number of serial sampling rates (" << heterogeneous_psi->getValue().size() << ") does not match number of time intervals (" << psi_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        else if(homogeneous_timeline != NULL && heterogeneous_psi->getValue().size() != homogeneous_timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of serial sampling rates (" << heterogeneous_psi->getValue().size() << ") does not match number of time intervals (" << homogeneous_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        if(psi_timeline != NULL)
        {
            std::vector<double> t = psi_timeline->getValue();
            std::vector<double> rt = t;
            std::sort(rt.rbegin(), rt.rend());

            if(rt != t)
            {
                throw(RbException("Serial sampling rate change times must be provided in descending order"));
            }
        }

        addParameter( heterogeneous_psi );
        addParameter( psi_timeline );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(r);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(r);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Taxon sampling probabilities must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_rho = tmp_c;
        addParameter( homogeneous_rho );
    }
    else
    {
        heterogeneous_rho = tmp_v;
        if( rho_timeline != NULL && heterogeneous_rho->getValue().size() != rho_timeline->getValue().size() + 1 )
        {
            std::stringstream ss;
            ss << "Number of taxon sampling probabilities (" << heterogeneous_rho->getValue().size() << ") does not match number of time intervals (" << rho_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }
        else if(homogeneous_timeline != NULL && heterogeneous_rho->getValue().size() != homogeneous_timeline->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of taxon sampling probabilities (" << heterogeneous_rho->getValue().size() << ") does not match number of time intervals (" << homogeneous_timeline->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        if(rho_timeline != NULL)
        {
            std::vector<double> t = rho_timeline->getValue();
            std::vector<double> rt = t;
            std::sort(rt.rbegin(), rt.rend());

            if(rt != t)
            {
                throw(RbException("taxon sampling probability change times must be provided in descending order"));
            }
        }

        addParameter( heterogeneous_rho );
        addParameter( rho_timeline );
    }

    simulateTree();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
PiecewiseConstantSerialSampledBirthDeathProcess* PiecewiseConstantSerialSampledBirthDeathProcess::clone( void ) const
{
    return new PiecewiseConstantSerialSampledBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::computeLnProbabilityDivergenceTimes( void ) const
{
    // prepare the probability computation
    prepareProbComputation();

    // variable declarations and initialization
    double lnProbTimes = computeLnProbabilityTimes();

    return lnProbTimes;
}


/**
 * Compute the log probability of the current value under the current parameter values.
 * Tip-dating (Theorem 1, Stadler et al. 2013 PNAS)
 * Ancestral fossils will be considered in the future.
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    // variable declarations and initialization
    double lnProbTimes = 0.0;
    double process_time = getOriginAge();
    size_t num_initial_lineages = 0;
    TopologyNode* root = &value->getRoot();

    if (use_origin) {
        // If we are conditioning on survival from the origin,
        // then we must divide by 2 the log survival probability computed by AbstractBirthDeathProcess
        // TODO: Generalize AbstractBirthDeathProcess to allow conditioning on the origin
        num_initial_lineages = 1;
    }
    // if conditioning on root, root node must be a "true" bifurcation event
    else
    {
        if (root->getChild(0).isSampledAncestor() || root->getChild(1).isSampledAncestor())
            return RbConstants::Double::neginf;

        num_initial_lineages = 2;
    }
    
    // get node/time variables
    size_t num_nodes = value->getNumberOfNodes();

    // classify nodes
    int num_sampled_ancestors = 0;
    int num_fossil_taxa = 0;
    int num_extant_taxa = 0;

    // retrieved the speciation times
    std::vector<double> fossil_tip_ages = std::vector<double>();
    std::vector<double> sampled_ancestor_ages = std::vector<double>();
    std::vector<double> internal_node_ages = std::vector<double>();

    for (size_t i = 0; i < num_nodes; i++)
    {
        const TopologyNode& n = value->getNode( i );

        if ( n.isTip() && n.isFossil() && n.isSampledAncestor() )
        {
            // node is sampled ancestor
            num_sampled_ancestors++;
            sampled_ancestor_ages.push_back(n.getAge());
        }
        else if ( n.isTip() && n.isFossil() && !n.isSampledAncestor() )
        {
            // node is fossil leaf
            num_fossil_taxa++;
            fossil_tip_ages.push_back( n.getAge() );
        }
        else if ( n.isTip() && !n.isFossil() )
        {
            // node is extant leaf
            num_extant_taxa++;
        }
        else if ( n.isInternal() && !n.getChild(0).isSampledAncestor() && !n.getChild(1).isSampledAncestor() )
        {
            if(!n.isRoot() || use_origin)
            {
                // node is bifurcation event (a "true" node)
                internal_node_ages.push_back( n.getAge() );
            }
        }
    }
    
    // add the fossil tip age terms
    for (size_t i = 0; i < fossil_tip_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = fossil_tip_ages[i];
        size_t index = l(t);

        // add the log probability for the fossilization events
        if (psi[index - 1] == 0.0)
        {
            std::stringstream ss;
            ss << "The serial sampling rate in interval " << index << " is zero, but the tree has serial sampled tips in this interval.";
            throw RbException(ss.str());
        }
        else
        {
            lnProbTimes += log( psi[index - 1] ) + log( p(index, t) / q(index, t) );
        }
    }

    if( heterogeneous_rho != NULL )
    {
        std::vector<double> rho_times = rho_timeline->getValue();
        rho_times.push_back(0.0);

        for (size_t i = 0; i < value->getNumberOfTips(); i++)
        {
            const TopologyNode& n = value->getNode( i );

            std::vector<double>::iterator it = std::find(rho_times.begin(), rho_times.end(), n.getAge() );

            if( it != rho_times.end() )
            {
                // add the rho-sampled tip age term
                lnProbTimes += log( heterogeneous_rho->getValue()[size_t( it - rho_times.begin() )] );
            }
        }
    }
    else
    {
        // add the extant tip age term
        lnProbTimes += num_extant_taxa * log( homogeneous_rho->getValue() );
    }

    // add the sampled ancestor age terms
    for (size_t i = 0; i < sampled_ancestor_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = sampled_ancestor_ages[i];
        size_t index = l(t);
        lnProbTimes += log( psi[index - 1] );
    }

    // add the single lineage propagation terms
    for (size_t i = 1; i <= timeline.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = timeline[i-1];
        int div = survivors(t);
        lnProbTimes += div * log( q(i+1, t) );
    }

    // add the bifurcation age terms
    for (size_t i = 0; i < internal_node_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = internal_node_ages[i];
        size_t index = l(t);
        lnProbTimes += log( 2.0 * q(index, t) * lambda[index - 1] );
    }

    // add the initial age term
    lnProbTimes += num_initial_lineages * log( q(1, process_time ) );

    // condition on survival
    if ( condition == "survival" )
    {
        lnProbTimes -= num_initial_lineages * log( pSurvival(process_time,0) );
    }
    // condition on nTaxa
    else if ( condition == "nTaxa" )
    {
        lnProbTimes -= lnProbNumTaxa( value->getNumberOfTips(), 0, process_time, true );
    }

    if ( RbMath::isFinite(lnProbTimes) == false )
    {
        return RbConstants::Double::nan;
    }


    return lnProbTimes;
}


/**
 * return the index i so that t_{i-1} > t >= t_i
 * where t_i is the instantaneous sampling time (i = 0,...,l)
 * t_0 is origin
 * t_l = 0.0
 */
size_t PiecewiseConstantSerialSampledBirthDeathProcess::l(double t) const
{
    return timeline.rend() - std::upper_bound( timeline.rbegin(), timeline.rend(), t) + 1;
}


double PiecewiseConstantSerialSampledBirthDeathProcess::lnProbTreeShape(void) const
{
    // the birth death divergence times density is derived for a (ranked) unlabeled oriented tree
    // so we convert to a (ranked) labeled non-oriented tree probability by multiplying by 2^{n-1} / ((n-m)! m!)
    // where m is the number of extinct tips

    size_t num_taxa = value->getNumberOfTips();
    size_t num_extinct = value->getNumberOfExtinctTips();

    return (num_taxa - 1) * RbConstants::LN2 - RbMath::lnFactorial(num_taxa - num_extinct) - RbMath::lnFactorial(num_extinct);
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Probability of survival.
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::pSurvival(double start, double end) const
{
    double t = start;
    
    std::vector<double> serial_bak = psi;

    std::fill(psi.begin(), psi.end(), 0.0);

    double p0 = p(l(t), t);
    
    psi = serial_bak;

    return 1.0 - p0;
}


/**
 * p_i(t)
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::p( size_t i, double t ) const
{
    if ( t == 0)
        return 1.0;

    // get the parameters
    double b = lambda[i-1];
    double d = mu[i-1];
    double f = psi[i-1];
    double r = rho[i-1];
    double ti = timeline[i-1];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;
    
    double e = exp(-A*dt);
    double tmp = b + d + f - A * ((1.0+B)-e*(1.0-B))/((1.0+B)+e*(1.0-B));
    
    return tmp / (2.0*b);
}


/**
 *
 *
 */
void PiecewiseConstantSerialSampledBirthDeathProcess::prepareProbComputation( void ) const
{
    // clean all the sets
    lambda.clear();
    mu.clear();
    psi.clear();
    rho.clear();
    
    if(homogeneous_timeline != NULL)
    {
        timeline = homogeneous_timeline->getValue();

        timeline.push_back(0.0);

        for (size_t i = 0; i < timeline.size(); i++)
        {
            lambda.push_back( getSpeciationRate(i) );

            mu.push_back( getExtinctionRate(i) );

            psi.push_back( getSerialSamplingRate(i) );

            rho.push_back( getTaxonSamplingProbability(i, timeline.size()) );
        }
    }
    else
    {
        timeline.clear();

        std::set<double> eventTimes;

        if( lambda_timeline != NULL )
        {
            const std::vector<double>& birthTimes = lambda_timeline->getValue();
            for (std::vector<double>::const_iterator it = birthTimes.begin(); it != birthTimes.end(); ++it)
            {
                eventTimes.insert( *it );
            }
        }

        if( mu_timeline != NULL )
        {
            const std::vector<double>& deathTimes = mu_timeline->getValue();
            for (std::vector<double>::const_iterator it = deathTimes.begin(); it != deathTimes.end(); ++it)
            {
                eventTimes.insert( *it );
            }
        }

        if( psi_timeline != NULL )
        {
            const std::vector<double>& fossilTimes = psi_timeline->getValue();
            for (std::vector<double>::const_iterator it = fossilTimes.begin(); it != fossilTimes.end(); ++it)
            {
                eventTimes.insert( *it );
            }
        }

        if( rho_timeline != NULL )
        {
            const std::vector<double>& samplingTimes = rho_timeline->getValue();
            for (std::vector<double>::const_iterator it = samplingTimes.begin(); it != samplingTimes.end(); ++it)
            {
                eventTimes.insert( *it );
            }
        }

        for(std::set<double>::reverse_iterator it = eventTimes.rbegin(); it != eventTimes.rend(); it++)
        {
            timeline.push_back(*it);
        }
        timeline.push_back(0.0);


        size_t birth_index = 0;
        size_t death_index = 0;
        size_t serial_index = 0;
        size_t fraction_index = 0;

        for (size_t i = 0; i < timeline.size(); i++)
        {
            if( lambda_timeline != NULL )
            {
                lambda.push_back( heterogeneous_lambda->getValue()[birth_index] );

                if( birth_index < lambda_timeline->getValue().size() && lambda_timeline->getValue()[birth_index] == timeline[i] )
                {
                    birth_index++;
                }
            }
            else
            {
                lambda.push_back( homogeneous_lambda->getValue() );
            }

            if( mu_timeline != NULL )
            {
                mu.push_back( heterogeneous_mu->getValue()[death_index] );

                if( death_index < mu_timeline->getValue().size() && mu_timeline->getValue()[death_index] == timeline[i] )
                {
                    death_index++;
                }
            }
            else
            {
                mu.push_back( homogeneous_mu->getValue() );
            }

            if( psi_timeline != NULL )
            {
                psi.push_back( heterogeneous_psi->getValue()[serial_index] );

                if( serial_index < psi_timeline->getValue().size() && psi_timeline->getValue()[serial_index] == timeline[i] )
                {
                    serial_index++;
                }
            }
            else
            {
                psi.push_back( homogeneous_psi->getValue() );
            }

            if( rho_timeline != NULL )
            {
                rho.push_back( heterogeneous_rho->getValue()[fraction_index] );

                if( fraction_index < rho_timeline->getValue().size() && rho_timeline->getValue()[fraction_index] == timeline[i] )
                {
                    fraction_index++;
                }
            }
            else
            {
                if( i == timeline.size() - 1 )
                {
                    rho.push_back( homogeneous_rho->getValue() );
                }
                else
                {
                    rho.push_back(0.0);
                }
            }
        }
    }
}


/**
 * q_i(t)
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::q( size_t i, double t ) const
{
    
    if ( t == 0.0 ) 
    {
        return 1.0;
    }
    
    // get the parameters
    double b = lambda[i-1];
    double d = mu[i-1];
    double f = psi[i-1];
    double r = rho[i-1];
    double ti = timeline[i-1];

    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;
    
    double e = exp(-A*dt);
    double tmp = (1.0+B) + e*(1.0-B);
    
    return 4.0*e / (tmp*tmp);
}


double PiecewiseConstantSerialSampledBirthDeathProcess::getExtinctionRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_mu != NULL )
    {
        return homogeneous_mu->getValue();
    }
    else
    {
        if(index > heterogeneous_mu->getValue().size())
        {
            throw(RbException("Extinction rate index out of bounds"));
        }
        return heterogeneous_mu->getValue()[index];
    }
}


double PiecewiseConstantSerialSampledBirthDeathProcess::getSerialSamplingRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_psi != NULL )
    {
        return homogeneous_psi->getValue();
    }
    else
    {
        if(index > heterogeneous_psi->getValue().size())
        {
            throw(RbException("Serial sampling rate index out of bounds"));
        }
        return heterogeneous_psi->getValue()[index];
    }
}


double PiecewiseConstantSerialSampledBirthDeathProcess::getSpeciationRate( size_t index ) const
{

    // remove the old parameter first
    if ( homogeneous_lambda != NULL )
    {
        return homogeneous_lambda->getValue();
    }
    else
    {
        if(index > heterogeneous_lambda->getValue().size())
        {
            throw(RbException("Speciation rate index out of bounds"));
        }
        return heterogeneous_lambda->getValue()[index];
    }
}


double PiecewiseConstantSerialSampledBirthDeathProcess::getTaxonSamplingProbability( size_t index, size_t num_intervals ) const
{

    // remove the old parameter first
    if ( homogeneous_rho != NULL )
    {
        if( index == num_intervals - 1 )
        {
            return homogeneous_rho->getValue();
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(index > heterogeneous_rho->getValue().size())
        {
            throw(RbException("Taxon sampling fraction index out of bounds"));
        }
        return heterogeneous_rho->getValue()[index];
    }
}


/**
 * Simulate new speciation times.
 */
double PiecewiseConstantSerialSampledBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    // incorrect placeholder for constant FBDP
    // previous simSpeciations did not generate trees with defined likelihoods

    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = getSpeciationRate(0);
    double d = getExtinctionRate(0);
    double r = getTaxonSamplingProbability(0);

    
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
    
    
    return present - t;
}


/**
 * Compute the diversity of the tree at time t.
 *
 * \param[in]    t      time at which we want to know the diversity.
 *
 * \return The diversity (number of species in the reconstructed tree).
 */
int PiecewiseConstantSerialSampledBirthDeathProcess::survivors(double t) const
{

    const std::vector<TopologyNode*>& nodes = value->getNodes();
    
    int survivors = 0;
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); ++it) 
    {
        TopologyNode* n = *it;
        if ( n->getAge() < t ) 
        {
            if ( n->isRoot() || n->getParent().getAge() > t ) 
            {
                survivors++;
            } 
        } 
    }
    
    return survivors;
}


/**
 * Swap the parameters held by this distribution.
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantSerialSampledBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    if (oldP == heterogeneous_lambda)
    {
        heterogeneous_lambda = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_mu)
    {
        heterogeneous_mu = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_psi)
    {
        heterogeneous_psi = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == heterogeneous_rho)
    {
        heterogeneous_rho = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == homogeneous_lambda)
    {
        homogeneous_lambda = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_mu)
    {
        homogeneous_mu = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_psi)
    {
        homogeneous_psi = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_rho)
    {
        homogeneous_rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    else if (oldP == homogeneous_timeline)
    {
        homogeneous_timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == lambda_timeline)
    {
        lambda_timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == mu_timeline)
    {
        mu_timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == psi_timeline)
    {
        psi_timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else if (oldP == rho_timeline)
    {
        rho_timeline = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
}
