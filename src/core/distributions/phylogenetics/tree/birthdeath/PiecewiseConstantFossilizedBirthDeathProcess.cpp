#include "DistributionExponential.h"
#include "PiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathLogic.h"
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
PiecewiseConstantFossilizedBirthDeathProcess::PiecewiseConstantFossilizedBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                                           const DagNode *s,
                                                                                           const DagNode *e,
                                                                                           const DagNode *p,
                                                                                           const TypedDagNode<double> *r,
                                                                                           const TypedDagNode< RbVector<double> > *t,
                                                                                           bool uo,
                                                                                           const std::string &cdt,
                                                                                           const std::vector<Taxon> &tn ) : AbstractBirthDeathProcess( ra, cdt, tn ),
    homogeneous_rho(r), sampling_times( t ), useOrigin(uo)
{
    // initialize all the pointers to NULL
    homogeneous_lambda   = NULL;
    homogeneous_mu       = NULL;
    homogeneous_psi      = NULL;
    heterogeneous_lambda = NULL;
    heterogeneous_mu     = NULL;
    heterogeneous_psi    = NULL;

    const TypedDagNode<RbVector<double> > *tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(s);
    const TypedDagNode<double> *tmp_c = dynamic_cast<const TypedDagNode<double >*>(s);

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
        if(heterogeneous_lambda->getValue().size() != sampling_times->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of speciation rates (" << heterogeneous_lambda->getValue().size() << ") does not match number of time intervals (" << sampling_times->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        addParameter( heterogeneous_lambda );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(e);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(e);

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
        if(heterogeneous_mu->getValue().size() != sampling_times->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of extinction rates (" << heterogeneous_mu->getValue().size() << ") does not match number of time intervals (" << sampling_times->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        addParameter( heterogeneous_mu );
    }

    tmp_v = dynamic_cast<const TypedDagNode<RbVector<double> >*>(p);
    tmp_c = dynamic_cast<const TypedDagNode<double >*>(p);

    if(tmp_v == NULL && tmp_c == NULL)
    {
        throw(RbException("Fossil sampling rate must be of type RealPos or RealPos[]"));
    }
    else if(tmp_v == NULL)
    {
        homogeneous_psi = tmp_c;
        addParameter( homogeneous_psi );
    }
    else
    {
        heterogeneous_psi = tmp_v;
        if(heterogeneous_psi->getValue().size() != sampling_times->getValue().size() + 1)
        {
            std::stringstream ss;
            ss << "Number of fossil sampling rates (" << heterogeneous_psi->getValue().size() << ") does not match number of time intervals (" << sampling_times->getValue().size() + 1 << ")";
            throw(RbException(ss.str()));
        }

        addParameter( heterogeneous_psi );
    }

    addParameter( homogeneous_rho );
    addParameter( sampling_times );
    
    simulateTree();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
PiecewiseConstantFossilizedBirthDeathProcess* PiecewiseConstantFossilizedBirthDeathProcess::clone( void ) const
{
    return new PiecewiseConstantFossilizedBirthDeathProcess( *this );
}


/**
 * If conditioning on the origin, then return the age of the root node
 * or zero if the tree is empty
 */
double PiecewiseConstantFossilizedBirthDeathProcess::getRootAge( void ) const
{
    if(useOrigin)
    {
        if(value->getNumberOfNodes() > 0)
        {
            return value->getRoot().getAge();
        }
        else
        {
            return 0;
        }
    }
    else
        return getOriginTime();
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double PiecewiseConstantFossilizedBirthDeathProcess::computeLnProbabilityDivergenceTimes( void ) const
{
    // prepare the probability computation
    prepareProbComputation();

    // variable declarations and initialization
    double lnProbTimes = 0;

    // present time
    double present_time = value->getRoot().getAge();

    // what do we condition on?
    // did we condition on survival?
    if ( condition == "nTaxa" )
    {
        lnProbTimes = -lnProbNumTaxa( num_taxa, 0, present_time, true );
    }

    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeLnProbabilityTimes();

    return lnProbTimes;
}


/**
 * Compute the log probability of the current value under the current parameter values.
 * Tip-dating (Theorem 1, Stadler et al. 2013 PNAS)
 * Ancestral fossils will be considered in the future.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::computeLnProbabilityTimes( void ) const
{
    // variable declarations and initialization
    double lnProbTimes = 0.0;
    double process_time = getOriginTime();
    size_t num_initial_lineages = 0;
    TopologyNode* root = &value->getRoot();

    if (useOrigin) {
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
            if(!n.isRoot() || useOrigin)
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
        lnProbTimes += log( fossil[index - 1] ) + log( p(index, t) / q(index, t) );
    }
    
    // add the extant tip age term
    lnProbTimes += num_extant_taxa * log( homogeneous_rho->getValue() );

    // add the sampled ancestor age terms
    for (size_t i = 0; i < sampled_ancestor_ages.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = sampled_ancestor_ages[i];
        size_t index = l(t);
        lnProbTimes += log( fossil[index - 1] );
    }

    // add the single lineage propagation terms
    for (size_t i = 1; i <= times.size(); ++i)
    {
        if ( RbMath::isFinite(lnProbTimes) == false )
        {
            return RbConstants::Double::nan;
        }
        
        double t = times[i-1];
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
        lnProbTimes += log( 2.0 * q(index, t) * birth[index - 1] );
    }
    
    // add the initial age term
    lnProbTimes += num_initial_lineages * log( q(1, process_time ) );
    
    // condition on survival
    if ( condition == "survival" )
    {
        lnProbTimes -= num_initial_lineages * log( pSurvival(process_time,0) );
    }
    
    if ( RbMath::isFinite(lnProbTimes) == false )
    {
        return RbConstants::Double::nan;
    }

    // subtract the normalizing constant for the number of labeled trees
    for(size_t i = 2; i <= num_fossil_taxa + num_sampled_ancestors; i++)
    {
        lnProbTimes -= log(i);
    }

    for(size_t i = 2; i <= num_extant_taxa; i++)
    {
        lnProbTimes -= log(i);
    }


    return lnProbTimes;
}


/**
 * return the index i so that t_{i-1} > t >= t_i
 * where t_i is the instantaneous sampling time (i = 0,...,l)
 * t_0 is origin
 * t_l = 0.0
 */
size_t PiecewiseConstantFossilizedBirthDeathProcess::l(double t) const
{
    return times.rend() - std::lower_bound( times.rbegin(), times.rend(), t) + 1;
}


/**
 * Compute the probability of survival if the process starts with one species at time start and ends at time end.
 *
 * \param[in]    start      Start time of the process.
 * \param[in]    end        End/stopping time of the process.
 *
 * \return Probability of survival.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::pSurvival(double start, double end) const
{
    double t = start;
    
    std::vector<double> fossil_bak = fossil;

    std::fill(fossil.begin(), fossil.end(), 0.0);

    double p0 = p(l(t), t);
    
    fossil = fossil_bak;

    return 1.0 - p0;
}


/**
 * p_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::p( size_t i, double t ) const
{
    if ( t == 0)
        return 1.0;

    // get the parameters
    double b = birth[i-1];
    double d = death[i-1];
    double f = fossil[i-1];
    double r = (i == times.size() ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i-1];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;
    
    double e = exp(-A*dt);
    double tmp = b + d + f - A * (e*(1.0+B)-(1.0-B))/(e*(1.0+B)+(1.0-B));
    
    return tmp / (2.0*b);
}


/**
 *
 *
 */
void PiecewiseConstantFossilizedBirthDeathProcess::prepareProbComputation( void ) const
{
    // clean all the sets
    birth.clear();
    death.clear();
    fossil.clear();
    
    times = sampling_times->getValue();

    times.push_back(0.0);

    for (size_t i = 0; i < times.size(); i++)
    {
        birth.push_back( getSpeciationRate(i) );

        death.push_back( getExtinctionRate(i) );

        fossil.push_back( getFossilizationRate(i) );
    }
}


/**
 * q_i(t)
 */
double PiecewiseConstantFossilizedBirthDeathProcess::q( size_t i, double t ) const
{
    
    if ( t == 0.0 ) 
    {
        return 1.0;
    }
    
    // get the parameters
    double b = birth[i-1];
    double d = death[i-1];
    double f = fossil[i-1];
    double r = (i == times.size() ? homogeneous_rho->getValue() : 0.0);
    double ti = times[i-1];
    
    double diff = b - d - f;
    double bp   = b*f;
    double dt   = t - ti;
    
    double A = sqrt( diff*diff + 4.0*bp);
    double B = ( (1.0 - 2.0*(1.0-r)*p(i+1,ti) )*b + d + f ) / A;
    
    double e = exp(-A*dt);
    double tmp = ((1.0-B)+e*(1.0+B));
    
    return 4.0*e / (tmp*tmp);    
}


double PiecewiseConstantFossilizedBirthDeathProcess::getExtinctionRate( size_t index ) const
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


double PiecewiseConstantFossilizedBirthDeathProcess::getFossilizationRate( size_t index ) const
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
            throw(RbException("Fossil recovery rate index out of bounds"));
        }
        return heterogeneous_psi->getValue()[index];
    }
}


double PiecewiseConstantFossilizedBirthDeathProcess::getSpeciationRate( size_t index ) const
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


/**
 * Simulate new speciation times.
 */
double PiecewiseConstantFossilizedBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    // incorrect placeholder for constant FBDP
    // previous simSpeciations did not generate trees with defined likelihoods
    
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = getSpeciationRate(0);
    double d = getExtinctionRate(0);
    double r = homogeneous_rho->getValue();
    
    
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
int PiecewiseConstantFossilizedBirthDeathProcess::survivors(double t) const
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
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void PiecewiseConstantFossilizedBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{

    if ( affecter == root_age )
    {
        if( useOrigin )
        {
            if ( dag_node != NULL )
            {
                dag_node->touchAffected();
            }
        }
        else
        {
            AbstractRootedTreeDistribution::restoreSpecialization(affecter);
        }
    }

}


/**
 * Set the current value.
 */
void PiecewiseConstantFossilizedBirthDeathProcess::setValue(Tree *v, bool f )
{

    // delegate to super class
    TypedDistribution<Tree>::setValue(v, f);


    if ( root_age != NULL && !useOrigin )
    {
        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(root_age);
        if ( stoch_root_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
            value->getRoot().setAge( root_age->getValue() );
        }

    }

}


/**
 * Swap the parameters held by this distribution.
 * 
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void PiecewiseConstantFossilizedBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
    else if (oldP == sampling_times)
    {
        sampling_times = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    else
    {
        // delegate the super-class
        AbstractBirthDeathProcess::swapParameterInternal(oldP, newP);
    }
}


/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void PiecewiseConstantFossilizedBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{

    if ( affecter == root_age )
    {
        if( useOrigin )
        {
            if ( dag_node != NULL )
            {
                dag_node->touchAffected();
            }
        }
        else
        {
            AbstractRootedTreeDistribution::touchSpecialization(affecter, touchAll);
        }
    }

}
