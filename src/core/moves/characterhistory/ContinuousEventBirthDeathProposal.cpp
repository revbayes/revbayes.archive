#include "CharacterEventContinuous.h"
#include "CharacterHistoryContinuous.h"
#include "ConditionedBirthDeathShiftProcessContinuous.h"
#include "DistributionNormal.h"
#include "ContinuousEventBirthDeathProposal.h"
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
ContinuousEventBirthDeathProposal::ContinuousEventBirthDeathProposal( StochasticNode<Tree> *n) : EventBirthDeathProposal( n )
{
    
    
}



/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
ContinuousEventBirthDeathProposal* ContinuousEventBirthDeathProposal::clone( void ) const
{
    
    return new ContinuousEventBirthDeathProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& ContinuousEventBirthDeathProposal::getProposalName( void ) const
{
    static std::string name = "ContinuousEventBirthDeath";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double ContinuousEventBirthDeathProposal::computeEventProposalProbability(CharacterEvent *event)
{
    
    TypedDistribution<double> *dist_lambda  = static_cast<ConditionedBirthDeathShiftProcessContinuous*>(distribution)->getSpeciationRateDistibution();
    TypedDistribution<double> *dist_mu      = static_cast<ConditionedBirthDeathShiftProcessContinuous*>(distribution)->getExtinctionRateDistibution();

    CharacterEventContinuous *cont_event = static_cast<CharacterEventContinuous*>( event );
    double rate_lambda  = cont_event->getState(0);
    double rate_mu      = cont_event->getState(1);

    dist_lambda->setValue( new double(rate_lambda) );
    dist_mu->setValue( new double(rate_mu) );

    double ln_proposal_prob = 0;
    ln_proposal_prob += dist_lambda->computeLnProbability();
    ln_proposal_prob += dist_mu->computeLnProbability();

    return ln_proposal_prob;
}


CharacterEvent* ContinuousEventBirthDeathProposal::drawNewEvent(double event_time)
{
    // first, get the two distributions
    TypedDistribution<double> *dist_lambda  = static_cast<ConditionedBirthDeathShiftProcessContinuous*>(distribution)->getSpeciationRateDistibution();
    TypedDistribution<double> *dist_mu      = static_cast<ConditionedBirthDeathShiftProcessContinuous*>(distribution)->getExtinctionRateDistibution();
    
    // now redraw values
    dist_lambda->redrawValue();
    dist_mu->redrawValue();
    
    // next, get the new values
    double rate_lambda  = dist_lambda->getValue();
    double rate_mu      = dist_mu->getValue();
    
    std::vector<double> new_state;
    new_state.push_back( rate_lambda );
    new_state.push_back( rate_mu );
    
    // draw a new state
    CharacterEvent *new_event = new CharacterEventContinuous(0, new_state, event_time);
    
    return new_event;
}


