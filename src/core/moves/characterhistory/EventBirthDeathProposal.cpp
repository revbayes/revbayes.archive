#include "DistributionNormal.h"
#include "EventBirthDeathProposal.h"
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
EventBirthDeathProposal::EventBirthDeathProposal( StochasticNode<Tree> *n) : Proposal(),
    variable( n ),
    stored_value()
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
void EventBirthDeathProposal::cleanProposal( void )
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
EventBirthDeathProposal* EventBirthDeathProposal::clone( void ) const
{
    
    return new EventBirthDeathProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& EventBirthDeathProposal::getProposalName( void ) const
{
    static std::string name = "EventBirthDeath";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double EventBirthDeathProposal::doProposal( void )
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


double EventBirthDeathProposal::doBirthProposal( void )
{
    // set the flag that this was a birth proposal
    was_birth_proposal = true;
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    CharacterHistory &history = distribution->getCharacterHistory();
    
    size_t num_events_before = history.getNumberEvents();
    size_t num_branches = history.getNumberBranches();
    size_t num_states   = history.getNumberStates();
    
    // randomly pick a branch
    size_t branch_index = size_t( std::floor(num_branches * rng->uniform01()) );
    
    // draw a new state
    size_t new_state = size_t( std::floor(num_states * rng->uniform01()) );
    
    // draw an event time, which is simply uniform between 0 and 1
    double event_time = rng->uniform01();
    
    CharacterEvent *new_event = new CharacterEvent(0, new_state, event_time);
    history.addEvent( new_event, branch_index );
    
    // store value for reversal of proposal
    stored_value = new_event;
    stored_branch_index = branch_index;
    
    double log_birth_move_prob = log(num_events_before == 0 ? 1.0 : 0.5);
    double log_death_move_prob = log(0.5);
    double p_forward  = log_birth_move_prob - log(num_branches) - log(num_states);
    double p_backward = log_death_move_prob - log(num_events_before+1);
    return p_backward - p_forward;
}

double EventBirthDeathProposal::doDeathProposal( void )
{
    // set the flag that this was a death proposal
    was_birth_proposal = false;
    
    CharacterHistory &history = distribution->getCharacterHistory();
    
    size_t num_events_before = history.getNumberEvents();
    size_t num_branches = history.getNumberBranches();
    size_t num_states   = history.getNumberStates();
    
    size_t branch_index = 0;
    CharacterEvent *event = history.pickRandomEvent( branch_index );
    history.removeEvent( event, branch_index );
    
    // store the event
    stored_value = event;
    stored_branch_index = branch_index;
    
    double log_death_move_prob = log(0.5);
    double log_birth_move_prob = log(num_events_before == 1 ? 1.0 : 0.5);
    double p_forward  = log_death_move_prob - log(num_events_before);
    double p_backward = log_birth_move_prob - log(num_branches) - log(num_states);
    return p_backward - p_forward;
}


/**
 *
 */
void EventBirthDeathProposal::prepareProposal( void )
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
void EventBirthDeathProposal::printParameterSummary(std::ostream &o) const
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
void EventBirthDeathProposal::undoProposal( void )
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
void EventBirthDeathProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
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
void EventBirthDeathProposal::tune( double rate )
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

