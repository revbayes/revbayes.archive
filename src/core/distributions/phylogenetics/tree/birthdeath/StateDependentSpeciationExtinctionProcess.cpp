#include "AbstractCladogenicStateFunction.h"
#include "SSE_ODE.h"
#include "Clade.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "DeterministicNode.h"
#include "MatrixReal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "StandardState.h"
#include "StochasticNode.h"
#include "TopologyNode.h"

#include <algorithm>
#include <cmath>
#include <boost/assign/list_of.hpp>
#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 */
StateDependentSpeciationExtinctionProcess::StateDependentSpeciationExtinctionProcess(const TypedDagNode<double> *ra,
                                                                                   const TypedDagNode<RbVector<double> > *ext,
                                                                                   const TypedDagNode<RateGenerator>* q,
                                                                                   const TypedDagNode<double>* r,
                                                                                   const TypedDagNode< RbVector< double > >* p,
                                                                                   const TypedDagNode<double> *rh,
                                                                                   const std::string &cdt,
                                                                                   const std::vector<Taxon> &tn) : TypedDistribution<Tree>( new TreeDiscreteCharacterData() ),
    root_age( ra ),
    mu( ext ),
    pi( p ),
    Q( q ),
    rate( r ),
    rho( rh ),
    cladogenesis_matrix( NULL ),
    use_cladogenetic_events( false ),
    condition( cdt ),
    num_taxa( tn.size() ),
    active_likelihood( std::vector<size_t>(2*tn.size()-1, 0) ),
    changed_nodes( std::vector<bool>(2*tn.size()-1, false) ),
    dirty_nodes( std::vector<bool>(2*tn.size()-1, true) ),
    partial_likelihoods( std::vector<std::vector<std::vector<double> > >(2*tn.size()-1, std::vector<std::vector<double> >(2,std::vector<double>(2*ext->getValue().size(),0))) ),
    num_states( ext->getValue().size() ),
    NUM_TIME_SLICES( 500.0 ),
    scaling_factors( std::vector<std::vector<double> >(2*tn.size()-1, std::vector<double>(2,0.0) ) ),
    total_scaling( 0.0 ),
    extinction_probabilities( std::vector<std::vector<double> >( 500.0, std::vector<double>( ext->getValue().size(), 0) ) )
{
    addParameter( mu );
    addParameter( pi );
    addParameter( Q );
    addParameter( rho );
    addParameter( rate );
    addParameter( root_age );
    
    // set the length of the time slices used by the ODE for numerical integration
    dt = root_age->getValue() / NUM_TIME_SLICES;
    
    // the combinatorial factor for the probability of a labelled history is
    // 2^{n-1} / ( n! * (n-1)! )
    // but since the probability of the divergence times contains the factor (n-1)! we simply store
    // 2^{n-1} / n!
    double lnFact = 0.0;
    for (size_t i = 2; i <= num_taxa; ++i)
    {
        lnFact += std::log(i);
    }
    
    log_tree_topology_prob = (num_taxa - 1) * RbConstants::LN2 - lnFact ;
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
StateDependentSpeciationExtinctionProcess* StateDependentSpeciationExtinctionProcess::clone( void ) const
{
    return new StateDependentSpeciationExtinctionProcess( *this );
}



/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double StateDependentSpeciationExtinctionProcess::computeLnProbability( void )
{
    
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
    
    double ra = value->getRoot().getAge();
    
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
    
    // mark all nodes as dirty
    for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        const TopologyNode &the_node = *(*it);
        dirty_nodes[the_node.getIndex()] = true;
    }
    
    
    // variable declarations and initialization
    double lnProbTimes = 0;
    
    // conditioning on survival
    if ( condition == "survival" )
    {
        lnProbTimes = - 2*log( pSurvival(0, ra) );
    }
    
    // reset the total scaling
    total_scaling = 0.0;
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeRootLikelihood();
    
    return lnProbTimes + log_tree_topology_prob;
}


void StateDependentSpeciationExtinctionProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t node_index) const
{
    
    // check for recomputation
//    if ( dirty_nodes[node_index] == true )
    if ( true )
    {
        // mark as computed
        dirty_nodes[node_index] = false;
        
        state_type node_likelihood = std::vector<double>(2 * num_states, 0);
        if ( node.isTip() == true )
        {
            
            // this is a tip node
            double samplingProbability = rho->getValue();
            const DiscreteCharacterState &state = static_cast<TreeDiscreteCharacterData*>( this->value )->getCharacterData().getTaxonData( node.getTaxon().getName() )[0];
            const RbBitSet &obs_state = state.getState();
            
            for (size_t j = 0; j < num_states; ++j)
            {
                
                node_likelihood[j] = 1.0 - samplingProbability;
                
                if ( obs_state.isSet( j ) == true || state.isMissingState() == true || state.isGapState() == true )
                {
                    node_likelihood[num_states+j] = samplingProbability;
                }
                else
                {
                    node_likelihood[num_states+j] = 1.0 - samplingProbability;
                }
            }
            
        }
        else
        {
            
            // this is an internal node
            const TopologyNode          &left           = node.getChild(0);
            size_t                      left_index      = left.getIndex();
            computeNodeProbability( left, left_index );
            const TopologyNode          &right          = node.getChild(1);
            size_t                      right_index     = right.getIndex();
            computeNodeProbability( right, right_index );
            
            // get the likelihoods of descendant nodes
            const state_type &left_likelihoods = partial_likelihoods[left_index][active_likelihood[left_index]];
            const state_type &right_likelihoods = partial_likelihoods[right_index][active_likelihood[right_index]];

            std::map<std::vector<unsigned>, double> eventMap;
            std::vector<double> speciation_rates;
            if ( use_cladogenetic_events == true )
            {
                // get cladogenesis event map (sparse speciation rate matrix)
                const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
                const TypedFunction<MatrixReal>& tf = cpn->getFunction();
                const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
                
                eventMap = csf->getEventMap();
            }
            else
            {
                speciation_rates = lambda->getValue();
            }
            
            // merge descendant likelihoods
            for (size_t i=0; i<num_states; ++i)
            {
                node_likelihood[i] = left_likelihoods[i];

                if ( use_cladogenetic_events == true )
                {
                    
                    double like_sum = 0.0;
                    std::map<std::vector<unsigned>, double>::iterator it;
                    for (it = eventMap.begin(); it != eventMap.end(); it++)
                    {
                        const std::vector<unsigned>& states = it->first;
                        double speciation_rate = it->second;
                        if (i == states[0])
                        {
                            double likelihoods = left_likelihoods[num_states + states[1]] * right_likelihoods[num_states + states[2]];
                            like_sum += speciation_rate * likelihoods;
                        }
                    }
                    node_likelihood[num_states + i] = like_sum;
                    
                }
                else
                {
                    node_likelihood[num_states + i] = left_likelihoods[num_states + i] * right_likelihoods[num_states + i] * speciation_rates[i];
                }
            }
        }
        
        // calculate likelihood for this branch
        double begin_age = node.getAge();
        double end_age = node.getParent().getAge();
        numericallyIntegrateProcess(node_likelihood, begin_age, end_age, true, false);
        
        // rescale the states
        double max = 0.0;
        for (size_t i=0; i<num_states; ++i)
        {
            if ( node_likelihood[num_states+i] > max )
            {
                max = node_likelihood[num_states+i];
            }
        }
        for (size_t i=0; i<num_states; ++i)
        {
            node_likelihood[num_states+i] /= max;
        }
        scaling_factors[node_index][active_likelihood[node_index]] = log(max);
        total_scaling += scaling_factors[node_index][active_likelihood[node_index]] - scaling_factors[node_index][active_likelihood[node_index]^1];
        
        // store the likelihoods
        partial_likelihoods[node_index][active_likelihood[node_index]] = node_likelihood;

    }
    
}


double StateDependentSpeciationExtinctionProcess::computeRootLikelihood( void ) const
{
    
    std::map<std::vector<unsigned>, double> eventMap;
    std::vector<double> speciation_rates;
    if ( use_cladogenetic_events == true )
    {
        // get cladogenesis event map (sparse speciation rate matrix)
        const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
        const TypedFunction<MatrixReal>& tf = cpn->getFunction();
        const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
        
        eventMap = csf->getEventMap();
    }
    else
    {
        speciation_rates = lambda->getValue();
    }
    
    // get the likelihoods of descendant nodes
    const TopologyNode     &root            = value->getRoot();
    size_t                  node_index      = root.getIndex();
    const TopologyNode     &left            = root.getChild(0);
    size_t                  left_index      = left.getIndex();
    computeNodeProbability( left, left_index );
    const TopologyNode     &right           = root.getChild(1);
    size_t                  right_index     = right.getIndex();
    computeNodeProbability( right, right_index );

    // merge descendant likelihoods
    const state_type &left_likelihoods = partial_likelihoods[left_index][active_likelihood[left_index]];
    const state_type &right_likelihoods = partial_likelihoods[right_index][active_likelihood[right_index]];
    const RbVector<double> &freqs = pi->getValue();
    double prob = 0.0;
    state_type node_likelihood = std::vector<double>(2 * num_states, 0);
    for (size_t i = 0; i < num_states; ++i)
    {
        
        if ( use_cladogenetic_events == true )
        {
            
            double like_sum = 0.0;
            std::map<std::vector<unsigned>, double>::iterator it;
            for (it = eventMap.begin(); it != eventMap.end(); it++)
            {
                const std::vector<unsigned>& states = it->first;
                double speciation_rate = it->second;
                if (i == states[0])
                {
                    double likelihoods = left_likelihoods[num_states + states[1]] * right_likelihoods[num_states + states[2]];
                    like_sum += likelihoods * speciation_rate;
                }
            }
            // weight by root frequencies
            node_likelihood[i] = left_likelihoods[i];
            node_likelihood[num_states + i] = freqs[i] * like_sum;
            prob += freqs[i] * like_sum;

        }
        else
        {
            prob += freqs[i] * left_likelihoods[num_states + i] * right_likelihoods[num_states + i];
        }
        
    }
    
    // store the likelihoods
    partial_likelihoods[node_index][active_likelihood[node_index]] = node_likelihood;
    
    return log(prob) + total_scaling;
}



void StateDependentSpeciationExtinctionProcess::drawJointConditionalAncestralStates(std::vector<size_t>& startStates, std::vector<size_t>& endStates)
{
    // now begin the root-to-tip pass, drawing ancestral states conditional on the start states
    
    std::map<std::vector<unsigned>, double> eventMap;
    std::vector<double> speciation_rates;
    if ( use_cladogenetic_events == true )
    {
        // get cladogenesis event map (sparse speciation rate matrix)
        const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
        const TypedFunction<MatrixReal>& tf = cpn->getFunction();
        const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
        
        eventMap = csf->getEventMap();
    }
    else
    {
        speciation_rates = lambda->getValue();
    }
    
    // get the likelihoods of descendant nodes
    const TopologyNode  &root               = value->getRoot();
    size_t               node_index         = root.getIndex();
    const TopologyNode  &left               = root.getChild(0);
    size_t               left_index         = left.getIndex();
    const state_type    &left_likelihoods   = partial_likelihoods[left_index][active_likelihood[left_index]];
    const TopologyNode  &right              = root.getChild(1);
    size_t               right_index        = right.getIndex();
    const state_type    &right_likelihoods  = partial_likelihoods[right_index][active_likelihood[right_index]];
    
    // get root frequencies
    const RbVector<double> &freqs = pi->getValue();
    
    std::map<std::vector<unsigned>, double> sample_probs;
    double sample_probs_sum = 0.0;
    std::map<std::vector<unsigned>, double>::iterator it;
    
    // calculate probabilities for each state
    if ( use_cladogenetic_events == true )
    {
        // iterate over each cladogenetic event possible
        // and initialize probabilities for each clado event
        for (it = eventMap.begin(); it != eventMap.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            double speciation_rate = it->second;
            
            // we need to sample from the ancestor, left, and right states jointly,
            // so keep track of the probability of each clado event
            double likelihoods = left_likelihoods[num_states + states[1]] * right_likelihoods[num_states + states[2]];
            sample_probs[ states ] = likelihoods * freqs[states[0]] * speciation_rate;
            sample_probs_sum += likelihoods * freqs[states[0]] * speciation_rate;
        }
    }
    else
    {
        for (size_t i = 0; i < num_states; i++)
        {
            double likelihood = left_likelihoods[num_states + i] * right_likelihoods[num_states + i] * speciation_rates[i];
            std::vector<unsigned> states = boost::assign::list_of(i)(i)(i);
            sample_probs[ states ] = likelihood * freqs[i];
            sample_probs_sum += likelihood * freqs[i];
        }
    }
    
    // sample ancestor, left, and right character states from probs
    size_t a, l, r;
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    double u = rng->uniform01() * sample_probs_sum;
    
    for (it = sample_probs.begin(); it != sample_probs.end(); it++)
    {
        u -= it->second;
        if (u < 0.0)
        {
            const std::vector<unsigned>& states = it->first;
            a = states[0];
            l = states[1];
            r = states[2];
            endStates[node_index] = a;
            startStates[node_index] = a;
            startStates[left_index] = l;
            startStates[right_index] = r;
            break;
        }
    }
    
    // recurse towards tips
    recursivelyDrawJointConditionalAncestralStates(left, startStates, endStates);
    recursivelyDrawJointConditionalAncestralStates(right, startStates, endStates);
    
}


void StateDependentSpeciationExtinctionProcess::recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<size_t>& startStates, std::vector<size_t>& endStates)
{
    
    size_t node_index = node.getIndex();
    
    if ( node.isTip() == true )
    {
        const HomologousDiscreteCharacterData<StandardState>& data = static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
        const DiscreteTaxonData<StandardState>& taxon_data = data.getTaxonData( node.getName() );
        endStates[node_index] = taxon_data.getCharacter(0).getStateIndex();
    }
    else
    {
        // sample characters by their probability conditioned on the branch's start state going to end states
        
        // initialize the conditional likelihoods for this branch
        state_type branch_conditional_probs = std::vector<double>(2 * num_states, 0);
        size_t start_state = startStates[node_index];
        branch_conditional_probs[ num_states + start_state ] = 1.0;
        
        // first calculate extinction likelihoods via a backward time pass
        double end_age = node.getParent().getAge();
        numericallyIntegrateProcess(branch_conditional_probs, 0, end_age, true, true);
        
        // now calculate conditional likelihoods along branch in forward time
        end_age = node.getParent().getAge() - node.getAge();
        numericallyIntegrateProcess(branch_conditional_probs, 0, end_age, false, false);
        
        // TODO: if character mapping compute likelihoods for each time slice....
//        double current_time_slice = floor(begin_age / dt);
//        bool computed_at_least_one = false;
//        
//        // first iterate forward along the branch subtending this node to get the
//        // probabilities of the end states conditioned on the start state
//        while (current_time_slice * dt >= end_age || !computed_at_least_one)
//        {
//            double begin_age_slice = current_time_slice * dt;
//            double end_age_slice = (current_time_slice + 1) * dt;
//            numericallyIntegrateProcess(branch_conditional_probs, begin_age_slice, end_age_slice, false);
//            
//            computed_at_least_one = true;
//            current_time_slice--;
//        }
        
        std::map<std::vector<unsigned>, double> eventMap;
        std::vector<double> speciation_rates;
        if ( use_cladogenetic_events == true )
        {
            // get cladogenesis event map (sparse speciation rate matrix)
            const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
            const TypedFunction<MatrixReal>& tf = cpn->getFunction();
            const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
            
            eventMap = csf->getEventMap();
        }
        else
        {
            speciation_rates = lambda->getValue();
        }
        
        // get likelihoods of descendant nodes
        const TopologyNode &left = node.getChild(0);
        size_t left_index = left.getIndex();
        state_type left_likelihoods = partial_likelihoods[left_index][active_likelihood[left_index]];
        const TopologyNode &right = node.getChild(1);
        size_t right_index = right.getIndex();
        state_type right_likelihoods = partial_likelihoods[right_index][active_likelihood[right_index]];
        
        std::map<std::vector<unsigned>, double> sample_probs;
        double sample_probs_sum = 0.0;
        std::map<std::vector<unsigned>, double>::iterator it;

        // calculate probabilities for each state
        if ( use_cladogenetic_events == true )
        {
            // iterate over each cladogenetic event possible
            // and initialize probabilities for each clado event
            for (it = eventMap.begin(); it != eventMap.end(); it++)
            {
                const std::vector<unsigned>& states = it->first;
                double speciation_rate = it->second;
                
                // we need to sample from the ancestor, left, and right states jointly,
                // so keep track of the probability of each clado event
                double prob = left_likelihoods[num_states + states[1]] * right_likelihoods[num_states + states[2]];
                prob *= speciation_rate * branch_conditional_probs[num_states + states[0]];
                sample_probs[ states ] = prob;
                sample_probs_sum += prob;
            }
        }
        else
        {
            for (size_t i = 0; i < num_states; i++)
            {
                double prob = left_likelihoods[num_states + i] * right_likelihoods[num_states + i] * speciation_rates[i];
                prob *= branch_conditional_probs[num_states + i];
                std::vector<unsigned> states = boost::assign::list_of(i)(i)(i);
                sample_probs[ states ] = prob;
                sample_probs_sum += prob;
            }
        }
        
        // finally, sample ancestor, left, and right character states from probs
        size_t a, l, r;
        
        RandomNumberGenerator* rng = GLOBAL_RNG;
        double u = rng->uniform01() * sample_probs_sum;
        
        for (it = sample_probs.begin(); it != sample_probs.end(); it++)
        {
            u -= it->second;
            if (u < 0.0)
            {
                const std::vector<unsigned>& states = it->first;
                a = states[0];
                l = states[1];
                r = states[2];
                endStates[node_index] = a;
                startStates[left_index] = l;
                startStates[right_index] = r;
                break;
            }
        }
        
        // recurse towards tips
        recursivelyDrawJointConditionalAncestralStates(left, startStates, endStates);
        recursivelyDrawJointConditionalAncestralStates(right, startStates, endStates);
    }
    
}


void StateDependentSpeciationExtinctionProcess::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
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
void StateDependentSpeciationExtinctionProcess::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age)
    {
        dag_node->getAffectedNodes( affected );
    }
    
}



/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void StateDependentSpeciationExtinctionProcess::keepSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        dag_node->keepAffected();
    }
    
}



double StateDependentSpeciationExtinctionProcess::pSurvival(double start, double end) const
{
    
    double samplingProbability = rho->getValue();
    state_type initial_state = std::vector<double>(2*num_states,0);
    for (size_t i=0; i<num_states; ++i)
    {
        initial_state[i] = 1.0 - samplingProbability;
        initial_state[num_states + i] = samplingProbability;
    }
    
    numericallyIntegrateProcess(initial_state, start, end, true, false);
    
    double prob = 0.0;
    const RbVector<double> &freqs = pi->getValue();
    for (size_t i=0; i<num_states; ++i)
    {
        prob += freqs[i] * initial_state[i];
        //        prob += freqs[i]*(1.0-initial_state[i])*(1.0-initial_state[i])*speciation_rates[i];
        
    }
    
    return 1.0-prob;
    //    return prob;
}



/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void StateDependentSpeciationExtinctionProcess::redrawValue( void )
{
    
    simulateTree();
    
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void StateDependentSpeciationExtinctionProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dag_node->restoreAffected();
    }
    
}



void StateDependentSpeciationExtinctionProcess::setCladogenesisMatrix(const TypedDagNode< MatrixReal >* cm)
{
    
    // remove the old parameter first
    this->removeParameter( cladogenesis_matrix );
    
    // set the value
    cladogenesis_matrix = cm;
    
    // should we use the event map for the speciation rates?
    use_cladogenetic_events = true;
    
    // add the new parameter
    this->addParameter( cladogenesis_matrix );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


void StateDependentSpeciationExtinctionProcess::setSpeciationRates(const TypedDagNode< RbVector<double> >* r)
{
    
    // remove the old parameter first
    this->removeParameter( lambda );
    
    // set the value
    lambda = r;
    
    // should we use the event map for the speciation rates?
    use_cladogenetic_events = false;
    
    // add the new parameter
    this->addParameter( lambda );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


void StateDependentSpeciationExtinctionProcess::setNumberOfTimeSlices( double n )
{
    
    NUM_TIME_SLICES = n;
    
}


/**
 * Set the current value.
 */
void StateDependentSpeciationExtinctionProcess::setValue(Tree *v, bool f )
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
            value->getRoot().setAge( root_age->getValue() );
        }
        
    }
    
}


/**
 *
 */
void StateDependentSpeciationExtinctionProcess::simulateTree( void )
{
    
}



/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void StateDependentSpeciationExtinctionProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if ( oldP == root_age )
    {
        root_age = static_cast<const TypedDagNode<double>* >( newP );
    }
    if ( oldP == mu )
    {
        mu = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
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
    if ( oldP == cladogenesis_matrix )
    {
        cladogenesis_matrix = static_cast<const TypedDagNode<MatrixReal>* >( newP );
    }
    
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void StateDependentSpeciationExtinctionProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getRoot().setAge( root_age->getValue() );
        dag_node->touchAffected();
    }
    
}


/**
 * Wrapper function for the ODE time stepper function.
 */
void StateDependentSpeciationExtinctionProcess::numericallyIntegrateProcess(state_type &likelihoods, double begin_age, double end_age, bool backward_time, bool extinction_only) const
{
    const std::vector<double> &extinction_rates = mu->getValue();
    SSE_ODE ode = SSE_ODE(extinction_rates, &Q->getValue(), rate->getValue(), backward_time, extinction_only);
    if ( use_cladogenetic_events )
    {
        
        // get cladogenesis event map (sparse speciation rate matrix)
        const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
        const TypedFunction<MatrixReal>& tf = cpn->getFunction();
        const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
        std::map<std::vector<unsigned>, double> event_map = csf->getEventMap();
        
        ode.setEventMap( event_map );
    }
    else
    {
        const std::vector<double> &speciation_rates = lambda->getValue();
        ode.setSpeciationRate( speciation_rates );
    }

    boost::numeric::odeint::bulirsch_stoer< state_type > stepper(1E-8, 0.0, 0.0, 0.0);
    boost::numeric::odeint::integrate_adaptive( stepper, ode , likelihoods , begin_age , end_age, dt );
    
}
