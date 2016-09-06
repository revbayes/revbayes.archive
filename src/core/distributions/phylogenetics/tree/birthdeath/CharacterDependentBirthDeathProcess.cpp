#include "CDSE.h"
#include "Clade.h"
#include "CharacterDependentBirthDeathProcess.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StandardState.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>
#include <boost/numeric/odeint.hpp>

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
CharacterDependentBirthDeathProcess::CharacterDependentBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                         const TypedDagNode<RbVector<double> > *lo,
                                                                         const TypedDagNode<RbVector<double> > *lh,
                                                                         const TypedDagNode<RbVector<double> > *mo,
                                                                         const TypedDagNode<RbVector<double> > *mh,
                                                                         const TypedDagNode<RateGenerator>* q,
                                                                         const TypedDagNode<double>* r,
                                                       const TypedDagNode< RbVector< double > >* p,
                                                       const TypedDagNode<double> *rh,
                                                       const std::string &cdt,
                                                       const std::vector<Taxon> &tn) : TypedDistribution<Tree>( new TreeDiscreteCharacterData() ),
    root_age( ra ),
    lambda_unobserved( lh ),
    lambda_observed( lo ),
    mu_unobserved( mh ),
    mu_observed( mo ),
    pi( p ),
    Q( q ),
    rate( r ),
    rho( rh ),
    condition( cdt ),
    num_taxa( tn.size() ),
    taxa( tn ),
    active_likelihood( std::vector<size_t>(2*tn.size()-1, 0) ),
    changed_nodes( std::vector<bool>(2*tn.size()-1, false) ),
    dirty_nodes( std::vector<bool>(2*tn.size()-1, true) ),
    node_states( std::vector<std::vector<state_type> >(2*tn.size()-1, std::vector<state_type>(2,std::vector<double>(2*lh->getValue().size()*lo->getValue().size(),0))) ),
    num_rate_categories( lh->getValue().size() * lo->getValue().size() ),
    num_observed_states( lo->getValue().size() ),
    num_hidden_states( lh->getValue().size() ),
    scaling_factors( std::vector<std::vector<double> >(2*tn.size()-1, std::vector<double>(2,0.0) ) ),
    total_scaling( 0.0 ),
    NUM_TIME_SLICES( 200.0 )
{
    
    addParameter( lambda_unobserved );
    addParameter( lambda_observed );
    addParameter( mu_observed );
    addParameter( mu_unobserved );
    addParameter( pi );
    addParameter( Q );
    addParameter( rho );
    addParameter( rate );
    
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= num_taxa; i++)
    {
        lnFact += std::log(i);
    }
    
    log_tree_topology_prob = (num_taxa - 1) * RbConstants::LN2 - lnFact ;
    log_tree_topology_prob = 0;
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
CharacterDependentBirthDeathProcess* CharacterDependentBirthDeathProcess::clone( void ) const
{
    return new CharacterDependentBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double CharacterDependentBirthDeathProcess::computeLnProbability( void )
{
    
    prepareProbComputation();
    
    // check that the ages are in correct chronological order
    // i.e., no child is older than its parent
    const std::vector<TopologyNode*>& nodes = value->getNodes();
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isRoot() == false )
        {
            
            if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( (the_node.getAge() - (*it)->getParent().getAge()) > 1E-6 && the_node.isSampledAncestor() == true )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // check that the sampled ancestor nodes have a zero branch length
    for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        const TopologyNode &the_node = *(*it);
        if ( the_node.isSampledAncestor() == true )
        {
            
            if ( the_node.isFossil() == false )
            {
                return RbConstants::Double::neginf;
            }
            else if ( the_node.getBranchLength() > 1E-6 )
            {
                return RbConstants::Double::neginf;
            }
            
        }
        
    }
    
    // present time
    double ra = value->getRoot().getAge();
    double present_time =  ra;
        
    if ( ra != root_age->getValue() )
    {
        return RbConstants::Double::neginf;
    }
        
    const std::vector<TopologyNode*> &c = value->getRoot().getChildren();
    
    for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        if ( ra < (*it)->getAge() )
        {
            return RbConstants::Double::neginf;
        }
    }
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // what do we condition on?
    // did we condition on survival?
    if ( condition == "survival" )
    {
        lnProbTimes = - 2*log( pSurvival(0,present_time) );
    }
    else if ( condition != "time")
    {
        throw RbException("Only conditioning on survival possible or time possible");
    }
    
    total_scaling = 0.0;
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeRootLikelihood();
    
    
    return lnProbTimes + log_tree_topology_prob;
}


void CharacterDependentBirthDeathProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t nodeIndex) const
{
    
    // check for recomputation
    if ( dirty_nodes[nodeIndex] || true )
    {
        // mark as computed
        dirty_nodes[nodeIndex] = false;
        
        state_type initial_state = std::vector<double>(2*num_rate_categories,0);
        if ( node.isTip() )
        {
            // this is a tip node
            
            double samplingProbability = rho->getValue();
            const DiscreteCharacterState &state = static_cast<TreeDiscreteCharacterData*>( this->value )->getCharacterData().getTaxonData( node.getTaxon().getName() )[0];
            unsigned long obs_state = state.getState();
            for (size_t i=0; i<num_hidden_states; ++i)
            {
                unsigned long val = obs_state;
                for (size_t j=0; j<num_observed_states; ++j)
                {
                    
                    initial_state[i+j] = 1.0 - samplingProbability;

                    if ( (val & 1) == 1 || state.isMissingState() == true || state.isGapState() == true )
                    {
                        initial_state[num_rate_categories+i+j] = samplingProbability;
                    }
                    else
                    {
                        initial_state[num_rate_categories+i+j] = 0.0;
                    }
                    
                    // remove this state from the observed states
                    val >>= 1;
                    
                }
            }
            
        }
        else
        {
            // this is an internal node
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            computeNodeProbability( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            computeNodeProbability( right, rightIndex );
            
            // now compute the likelihoods of this internal node
            const state_type &leftStates = node_states[leftIndex][active_likelihood[leftIndex]];
            const state_type &rightStates = node_states[rightIndex][active_likelihood[rightIndex]];
            for (size_t i=0; i<num_rate_categories; ++i)
            {
                initial_state[i] = leftStates[i];
                initial_state[num_rate_categories+i] = leftStates[num_rate_categories+i]*rightStates[num_rate_categories+i]*speciation_rates[i];
            }
            
        }
        
        CDSE ode = CDSE( speciation_rates, extinction_rates, &Q->getValue(), rate->getValue());
        double beginAge = node.getAge();
        double endAge = node.getParent().getAge();
        double dt = root_age->getValue() / NUM_TIME_SLICES;
        boost::numeric::odeint::runge_kutta4< state_type > stepper;
        boost::numeric::odeint::integrate_const( stepper, ode , initial_state , beginAge , endAge, dt );
        
        // rescale the states
        double max = 0.0;
        for (size_t i=0; i<num_rate_categories; ++i)
        {
            if ( initial_state[num_rate_categories+i] > max )
            {
                max = initial_state[num_rate_categories+i];
            }
        }
        for (size_t i=0; i<num_rate_categories; ++i)
        {
            initial_state[num_rate_categories+i] /= max;
        }
        scaling_factors[nodeIndex][active_likelihood[nodeIndex]] = log(max);
        total_scaling += scaling_factors[nodeIndex][active_likelihood[nodeIndex]] - scaling_factors[nodeIndex][active_likelihood[nodeIndex]^1];
        
        // store the states
        node_states[nodeIndex][active_likelihood[nodeIndex]] = initial_state;
    }
    
}


double CharacterDependentBirthDeathProcess::computeRootLikelihood( void ) const
{
    
    const TopologyNode &root = value->getRoot();
    
    // fill the like
    const TopologyNode &left = root.getChild(0);
    size_t leftIndex = left.getIndex();
    computeNodeProbability( left, leftIndex );
    const TopologyNode &right = root.getChild(1);
    size_t rightIndex = right.getIndex();
    computeNodeProbability( right, rightIndex );
    
    
    // now compute the likelihoods of this internal node
    state_type leftStates = node_states[leftIndex][active_likelihood[leftIndex]];
    state_type rightStates = node_states[rightIndex][active_likelihood[rightIndex]];
    const RbVector<double> &freqs = pi->getValue();
    double prob = 0.0;
    for (size_t i=0; i<num_rate_categories; ++i)
    {

        prob += freqs[i]*leftStates[num_rate_categories+i]*rightStates[num_rate_categories+i];

    }
  
    
    return log(prob) + total_scaling;
}


void CharacterDependentBirthDeathProcess::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
{
    
    if (name == "clampCharData")
    {
        found = true;
        
        const HomologousDiscreteCharacterData<StandardState>& v     = static_cast<const TypedDagNode<HomologousDiscreteCharacterData<StandardState> > *>( args[0] )->getValue();
        
        static_cast<TreeDiscreteCharacterData*>(this->value)->setCharacterData( v );
    }
    
    return TypedDistribution<Tree>::executeProcedure( name, args, found );
}


/**
 * Get the affected nodes by a change of this node.
 * If the root age has changed than we need to call get affected again.
 */
void CharacterDependentBirthDeathProcess::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age)
    {
        dag_node->getAffectedNodes( affected );
    }
    
}



/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void CharacterDependentBirthDeathProcess::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        dag_node->keepAffected();
    }
    
}
/**
 *
 *
 */
void CharacterDependentBirthDeathProcess::prepareProbComputation( void ) const
{
    
    speciation_rates.clear();
    extinction_rates.clear();
    
    for (size_t i=0; i<num_hidden_states; ++i)
    {
        for (size_t j=0; j<num_observed_states; ++j)
        {
            double sp_rate = lambda_unobserved->getValue()[i];
            sp_rate += lambda_observed->getValue()[j];
            speciation_rates.push_back( exp(sp_rate) );
            
            double ex_rate = mu_unobserved->getValue()[i];
            ex_rate += mu_observed->getValue()[j];
            extinction_rates.push_back( exp(ex_rate) );
        }
        
    }
    
}




double CharacterDependentBirthDeathProcess::pSurvival(double start, double end) const
{
    
    double samplingProbability = rho->getValue();
    state_type initial_state = std::vector<double>(2*num_rate_categories,0);
    for (size_t i=0; i<num_rate_categories; ++i)
    {
        initial_state[i] = 1.0 - samplingProbability;
        initial_state[num_rate_categories+i] = samplingProbability;
    }
    
    double dt = root_age->getValue() / NUM_TIME_SLICES;
    CDSE ode = CDSE( speciation_rates, extinction_rates, &Q->getValue(), rate->getValue());
    boost::numeric::odeint::integrate( ode , initial_state , start , end , dt );
    
    
    double prob = 0.0;
    const RbVector<double> &freqs = pi->getValue();
    for (size_t i=0; i<num_rate_categories; ++i)
    {
        prob += freqs[i]*initial_state[i];
//        prob += freqs[i]*(1.0-initial_state[i])*(1.0-initial_state[i])*speciation_rates[i];
        
    }
    
    
    return 1.0-prob;
//    return prob;
}


/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void CharacterDependentBirthDeathProcess::redrawValue( void )
{
    
    simulateTree();
    
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void CharacterDependentBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dag_node->restoreAffected();
    }
    
}


/**
 * Set the current value.
 */
void CharacterDependentBirthDeathProcess::setValue(Tree *v, bool f )
{
    
    // delegate to super class
//    TypedDistribution<Tree>::setValue(v, f);
    static_cast<TreeDiscreteCharacterData *>(this->value)->setTree( *v );
    delete v;
    
    
    if ( root_age != NULL )
    {
        const StochasticNode<double> *stoch_root_age = dynamic_cast<const StochasticNode<double>* >(root_age);
        if ( stoch_root_age != NULL )
        {
            const_cast<StochasticNode<double> *>(stoch_root_age)->setValue( new double( value->getRoot().getAge() ), f);
        }
        else
        {
            //            double factor = root_age->getValue() / value->getRoot().getAge();
            //            TreeUtilities::rescaleTree( value, &value->getRoot(), factor);
            
            value->getRoot().setAge( root_age->getValue() );
        }
        
    }
    
}


/**
 *
 */
void CharacterDependentBirthDeathProcess::simulateTree( void )
{
    
}



/**
 * Simulate new speciation times.
 */
double CharacterDependentBirthDeathProcess::simulateDivergenceTime(double origin, double present) const
{
    
    // Get the rng
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get the parameters
    double age = present - origin;
    double b = lambda_observed->getValue()[0];
    double d = mu_observed->getValue()[0];
    double rho = 1.0;
    
    // get a random draw
    double u = rng->uniform01();
    
    // compute the time for this draw
    double t = ( log( ( (b-d) / (1 - (u)*(1-((b-d)*exp((d-b)*age))/(rho*b+(b*(1-rho)-d)*exp((d-b)*age) ) ) ) - (b*(1-rho)-d) ) / (rho * b) ) + (d-b)*age )  /  (d-b);
    
    
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
void CharacterDependentBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == root_age )
    {
        root_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == lambda_unobserved )
    {
        lambda_unobserved = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == lambda_observed )
    {
        lambda_observed = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == mu_unobserved )
    {
        mu_unobserved = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == mu_observed )
    {
        mu_observed = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == Q )
    {
        Q = static_cast<const TypedDagNode<RateGenerator>* >( newP );
    }
    if ( oldP == rate )
    {
        rate = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == pi )
    {
        pi = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    
    if ( oldP == rho )
    {
        rho = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void CharacterDependentBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getRoot().setAge( root_age->getValue() );
        dag_node->touchAffected();
    }
    
}
