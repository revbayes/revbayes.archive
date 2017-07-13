#include "AbstractCladogenicStateFunction.h"
#include "AbstractHomologousDiscreteCharacterData.h"
#include "SSE_ODE.h"
#include "Clade.h"
#include "StateDependentSpeciationExtinctionProcess.h"
#include "DeterministicNode.h"
#include "MatrixReal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RateMatrix_JC.h"
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
                                                                                   const TypedDagNode< Simplex >* p,
                                                                                   const TypedDagNode<double> *rh,
                                                                                   const std::string &cdt,
                                                                                   const std::vector<Taxon> &tn) : TypedDistribution<Tree>( new TreeDiscreteCharacterData() ),
    condition( cdt ),
    num_taxa( tn.size() ),
    active_likelihood( std::vector<size_t>(2*tn.size()-1, 0) ),
    changed_nodes( std::vector<bool>(2*tn.size()-1, false) ),
    dirty_nodes( std::vector<bool>(2*tn.size()-1, true) ),
    node_partial_likelihoods( std::vector<std::vector<std::vector<double> > >(2*tn.size()-1, std::vector<std::vector<double> >(2,std::vector<double>(2*ext->getValue().size(),0))) ),
    extinction_probabilities( std::vector<std::vector<double> >( 500.0, std::vector<double>( ext->getValue().size(), 0) ) ),
    num_states( ext->getValue().size() ),
    scaling_factors( std::vector<std::vector<double> >(2*tn.size()-1, std::vector<double>(2,0.0) ) ),
    total_scaling( 0.0 ),
    use_cladogenetic_events( false ),
    sample_character_history( false ),
    cladogenesis_matrix( NULL ),
    root_age( ra ),
    mu( ext ),
    lambda(NULL),
    psi( NULL),
    pi( p ),
    Q( q ),
    rate( r ),
    rho( rh ),
    Q_default( ext->getValue().size() ),
    NUM_TIME_SLICES( 500.0 )

{
    addParameter( mu );
    addParameter( pi );
    addParameter( Q );
    addParameter( rho );
    addParameter( rate );
    addParameter( root_age );
    
    // set the length of the time slices used by the ODE for numerical integration
    dt = root_age->getValue() / NUM_TIME_SLICES * 50.0;
    
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
            else if ( (the_node.getAge() - (*it)->getParent().getAge()) > 0 && the_node.isSampledAncestor() == true )
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
            else if ( the_node.getBranchLength() > 0 )
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
        
        std::vector<double> node_likelihood = std::vector<double>(2 * num_states, 0);
        if ( node.isTip() == true )
        {
            // this is a tip node
            TreeDiscreteCharacterData* tree = static_cast<TreeDiscreteCharacterData*>( this->value );

            std::vector<double> sampling(num_states, rho->getValue());
            std::vector<double> extinction(num_states, 1.0 - rho->getValue());

            if(psi != NULL && node.isFossil())
            {
                sampling = psi->getValue();
                extinction = pExtinction(0.0, node.getAge());
            }
            
            RbBitSet obs_state(num_states, true);
            bool gap = true;

            if(tree->hasCharacterData())
            {
                const DiscreteCharacterState &state = tree->getCharacterData().getTaxonData( node.getTaxon().getName() )[0];
                obs_state = state.getState();
                gap = (state.isMissingState() == true || state.isGapState() == true);
            }

            for (size_t j = 0; j < num_states; ++j)
            {
                
                node_likelihood[j] = extinction[j];
                
                if ( obs_state.isSet( j ) == true || gap )
                {
                    node_likelihood[num_states+j] = sampling[j];
                }
                else
                {
                    node_likelihood[num_states+j] = 0.0;
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
            const std::vector<double> &left_likelihoods  = node_partial_likelihoods[left_index][active_likelihood[left_index]];
            const std::vector<double> &right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];

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
            
            bool speciation_node = true;
            if( left.isSampledAncestor() || right.isSampledAncestor() )
            {
                speciation_node = psi == NULL;
            }

            // merge descendant likelihoods
            for (size_t i=0; i<num_states; ++i)
            {
                node_likelihood[i] = left_likelihoods[i];

                if ( use_cladogenetic_events == true && speciation_node == true )
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
                    node_likelihood[num_states + i] = left_likelihoods[num_states + i] * right_likelihoods[num_states + i];
                    node_likelihood[num_states + i] *= speciation_node ? speciation_rates[i] : 1.0;
                }
            }
        }
        
        double begin_age = node.getAge();
        double end_age = node.getParent().getAge();
        
        if ( node.isSampledAncestor() == false )
        {
            // calculate likelihoods for this branch
            if ( sample_character_history == false )
            {
                // numerically integrate over the entire branch length
                numericallyIntegrateProcess(node_likelihood, begin_age, end_age, true, false);
            }
            else
            {
                // calculate the conditional likelihoods for each time slice moving
                // along this branch backwards in time from the tip towards the root

                std::vector<std::vector<double> > branch_likelihoods;
                size_t current_dt = 0;
                
                // calculate partial likelihoods for each time slice and store them in branch_likelihoods
                while ( (current_dt * dt) + begin_age < end_age )
                {

                    std::vector<double> dt_likelihood;

                    double current_dt_start = (current_dt * dt) + begin_age;
                    double current_dt_end = ((current_dt + 1) * dt) + begin_age;
                    if (current_dt_end > end_age)
                    {
                        current_dt_end = end_age;
                    }
                    numericallyIntegrateProcess(node_likelihood, current_dt_start, current_dt_end, true, false);

                    std::vector<double>::const_iterator first = node_likelihood.begin() + num_states;
                    std::vector<double>::const_iterator last = node_likelihood.begin() + (num_states * 2);
                    dt_likelihood = std::vector<double>(first, last);

                    branch_likelihoods.push_back(dt_likelihood);
                    current_dt++;

                }
                
                // save the branch conditional likelihoods
                branch_partial_likelihoods[node_index] = branch_likelihoods;
            }
        }
        
        // rescale the conditional likelihoods at the "end" of the branch
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
        
        // store the likelihoods for this node
        node_partial_likelihoods[node_index][active_likelihood[node_index]] = node_likelihood;

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

    bool speciation_node = true;
    if( left.isSampledAncestor() || right.isSampledAncestor() )
    {
        speciation_node = psi == NULL;
    }

    // merge descendant likelihoods
    const std::vector<double> &left_likelihoods  = node_partial_likelihoods[left_index][active_likelihood[left_index]];
    const std::vector<double> &right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];
    const RbVector<double> &freqs = getRootFrequencies();
    double prob = 0.0;
    state_type node_likelihood = std::vector<double>(2 * num_states, 0);
    for (size_t i = 0; i < num_states; ++i)
    {
        
        if ( use_cladogenetic_events == true && speciation_node == true )
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
    node_partial_likelihoods[node_index][active_likelihood[node_index]] = node_likelihood;
    
    return log(prob) + total_scaling;
}

const AbstractHomologousDiscreteCharacterData& StateDependentSpeciationExtinctionProcess::getCharacterData() const
{
    return static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
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
    const state_type    &left_likelihoods   = node_partial_likelihoods[left_index][active_likelihood[left_index]];
    const TopologyNode  &right              = root.getChild(1);
    size_t               right_index        = right.getIndex();
    const state_type    &right_likelihoods  = node_partial_likelihoods[right_index][active_likelihood[right_index]];
    
    // get root frequencies
    const RbVector<double> &freqs = getRootFrequencies();
    
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
            prob *= freqs[states[0]] * speciation_rate;
            sample_probs[ states ] = prob;
            sample_probs_sum += prob;
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
    size_t a = 0, l = 0, r = 0;
    
    if (sample_probs_sum == 0)
    {
        RandomNumberGenerator* rng = GLOBAL_RNG;
        size_t u = rng->uniform01() * sample_probs.size();
        size_t v = 0;
        for (it = sample_probs.begin(); it != sample_probs.end(); it++)
        {
            if (u < v)
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
             v++;
         }
    }
    else
    {
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
        const AbstractHomologousDiscreteCharacterData& data = static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
        const AbstractDiscreteTaxonData& taxon_data = data.getTaxonData( node.getName() );
        
        const DiscreteCharacterState &char_state = taxon_data.getCharacter(0);
        
        // we need to treat ambiguous state differently
        if ( char_state.isAmbiguous() == false )
        {
            endStates[node_index] = char_state.getStateIndex();
        }
        else
        {
            // initialize the conditional likelihoods for this branch
            state_type branch_conditional_probs = std::vector<double>(2 * num_states, 0);
            size_t start_state = startStates[node_index];
            branch_conditional_probs[ num_states + start_state ] = 1.0;
            
            // first calculate extinction likelihoods via a backward time pass
            double end_age = node.getParent().getAge();
            numericallyIntegrateProcess(branch_conditional_probs, 0, end_age, true, true);
            
            // now calculate conditional likelihoods along branch in forward time
            end_age        = node.getParent().getAge() - node.getAge();
            numericallyIntegrateProcess(branch_conditional_probs, 0, end_age, false, false);
            
            double total_prob = 0.0;
            for (size_t i = 0; i < num_states; ++i)
            {
                if ( char_state.isMissingState() == true || char_state.isGapState() == true || char_state.isStateSet(i) == true )
                {
                    total_prob += branch_conditional_probs[i];
                }
            }
            
            RandomNumberGenerator* rng = GLOBAL_RNG;
            size_t u = rng->uniform01() * total_prob;
            
            for (size_t i = 0; i < num_states; ++i)
            {
                
                if ( char_state.isMissingState() == true || char_state.isGapState() == true || char_state.isStateSet(i) == true )
                {
                    u -= branch_conditional_probs[i];
                    if ( u <= 0.0 )
                    {
                        endStates[node_index] = i;
                        break;
                    }
                    
                }
                
            }
            
        }
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
        end_age        = node.getParent().getAge() - node.getAge();
        numericallyIntegrateProcess(branch_conditional_probs, 0, end_age, false, false);
        
        std::map<std::vector<unsigned>, double> event_map;
        std::vector<double> speciation_rates;
        if ( use_cladogenetic_events == true )
        {
            // get cladogenesis event map (sparse speciation rate matrix)
            const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
            const TypedFunction<MatrixReal>& tf = cpn->getFunction();
            const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
            
            event_map = csf->getEventMap();
        }
        else
        {
            speciation_rates = lambda->getValue();
        }
        
        // get likelihoods of descendant nodes
        const TopologyNode &left = node.getChild(0);
        size_t left_index = left.getIndex();
        state_type left_likelihoods = node_partial_likelihoods[left_index][active_likelihood[left_index]];
        const TopologyNode &right = node.getChild(1);
        size_t right_index = right.getIndex();
        state_type right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];
        
        std::map<std::vector<unsigned>, double> sample_probs;
        double sample_probs_sum = 0.0;
        std::map<std::vector<unsigned>, double>::iterator it;

        // calculate probabilities for each state
        if ( use_cladogenetic_events == true )
        {
            // iterate over each cladogenetic event possible
            // and initialize probabilities for each clado event
            for (it = event_map.begin(); it != event_map.end(); it++)
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
        size_t a = 0, l = 0, r = 0;

        if (sample_probs_sum == 0)
        {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            size_t u = rng->uniform01() * sample_probs.size();
            size_t v = 0;
            for (it = sample_probs.begin(); it != sample_probs.end(); it++)
            {
                if (u < v)
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
                 v++;
             }
        }
        else
        {
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
        }
        
        // recurse towards tips
        recursivelyDrawJointConditionalAncestralStates(left, startStates, endStates);
        recursivelyDrawJointConditionalAncestralStates(right, startStates, endStates);
    }
    
}


void StateDependentSpeciationExtinctionProcess::drawStochasticCharacterMap(std::vector<std::string*>& character_histories)
{
    // first populate partial likelihood vectors along all the branches
    sample_character_history = true;
    computeLnProbability();

    // now begin the root-to-tip pass, drawing ancestral states for each time slice conditional on the start states
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
    const state_type    &left_likelihoods   = node_partial_likelihoods[left_index][active_likelihood[left_index]];
    const TopologyNode  &right              = root.getChild(1);
    size_t               right_index        = right.getIndex();
    const state_type    &right_likelihoods  = node_partial_likelihoods[right_index][active_likelihood[right_index]];
    
    // get root frequencies
    const RbVector<double> &freqs = getRootFrequencies();
    
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
            prob *= freqs[states[0]] * speciation_rate;
            sample_probs[ states ] = prob;
            sample_probs_sum += prob;
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
    size_t a = 0, l = 0, r = 0;
    
    if (sample_probs_sum == 0)
    {
        RandomNumberGenerator* rng = GLOBAL_RNG;
        size_t u = rng->uniform01() * sample_probs.size();
        size_t v = 0;
        for (it = sample_probs.begin(); it != sample_probs.end(); it++)
        {
            if (u < v)
            {
                const std::vector<unsigned>& states = it->first;
                a = states[0];
                l = states[1];
                r = states[2];
                break;
            }
            v++;
        }
    }
    else
    {
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
                break;
            }
        }
    }
    
    // save the character history for the root
    std::string* simmap_string = new std::string("{" + StringUtilities::toString(a) + "," + StringUtilities::toString( root.getBranchLength() ) + "}");
    character_histories[node_index] = simmap_string;
    
    // recurse towards tips
    recursivelyDrawStochasticCharacterMap(left, l, character_histories);
    recursivelyDrawStochasticCharacterMap(right, r, character_histories);
    
    // turn off sampling until we need it again
    sample_character_history = false;
}


void StateDependentSpeciationExtinctionProcess::recursivelyDrawStochasticCharacterMap(const TopologyNode &node, size_t start_state, std::vector<std::string*>& character_histories)
{
    
    size_t node_index = node.getIndex();
    
    // sample characters by their probability conditioned on the branch's start state going to end states
    
    // initialize the conditional likelihoods for this branch
    state_type branch_conditional_probs = std::vector<double>(2 * num_states, 0);
    branch_conditional_probs[ num_states + start_state ] = 1.0;
    
    // first calculate extinction likelihoods via a backward time pass
    double start_time = node.getParent().getAge();
    numericallyIntegrateProcess(branch_conditional_probs, 0, start_time, true, true);
    
    // now calculate conditional likelihoods along branch in forward time
    double branch_length = node.getParent().getAge() - node.getAge();
    size_t current_dt = 0;
    double current_dt_start = 0;
    double current_dt_end = 0;
    
    size_t current_state = start_state;
    
    // set up vectors to hold the transition events
    std::vector<size_t> transition_states;
    std::vector<double> transition_times;
    transition_states.push_back(current_state);
    
    int downpass_dt = int( branch_partial_likelihoods[node_index].size() ) - 1;
    
    // loop over every time slice, stopping before the last time slice
    while ( downpass_dt >= 0 && ((current_dt + 1) * dt) < branch_length)
    {
        current_dt_start = (current_dt * dt);
        current_dt_end = ((current_dt + 1) * dt);
        
        numericallyIntegrateProcess(branch_conditional_probs, current_dt_start, current_dt_end, false, false);

        // draw state for this time slice
        size_t new_state = current_state;
        double probs_sum = 0.0;
        for (size_t i = 0; i < num_states; i++)
        {
            probs_sum += branch_conditional_probs[i + num_states] * branch_partial_likelihoods[node_index][downpass_dt][i];
        }
        
        if ( probs_sum == 0.0 )
        {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            double u = rng->uniform01() * num_states;
            new_state = size_t(u);
        }
        else
        {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            double u = rng->uniform01() * probs_sum;

            for (size_t i = 0; i < num_states; i++)
            {
                u -= branch_conditional_probs[i + num_states] * branch_partial_likelihoods[node_index][downpass_dt][i];
                if (u < 0.0)
                {
                    new_state = i;
                    break;
                }
            }
        }
        
        // check if there was a character state transition
        if (new_state != current_state)
        {
            double time_since_last_transition = 0.0;
            double transition_times_sum = 0.0;
            for (size_t j = 0; j < transition_times.size(); j++)
            {
                transition_times_sum += transition_times[j];
            }
            time_since_last_transition = current_dt_end - transition_times_sum;

            transition_times.push_back(time_since_last_transition);
            transition_states.push_back(new_state);
            current_state = new_state;
        }
        
        // condition branch_conditional_probs on the sampled state
        for (size_t i = 0; i < num_states; i++)
        {
            if (i == current_state)
            {
                branch_conditional_probs[ num_states + i ] = 1.0;
            }
            else
            {
                branch_conditional_probs[ num_states + i ] = 0.0;
            }
        }
        
        current_dt++;
        downpass_dt--;
    }
    
    if ( node.isTip() == true )
    {
        // the last time slice of the branch will be the observed state
        
        const AbstractHomologousDiscreteCharacterData& data = static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
        const AbstractDiscreteTaxonData& taxon_data = data.getTaxonData( node.getName() );
        
        const DiscreteCharacterState &char_state = taxon_data.getCharacter(0);
        size_t new_state = current_state;
        
        if ( char_state.isAmbiguous() == false )
        {
            new_state = char_state.getStateIndex();
        }
        else
        {
            // use the simulated state
            new_state = current_state;
        }
        
        // check if there was a character state transition
        if (new_state != current_state)
        {
            double time_since_last_transition = 0.0;
            double transition_times_sum = 0.0;
            for (size_t j = 0; j < transition_times.size(); j++)
            {
                transition_times_sum += transition_times[j];
            }
            time_since_last_transition = current_dt_end - transition_times_sum;
            
            transition_times.push_back(time_since_last_transition);
            transition_states.push_back(new_state);
        }
        
        // add the length of the final character state
        double time_since_last_transition = 0.0;
        double transition_times_sum = 0.0;
        for (size_t j = 0; j < transition_times.size(); j++)
        {
            transition_times_sum += transition_times[j];
        }
        time_since_last_transition = branch_length - transition_times_sum;
        transition_times.push_back(time_since_last_transition);
        
        // make SIMMAP string
        std::string simmap_string = "{";
        for (size_t i = transition_times.size(); i > 0; i--)
        {
            simmap_string = simmap_string + StringUtilities::toString(transition_states[i - 1]) + "," + StringUtilities::toString(transition_times[i - 1]);
            if (i != 1)
            {
                simmap_string = simmap_string + ":";
            }
        }
        simmap_string = simmap_string + "}";
        
        // save the character history for this branch
        character_histories[node_index] = new std::string(simmap_string);
        
    }
    else
    {
        // the last time slice of the branch will be the state of the node before any cladogenetic events
        
        std::map<std::vector<unsigned>, double> event_map;
        std::vector<double> speciation_rates;
        if ( use_cladogenetic_events == true )
        {
            // get cladogenesis event map (sparse speciation rate matrix)
            const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( cladogenesis_matrix );
            const TypedFunction<MatrixReal>& tf = cpn->getFunction();
            const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
            
            event_map = csf->getEventMap();
        }
        else
        {
            speciation_rates = lambda->getValue();
        }
        
        // get likelihoods of descendant nodes
        const TopologyNode &left = node.getChild(0);
        size_t left_index = left.getIndex();
        state_type left_likelihoods = node_partial_likelihoods[left_index][active_likelihood[left_index]];
        const TopologyNode &right = node.getChild(1);
        size_t right_index = right.getIndex();
        state_type right_likelihoods = node_partial_likelihoods[right_index][active_likelihood[right_index]];
        
        std::map<std::vector<unsigned>, double> sample_probs;
        double sample_probs_sum = 0.0;
        std::map<std::vector<unsigned>, double>::iterator it;
        
        // calculate probabilities for each state
        if ( use_cladogenetic_events == true )
        {
            // iterate over each cladogenetic event possible
            // and initialize probabilities for each clado event
            for (it = event_map.begin(); it != event_map.end(); it++)
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
        size_t a = 0;
        size_t l = 0;
        size_t r = 0;
        
        if (sample_probs_sum == 0)
        {
            RandomNumberGenerator* rng = GLOBAL_RNG;
            size_t u = rng->uniform01() * sample_probs.size();
            size_t v = 0;
            for (it = sample_probs.begin(); it != sample_probs.end(); it++)
            {
                if (u < v)
                {
                    const std::vector<unsigned>& states = it->first;
                    a = states[0];
                    l = states[1];
                    r = states[2];
                    break;
                }
                v++;
            }
        }
        else
        {
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
                    break;
                }
            }
        }
        
        // check if there was a character state transition
        if (a != current_state)
        {
            double time_since_last_transition = 0.0;
            double transition_times_sum = 0.0;
            for (size_t j = 0; j < transition_times.size(); j++)
            {
                transition_times_sum += transition_times[j];
            }
            time_since_last_transition = current_dt_end - transition_times_sum;

            transition_times.push_back(time_since_last_transition);
            transition_states.push_back(a);
        }
        
        // add the length of the final character state
        double time_since_last_transition = 0.0;
        double transition_times_sum = 0.0;
        for (size_t j = 0; j < transition_times.size(); j++)
        {
            transition_times_sum += transition_times[j];
        }
        time_since_last_transition = branch_length - transition_times_sum;

        transition_times.push_back(time_since_last_transition);
        
        // make SIMMAP string
        std::string simmap_string = "{";
        for (size_t i = transition_times.size(); i > 0; i--)
        {
            simmap_string = simmap_string + StringUtilities::toString(transition_states[i - 1]) + "," + StringUtilities::toString(transition_times[i - 1]);
            if (i != 1)
            {
                simmap_string = simmap_string + ":";
            }
        }
        simmap_string = simmap_string + "}";
        
        // save the character history for this branch
        character_histories[node_index] = new std::string(simmap_string);
        
        // recurse towards tips
        recursivelyDrawStochasticCharacterMap(left, l, character_histories);
        recursivelyDrawStochasticCharacterMap(right, r, character_histories);
    }
}


void StateDependentSpeciationExtinctionProcess::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
{
    
    if (name == "clampCharData")
    {
        found = true;
        
        const AbstractHomologousDiscreteCharacterData& v = static_cast<const TypedDagNode<AbstractHomologousDiscreteCharacterData > *>( args[0] )->getValue();
        
        static_cast<TreeDiscreteCharacterData*>(this->value)->setCharacterData( v.clone() );
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
 * Get the event rate
 */
double StateDependentSpeciationExtinctionProcess::getEventRate(void) const
{

    if( rate != NULL )
    {
        return rate->getValue();
    }
    else
    {
        return 1.0;
    }

}


/**
 * Get the event rate generator
 */
const RateGenerator& StateDependentSpeciationExtinctionProcess::getEventRateMatrix(void) const
{

    if( Q != NULL )
    {
        return Q->getValue();
    }
    else
    {
        return Q_default;
    }

}


/**
 * Get the stationary root frequencies
 */
std::vector<double> StateDependentSpeciationExtinctionProcess::getRootFrequencies(void) const
{

    if( pi != NULL )
    {
        return pi->getValue();
    }
    else
    {
        return std::vector<double>(num_states, 1.0/num_states);
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



std::vector<double> StateDependentSpeciationExtinctionProcess::pExtinction(double start, double end) const
{
    
    double samplingProbability = rho->getValue();
    state_type initial_state = std::vector<double>(2*num_states,0);
    for (size_t i=0; i<num_states; ++i)
    {
        initial_state[i] = 1.0 - samplingProbability;
        initial_state[num_states + i] = samplingProbability;
    }
    
    numericallyIntegrateProcess(initial_state, start, end, true, false);
    
    return initial_state;
}


double StateDependentSpeciationExtinctionProcess::pSurvival(double start, double end) const
{

    state_type initial_state = pExtinction(start,end);

    double prob = 0.0;
    const RbVector<double> &freqs = getRootFrequencies();
    for (size_t i=0; i<num_states; ++i)
    {
        prob += freqs[i] * initial_state[i];
        //        prob += freqs[i]*(1.0-initial_state[i])*(1.0-initial_state[i])*speciation_rates[i];
        
    }
    
    return 1.0-prob;
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


void StateDependentSpeciationExtinctionProcess::setFossilizationRates(const TypedDagNode< RbVector<double> >* r)
{

    // remove the old parameter first
    this->removeParameter( psi );

    // set the value
    psi = r;

    // add the new parameter
    this->addParameter( psi );

    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


void StateDependentSpeciationExtinctionProcess::setSampleCharacterHistory(bool sample_history)
{
    sample_character_history = sample_history;
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
    dt = root_age->getValue() / NUM_TIME_SLICES;
    
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
    if ( oldP == lambda )
    {
        lambda = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
    }
    if ( oldP == psi )
    {
        psi = static_cast<const TypedDagNode<RbVector<double> >* >( newP );
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
        pi = static_cast<const TypedDagNode<Simplex>* >( newP );
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
    
    touchAll = true;
    
    if ( affecter == root_age )
    {
        value->getRoot().setAge( root_age->getValue() );
        dag_node->touchAffected();
    }
    
    if ( touchAll )
    {
        
        for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
        {
            (*it) = true;
        }
        
        // flip the active likelihood pointers
        for (size_t index = 0; index < changed_nodes.size(); ++index)
        {
            if ( changed_nodes[index] == false )
            {
                active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
                changed_nodes[index] = true;
            }
        }
    }
    
}


/**
 * Wrapper function for the ODE time stepper function.
 */
void StateDependentSpeciationExtinctionProcess::numericallyIntegrateProcess(state_type &likelihoods, double begin_age, double end_age, bool backward_time, bool extinction_only) const
{
    const std::vector<double> &extinction_rates = mu->getValue();
    SSE_ODE ode = SSE_ODE(extinction_rates, &getEventRateMatrix(), getEventRate(), backward_time, extinction_only);
    if ( use_cladogenetic_events == true )
    {
        cladogenesis_matrix->getValue(); // we must call getValue() to update the speciation and extinction rates in the event map
        
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

    if( psi != NULL )
    {
        const std::vector<double> &fossilization_rates = psi->getValue();
        ode.setFossilizationRate( fossilization_rates );
    }

    typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > stepper_type;
    boost::numeric::odeint::integrate_adaptive( make_controlled( 1E-6 , 1E-6 , stepper_type() ) , ode , likelihoods , begin_age , end_age , dt );

    // catch negative extinction probabilities that can result from
    // rounding errors in the ODE stepper
    for (size_t i = 0; i < num_states; ++i)
    {
        likelihoods[i] = ( likelihoods[i] < 0.0 ? 0.0 : likelihoods[i] );
    }
}
