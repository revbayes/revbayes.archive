#include "AbstractCladogenicStateFunction.h"
#include "CDCladoSE.h"
#include "Clade.h"
#include "CharacterDependentCladoBirthDeathProcess.h"
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
#include <boost/numeric/odeint.hpp>

using namespace RevBayesCore;


/**
 * Constructor.
 *
 * The constructor connects the parameters of the birth-death process (DAG structure)
 * and initializes the probability density by computing the combinatorial constant of the tree structure.
 */
CharacterDependentCladoBirthDeathProcess::CharacterDependentCladoBirthDeathProcess(const TypedDagNode<double> *ra,
                                                                         const TypedDagNode<RbVector<double> > *mo,
                                                                         const TypedDagNode<RateGenerator>* q,
                                                                         const TypedDagNode<double>* r,
                                                                         const TypedDagNode< RbVector< double > >* p,
                                                                         const TypedDagNode<double> *rh,
                                                                         const std::string &cdt,
                                                                         const std::vector<Taxon> &tn) : TypedDistribution<Tree>( new TreeDiscreteCharacterData() ),
root_age( ra ),
mu( mo ),
pi( p ),
Q( q ),
rate( r ),
rho( rh ),
condition( cdt ),
num_taxa( tn.size() ),
changed_nodes( std::vector<bool>(2*tn.size()-1, false) ),
dirty_nodes( std::vector<bool>(2*tn.size()-1, true) ),
partial_likelihoods( std::vector<std::vector<double> >(2*tn.size()-1, std::vector<double>(2*mo->getValue().size(), 0) ) ),
marginal_likelihoods( std::vector<std::vector<double> >(2*tn.size()-1, std::vector<double>(2*mo->getValue().size(), 0) ) ),
num_states( mo->getValue().size() ),
NUM_TIME_SLICES( 200.0 )
{
    addParameter( mu );
    addParameter( pi );
    addParameter( Q );
    addParameter( rho );
    addParameter( rate );
    addParameter( root_age );
    
    
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
CharacterDependentCladoBirthDeathProcess* CharacterDependentCladoBirthDeathProcess::clone( void ) const
{
    return new CharacterDependentCladoBirthDeathProcess( *this );
}


/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double CharacterDependentCladoBirthDeathProcess::computeLnProbability( void )
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
    
// Have not implemented conditioning on survival with cladogenetic changes....
//    if ( condition == "survival" )
//    {
//        lnProbTimes = - 2*log( pSurvival(0, present_time) );
//        //        lnProbTimes = - log( pSurvival(0,present_time) );
//    }
    
    if ( condition != "time")
    {
        throw RbException("Only conditioning on time is possible");
    }
    
    // multiply the probability of a descendant of the initial species
    lnProbTimes += computeRootLikelihood();
    
    return lnProbTimes + log_tree_topology_prob;
}


void CharacterDependentCladoBirthDeathProcess::computeNodeProbability(const RevBayesCore::TopologyNode &node, size_t nodeIndex) const
{
    
    // check for recomputation
    if ( dirty_nodes[nodeIndex] || true )
    {
        // mark as computed
        dirty_nodes[nodeIndex] = false;
        
        // get cladogenesis event map (sparse speciation rate matrix)
        const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
        const TypedFunction<MatrixReal>& tf = cpn->getFunction();
        const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
        std::map<std::vector<unsigned>, double> eventMap = csf->getEventMap();
        
        state_type node_likelihood = std::vector<double>(2*num_states,0);
        if ( node.isTip() )
        {
            
            // this is a tip node
            double samplingProbability = rho->getValue();
            const DiscreteCharacterState &state = static_cast<TreeDiscreteCharacterData*>( this->value )->getCharacterData().getTaxonData( node.getTaxon().getName() )[0];
            size_t obs_state = state.getState();

            for (size_t j=0; j<num_states; ++j)
            {
                
                node_likelihood[j] = 1.0 - samplingProbability;
                
                if ( j == obs_state || state.isMissingState() == true || state.isGapState() == true )
                {
                    node_likelihood[num_states+j] = samplingProbability;
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
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            computeNodeProbability( left, leftIndex );
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            computeNodeProbability( right, rightIndex );
            
            // get the likelihoods of descendant nodes
            const state_type &left_likelihoods = partial_likelihoods[leftIndex];
            const state_type &right_likelihoods = partial_likelihoods[rightIndex];
            
            // merge descendant likelihoods
            for (size_t i=1; i<num_states; ++i)
            {
                
                node_likelihood[i] = left_likelihoods[i];
                
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
        }
        
        // calculate likelihood for this branch
        CDCladoSE ode = CDCladoSE(extinction_rates, &Q->getValue(), eventMap, rate->getValue());
        double beginAge = node.getAge();
        double endAge = node.getParent().getAge();
        double dt = root_age->getValue() / NUM_TIME_SLICES;
        boost::numeric::odeint::runge_kutta4< state_type > stepper;
        boost::numeric::odeint::integrate_const( stepper, ode , node_likelihood , beginAge , endAge, dt );
  
        // store the likelihoods
        partial_likelihoods[nodeIndex] = node_likelihood;
    }
    
}


double CharacterDependentCladoBirthDeathProcess::computeRootLikelihood( void ) const
{
    
    // get cladogenesis event map (sparse speciation rate matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMap = csf->getEventMap();
    
    // get the likelihoods of descendant nodes
    const TopologyNode &root = value->getRoot();
    size_t node_index = root.getIndex();
    const TopologyNode &left = root.getChild(0);
    size_t leftIndex = left.getIndex();
    computeNodeProbability( left, leftIndex );
    const TopologyNode &right = root.getChild(1);
    size_t rightIndex = right.getIndex();
    computeNodeProbability( right, rightIndex );

    // merge descendant likelihoods
    state_type left_likelihoods = partial_likelihoods[leftIndex];
    state_type right_likelihoods = partial_likelihoods[rightIndex];
    const RbVector<double> &freqs = pi->getValue();
    double prob = 0.0;
    state_type node_likelihood = std::vector<double>(2*num_states,0);
    for (size_t i = 0; i < num_states; ++i)
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
    
    // store the likelihoods
    partial_likelihoods[node_index] = node_likelihood;
    
    return log(prob);
}


// TODO!!! drawing ancestral states for internal nodes does not work yet
void CharacterDependentCladoBirthDeathProcess::drawJointConditionalAncestralStates(std::vector<size_t>& startStates, std::vector<size_t>& endStates)
{
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get cladogenesis event map (sparse speciation rate matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const AbstractCladogenicStateFunction* csf = dynamic_cast<const AbstractCladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMap = csf->getEventMap();
    
    // get the likelihoods of descendant nodes
    const TopologyNode &root = value->getRoot();
    size_t node_index = root.getIndex();
    const TopologyNode &left = root.getChild(0);
    size_t left_index = left.getIndex();
    state_type left_likelihoods = partial_likelihoods[left_index];
    const TopologyNode &right = root.getChild(1);
    size_t right_index = right.getIndex();
    state_type right_likelihoods = partial_likelihoods[right_index];
    
    // get root frequencies
    const RbVector<double> &freqs = pi->getValue();
    
    std::map<std::vector<unsigned>, double> sample_probs;
    double sample_probs_sum = 0.0;
    state_type node_likelihood = std::vector<double>(2 * num_states, 0);
    std::map<std::vector<unsigned>, double>::iterator it;
    
    // iterate over possible states
    for (size_t i = 0; i < num_states; ++i)
    {
        // iterate over each cladogenetic event possible for this state
        for (it = eventMap.begin(); it != eventMap.end(); it++)
        {
            const std::vector<unsigned>& states = it->first;
            double speciation_rate = it->second;
            
            if (i == states[0])
            {
                // we need to sample from the ancestor, left, and right states jointly,
                // so keep track of the probability of each clado event
                double likelihoods = left_likelihoods[num_states + states[1]] * right_likelihoods[num_states + states[2]];
                sample_probs[ states ] += likelihoods * freqs[states[0]] * speciation_rate;
                sample_probs_sum += likelihoods * freqs[states[0]] * speciation_rate;
            }
            
        }
//        // keep track of marginal node likelihoods
//        node_likelihood[i] = left_likelihoods[i];
//        node_likelihood[num_states + i] = sample_probs_sum;
    }
    
//    marginal_likelihoods[node_index] = node_likelihood;
    
    // sample ancestor, left, and right character states from probs
    size_t a, l, r;
    
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


// TODO!!! drawing ancestral states for internal nodes does not work yet
void CharacterDependentCladoBirthDeathProcess::recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<size_t>& startStates, std::vector<size_t>& endStates)
{
    size_t node_index = node.getIndex();
    if (node.isTip())
    {
        const HomologousDiscreteCharacterData<StandardState>& data = static_cast<TreeDiscreteCharacterData*>(this->value)->getCharacterData();
        const DiscreteTaxonData<StandardState>& taxon_data = data.getTaxonData( node.getName() );
        endStates[node_index] = taxon_data.getCharacter(0).getStateIndex();
    }
    else
    {
        const TopologyNode &left = node.getChild(0);
        size_t left_index = left.getIndex();
        const TopologyNode &right = node.getChild(1);
        size_t right_index = right.getIndex();
        
        // TODO !!!
        endStates[node_index] = 0;
        startStates[left_index] = 0;
        startStates[right_index] = 0;
        
        // recurse towards tips
        recursivelyDrawJointConditionalAncestralStates(left, startStates, endStates);
        recursivelyDrawJointConditionalAncestralStates(right, startStates, endStates);
    }

}


void CharacterDependentCladoBirthDeathProcess::executeProcedure(const std::string &name, const std::vector<DagNode *> args, bool &found)
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
void CharacterDependentCladoBirthDeathProcess::getAffected(RbOrderedSet<DagNode *> &affected, RevBayesCore::DagNode *affecter)
{
    
    if ( affecter == root_age)
    {
        dag_node->getAffectedNodes( affected );
    }
    
}



/**
 * Keep the current value and reset some internal flags. Nothing to do here.
 */
void CharacterDependentCladoBirthDeathProcess::keepSpecialization(DagNode *affecter)
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
void CharacterDependentCladoBirthDeathProcess::prepareProbComputation( void ) const
{
    
    // update speciation rates
    const TopologyNode &root = value->getRoot();
    if ( branchHeterogeneousCladogenesis )
        heterogeneousCladogenesisMatrices->getValue()[root.getIndex()];
    else
        homogeneousCladogenesisMatrix->getValue();
    
    // update extinction rates
    extinction_rates.clear();
    for (size_t j = 0; j < num_states; ++j)
    {
        double ex_rate = mu->getValue()[j];
        extinction_rates.push_back( ex_rate );
    }
    
}



/**
 * Redraw the current value. We delegate this to the simulate method.
 */
void CharacterDependentCladoBirthDeathProcess::redrawValue( void )
{
    
    simulateTree();
    
}



/**
 * Restore the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void CharacterDependentCladoBirthDeathProcess::restoreSpecialization(DagNode *affecter)
{
    
    if ( affecter == root_age )
    {
        value->getNode( value->getRoot().getIndex() ).setAge( root_age->getValue() );
        dag_node->restoreAffected();
    }
    
}



void CharacterDependentCladoBirthDeathProcess::setCladogenesisMatrix(const TypedDagNode< MatrixReal >* cm) {
    
    // remove the old parameter first
    if ( homogeneousCladogenesisMatrix != NULL )
    {
        this->removeParameter( homogeneousCladogenesisMatrix );
        homogeneousCladogenesisMatrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousCladogenesisMatrices );
        heterogeneousCladogenesisMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousCladogenesis = false;
    homogeneousCladogenesisMatrix = cm;
    
    // add the new parameter
    this->addParameter( homogeneousCladogenesisMatrix );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}


void CharacterDependentCladoBirthDeathProcess::setCladogenesisMatrix(const TypedDagNode< RbVector< MatrixReal > > *cm) {
    
    // remove the old parameter first
    if ( homogeneousCladogenesisMatrix != NULL )
    {
        this->removeParameter( homogeneousCladogenesisMatrix );
        homogeneousCladogenesisMatrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousCladogenesisMatrices );
        heterogeneousCladogenesisMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousCladogenesis = true;
    heterogeneousCladogenesisMatrices = cm;
    
    // add the new parameter
    this->addParameter( heterogeneousCladogenesisMatrices );
    
    // redraw the current value
    if ( this->dag_node == NULL || this->dag_node->isClamped() == false )
    {
        this->redrawValue();
    }
}



/**
 * Set the current value.
 */
void CharacterDependentCladoBirthDeathProcess::setValue(Tree *v, bool f )
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
void CharacterDependentCladoBirthDeathProcess::simulateTree( void )
{
    
}



/**
 * Swap the parameters held by this distribution.
 *
 *
 * \param[in]    oldP      Pointer to the old parameter.
 * \param[in]    newP      Pointer to the new parameter.
 */
void CharacterDependentCladoBirthDeathProcess::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
    if ( oldP == homogeneousCladogenesisMatrix )
    {
        homogeneousCladogenesisMatrix = static_cast<const TypedDagNode<MatrixReal>* >( newP );
    }
    if ( oldP == heterogeneousCladogenesisMatrices )
    {
        heterogeneousCladogenesisMatrices = static_cast<const TypedDagNode< RbVector< MatrixReal > >* >( newP );
    }
    
}



/**
 * Touch the current value and reset some internal flags.
 * If the root age variable has been restored, then we need to change the root age of the tree too.
 */
void CharacterDependentCladoBirthDeathProcess::touchSpecialization(DagNode *affecter, bool touchAll)
{
    
    if ( affecter == root_age )
    {
        value->getRoot().setAge( root_age->getValue() );
        dag_node->touchAffected();
    }
    
}
