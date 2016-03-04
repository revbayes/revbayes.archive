#include "DistributionNormal.h"
#include "EventTimeSlideProposal.h"
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
EventTimeSlideProposal::EventTimeSlideProposal( StochasticNode<Tree> *n, double d) : Proposal(),
    variable( n ),
    delta( d )
{
    // tell the base class to add the node
    addNode( variable );
    
    distribution = dynamic_cast< AbstractCharacterHistoryBirthDeathProcess* >( &variable->getDistribution() );
    if ( distribution == NULL )
    {
        throw RbException("Wrong type of variable for discrete-event-category random walk move.");
    }
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void EventTimeSlideProposal::cleanProposal( void )
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
EventTimeSlideProposal* EventTimeSlideProposal::clone( void ) const
{
    
    return new EventTimeSlideProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& EventTimeSlideProposal::getProposalName( void ) const
{
    static std::string name = "EventTimeSlide";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double EventTimeSlideProposal::doProposal( void )
{
    // reset flags
    failed = true;
    
    CharacterHistory &history = distribution->getCharacterHistory();
    Tree &tree = variable->getValue();
    
    RandomNumberGenerator *rng = GLOBAL_RNG;
    
    size_t num_events = history.getNumberEvents();
    failed = (num_events == 0);
    
    double ln_proposal_ratio = 0.0;
    if ( num_events > 0 )
    {
        
        // pick a random event
        size_t branch_index = 0;
        CharacterEvent *event = history.pickRandomEvent( branch_index );
        
        // always remove event because we need to re-order the times
        history.removeEvent(event, branch_index);

        
        // store the event
        stored_value = event;
        // store the current time
        stored_time = event->getTime();
        // store the current branch
        stored_branch_index = branch_index;
        
        // draw a new time which we slide
        double t = RbStatistics::Normal::rv(0, delta, *rng);
        
        double remaining_branch_length = 0.0;
        double used_time = 0.0;
        double branch_length = tree.getNode( branch_index ).getBranchLength();
        double current_absolute_time = event->getTime();
        if ( t > 0 )
        {
            remaining_branch_length = branch_length - current_absolute_time;
            used_time = current_absolute_time;
        }
        else
        {
            remaining_branch_length = current_absolute_time;
            used_time = branch_length - current_absolute_time;
        }
        
        while ( fabs(t) > remaining_branch_length )
        {
            // we need to remove the event from its branch
            used_time = 0.0;
            
            if ( t > 0 )
            {
                // we are sliding up the tree
                t -= remaining_branch_length;
                if ( tree.getNode(branch_index).getParent().isRoot() == true )
                {
                    // we need to reflect
                    t = -t;
                    
                    // flip a coin if we go left or right
                    size_t child_index = ( rng->uniform01() < 0.5 ? 0 : 1 );
                    // add to the proposal ratio
                    ln_proposal_ratio += RbConstants::LN2;
                    // the new branch index
                    branch_index = tree.getNode(branch_index).getParent().getChild(child_index).getIndex();
                }
                else
                {
                    branch_index = tree.getNode(branch_index).getParent().getIndex();
                }
                
                // the new remaining branch length
                remaining_branch_length = tree.getNode( branch_index ).getBranchLength();
                
            }
            else
            {
                t += remaining_branch_length;
                if ( tree.getNode(branch_index).isTip() == true )
                {
                    // we need to reflect
                    t = -t;
                }
                else
                {
                    // flip a coin if we go left or right
                    size_t child_index = ( rng->uniform01() < 0.5 ? 0 : 1 );
                    // add to the proposal ratio
                    ln_proposal_ratio += RbConstants::LN2;
                    // the new branch index
                    branch_index = tree.getNode(branch_index).getChild(child_index).getIndex();
                }
                
                // the new remaining branch length
                remaining_branch_length = tree.getNode( branch_index ).getBranchLength();
            }
            
        }
        
        // new_relative_time = (used_time+t)/bl;
        //        }
        //        else
        //        {
        //            new_relative_time = (bl-used_time+t)/bl;
        
        double new_branch_length = tree.getNode( branch_index ).getBranchLength();
        double new_absolute_time = 0.0;
        if (t > 0)
        {
            new_absolute_time = used_time + t;
        }
        else
        {
            new_absolute_time = new_branch_length - used_time + t;
        }
        
        assert( new_absolute_time >= 0 && new_absolute_time <= new_branch_length );
        
        // set the time
        event->setTime(new_absolute_time);
        history.addEvent( event, branch_index );
        proposed_branch_index = branch_index;
//        double remaining_branch_length = 0.0;
//        double used_time = 0.0;
//        double branch_length = tree.getNode( branch_index ).getBranchLength();
//        double current_relative_time = event->getTime() / branch_length;
//        if ( t > 0 )
//        {
//            remaining_branch_length = (1.0-current_relative_time) * branch_length;
//            used_time = current_relative_time * branch_length;
//        }
//        else
//        {
//            remaining_branch_length = current_relative_time * branch_length;
//            used_time = (1.0-current_relative_time) * branch_length;
//        }
//
//        while ( fabs(t) > remaining_branch_length )
//        {
//            // we need to remove the event from its branch
//            used_time = 0.0;
//            
//            if ( t > 0 )
//            {
//                // we are sliding up the tree
//                t -= remaining_branch_length;
//                if ( tree.getNode(branch_index).getParent().isRoot() == true )
//                {
//                    // we need to reflect
//                    t = -t;
//                    
//                    // flip a coin if we go left or right
//                    size_t child_index = ( rng->uniform01() < 0.5 ? 0 : 1 );
//                    // add to the proposal ratio
//                    ln_proposal_ratio += RbConstants::LN2;
//                    // the new branch index
//                    branch_index = tree.getNode(branch_index).getParent().getChild(child_index).getIndex();
//                }
//                else
//                {
//                    branch_index = tree.getNode(branch_index).getParent().getIndex();
//                }
//                
//                // the new remaining branch length
//                remaining_branch_length = tree.getNode( branch_index ).getBranchLength();
//                
//            }
//            else
//            {
//                t += remaining_branch_length;
//                if ( tree.getNode(branch_index).isTip() == true )
//                {
//                    // we need to reflect
//                    t = -t;
//                }
//                else
//                {
//                    // flip a coin if we go left or right
//                    size_t child_index = ( rng->uniform01() < 0.5 ? 0 : 1 );
//                    // add to the proposal ratio
//                    ln_proposal_ratio += RbConstants::LN2;
//                    // the new branch index
//                    branch_index = tree.getNode(branch_index).getChild(child_index).getIndex();
//                }
//                
//                // the new remaining branch length
//                remaining_branch_length = tree.getNode( branch_index ).getBranchLength();
//            }
//            
//        }
//        
//        
//        double new_relative_time = 0.0;
//        double bl = tree.getNode( branch_index ).getBranchLength();
//        if ( t > 0 )
//        {
//            new_relative_time = (used_time+t)/bl;
//        }
//        else
//        {
//            new_relative_time = (bl-used_time+t)/bl;
//        }
//        
//        assert( new_relative_time >= 0 && new_relative_time <= 1 );
//        
//        // set the time
//        event->setTime( new_relative_time * bl );
//        history.addEvent( event, branch_index );
//        proposed_branch_index = branch_index;
        
    }
    else
    {
        //        move->decrementTriedCounter();
    }
    
    
    return ln_proposal_ratio;
}


/**
 *
 */
void EventTimeSlideProposal::prepareProposal( void )
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
void EventTimeSlideProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "delta = " << delta;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void EventTimeSlideProposal::undoProposal( void )
{
    
    if ( failed == false )
    {
        CharacterHistory &history = distribution->getCharacterHistory();
        history.removeEvent( stored_value, proposed_branch_index);
        
        stored_value->setTime( stored_time );
        
        history.addEvent( stored_value, stored_branch_index );
        
    }
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void EventTimeSlideProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
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
void EventTimeSlideProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        delta *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        delta /= (2.0 - rate/0.44 );
    }
    
}

