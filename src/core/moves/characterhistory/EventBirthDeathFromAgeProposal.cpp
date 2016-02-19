#include "DistributionNormal.h"
#include "EventBirthDeathFromAgeProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
EventBirthDeathFromAgeProposal::EventBirthDeathFromAgeProposal( StochasticNode<Tree> *n) : Proposal(),
variable( n ),
stored_value(),
by_height(true)
{
    // tell the base class to add the node
    addNode( variable );
    
    distribution = dynamic_cast< AbstractCharacterHistoryBirthDeathProcess* >( &variable->getDistribution() );
    if ( distribution == NULL )
    {
        throw RbException("Wrong type of variable for BirthDeathEvent move.");
    }
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void EventBirthDeathFromAgeProposal::cleanProposal( void )
{
    
    if ( was_birth_proposal == false )
    {
        delete stored_value;
        stored_value = NULL;
    }
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
EventBirthDeathFromAgeProposal* EventBirthDeathFromAgeProposal::clone( void ) const
{
    
    return new EventBirthDeathFromAgeProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& EventBirthDeathFromAgeProposal::getProposalName( void ) const
{
    static std::string name = "EventBirthDeath";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double EventBirthDeathFromAgeProposal::doProposal( void )
{
    
    CharacterHistory &history = distribution->getCharacterHistory();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    double u = rng->uniform01();
    
    double p_birth = 1.0;
    double p_death = 0.0;
    
    size_t num_events = history.getNumberEvents();
    
    if ( num_events > 0 )
    {
        p_birth = 0.5;
        p_death = 0.5;
    }
    
    double proposal_prob = 0.0;
    if ( u < p_birth )
    {
        proposal_prob = doBirthProposal();
    }
    else
    {
        proposal_prob = doDeathProposal();
    }
    
    return proposal_prob;
}


double EventBirthDeathFromAgeProposal::doBirthProposal( void )
{
    // set the flag that this was a birth proposal
    was_birth_proposal = true;
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    CharacterHistory &history = distribution->getCharacterHistory();
    Tree &tree = distribution->getValue();
    
    size_t num_events_before = history.getNumberEvents();
    size_t num_states   = history.getNumberStates();
    
    
    double tree_length = 0.0;
    size_t num_possible_branches = 0;
    double event_time = 0;
    size_t branch_index = 0;
    if (by_height)
    {

        // pick an age
        double root_age = tree.getRoot().getAge();
        double new_age = rng->uniform01() * root_age;
        
        // pick a branch
        std::vector<TopologyNode*> nodes = tree.getNodes();
        std::vector<size_t> possible_branches;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->isRoot())
                continue;
            else if ( new_age > nodes[i]->getAge() && new_age < nodes[i]->getParent().getAge() )
            {
                possible_branches.push_back( nodes[i]->getIndex() );
    //            std::cout << nodes[i]->getParent().getAge() << " " << new_age << " " << nodes[i]->getAge() << "\n";
            }
        }
        num_possible_branches = possible_branches.size();
        size_t u = (size_t)(rng->uniform01() * num_possible_branches);
        branch_index = possible_branches[u];
        
        // draw an event time, which is simply uniform between 0 and 1
        double parent_age = nodes[branch_index]->getParent().getAge();
        double node_age = nodes[branch_index]->getAge();
        event_time = (parent_age - new_age) / (parent_age - node_age);
    }
    else
    {
        std::vector<TopologyNode*> nodes = tree.getNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            tree_length += nodes[i]->getBranchLength();
        }
        double u = rng->uniform01() * tree_length;
        
        while (u > 0)
        {
            if (nodes[branch_index]->isRoot())
                continue;
            
            u -= nodes[branch_index]->getBranchLength();
            
            if (u > 0)
                branch_index++;
        }
        event_time = -u / nodes[branch_index]->getBranchLength();
    }
    
    
     
    // draw a new state
    size_t new_state = size_t( std::floor(num_states * rng->uniform01()) );

    CharacterEvent *new_event = new CharacterEvent(0, new_state, event_time);
    history.addEvent( new_event, branch_index );
    
    // store value for reversal of proposal
    stored_value = new_event;
    stored_branch_index = branch_index;
    
    double log_birth_move_prob = log(num_events_before == 0 ? 1.0 : 0.5);
    double log_death_move_prob = log(0.5);
    double p_forward = 0.0;
    if (by_height)
    {
        p_forward  = log_birth_move_prob - log(num_states) - log(num_possible_branches) - log(tree.getRoot().getAge());

    }
    else
    {
        p_forward  = log_birth_move_prob - log(num_states) - log(tree_length);
    }
    double p_backward = log_death_move_prob - log(num_events_before+1);
    return p_backward - p_forward;
}

double EventBirthDeathFromAgeProposal::doDeathProposal( void )
{
    // set the flag that this was a death proposal
    was_birth_proposal = false;
    
    CharacterHistory &history = distribution->getCharacterHistory();
    Tree &tree = distribution->getValue();
    
    size_t num_events_before = history.getNumberEvents();
    size_t num_states   = history.getNumberStates();
    
    size_t branch_index = 0;
    CharacterEvent *event = history.pickRandomEvent( branch_index );
    double event_time = event->getTime();
    history.removeEvent( event, branch_index );
    
    // store the event
    stored_value = event;
    stored_branch_index = branch_index;
    
    double log_death_move_prob = log(0.5);
    double log_birth_move_prob = log(num_events_before == 1 ? 1.0 : 0.5);
    double p_forward  = log_death_move_prob - log(num_events_before);
    double p_backward = 0.0;

    if (by_height)
    {
        double stored_branch_length = tree.getNode(stored_branch_index).getBranchLength();
        double stored_parent_age = tree.getNode(stored_branch_index).getParent().getAge();
        double remove_age = stored_parent_age + stored_branch_length * event_time;
        
        // pick a branch
        std::vector<TopologyNode*> nodes = tree.getNodes();
        std::vector<size_t> possible_branches;
        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->isRoot())
                continue;
            else if ( remove_age > nodes[i]->getAge() && remove_age < nodes[i]->getParent().getAge() )
                possible_branches.push_back( nodes[i]->getIndex() );
        }
        size_t num_possible_branches = possible_branches.size();
        
        p_backward = log_birth_move_prob - log(num_states) - log(num_possible_branches) - log(tree.getRoot().getAge());
    }
    else
    {
        double tree_length = 0.0;
        std::vector<TopologyNode*> nodes = tree.getNodes();
        for (size_t i = 0; i < nodes.size(); i++)
        {
            tree_length += nodes[i]->getBranchLength();
        }

        p_backward = log_birth_move_prob - log(num_states) - log(tree_length);
    }
    
    return p_backward - p_forward;
}


void EventBirthDeathFromAgeProposal::findNewBrothers(std::vector<TopologyNode *> &b, TopologyNode &p, TopologyNode *n) {
    // security check that I'm not a tip
    if (!n->isTip() && &p != n)
    {
        // check the first child
        TopologyNode* child = &n->getChild( 0 );
        if ( child->getAge() < p.getAge() )
        {
            b.push_back( child );
        }
        else
        {
            findNewBrothers(b, p, child);
        }
        
        // check the second child
        child = &n->getChild( 1 );
        if ( child->getAge() < p.getAge() )
        {
            b.push_back( child );
        }
        else
        {
            findNewBrothers(b, p, child);
        }
    }
}

/**
 *
 */
void EventBirthDeathFromAgeProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void EventBirthDeathFromAgeProposal::printParameterSummary(std::ostream &o) const
{
    
    //    o << "delta = " << delta;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void EventBirthDeathFromAgeProposal::undoProposal( void )
{
    
    CharacterHistory &history = distribution->getCharacterHistory();
    if ( was_birth_proposal == true )
    {
        history.removeEvent( stored_value, stored_branch_index );
        delete stored_value;
        stored_value = NULL;
    }
    else
    {
        history.addEvent( stored_value, stored_branch_index );
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void EventBirthDeathFromAgeProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast< StochasticNode<Tree>* >(newN) ;
    
    distribution = dynamic_cast< AbstractCharacterHistoryBirthDeathProcess* >( &variable->getDistribution() );
    if ( distribution == NULL )
    {
        throw RbException("Wrong type of variable for BirthDeathEvent move.");
    }
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void EventBirthDeathFromAgeProposal::tune( double rate )
{
    
    //    if ( rate > 0.44 )
    //    {
    //        delta *= (1.0 + ((rate-0.44)/0.56) );
    //    }
    //    else
    //    {
    //        delta /= (2.0 - rate/0.44 );
    //    }
    
}

