//
//  CharacterHistoryCtmcPathUpdate.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "CharacterHistoryCtmcPathUpdate.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"

using namespace RevBayesCore;

CharacterHistoryCtmcPathUpdate::CharacterHistoryCtmcPathUpdate( StochasticNode<BranchHistory> *n, double l, bool tuning, double w) : SimpleMove(n, w, tuning), variable(n), lambda(l), storedValue(n->getValue().getNumCharacters(),n->getValue().getNumStates()), numCharacters(n->getValue().getNumCharacters()), numStates(n->getValue().getNumStates())
{
    ;
}

// Basic utility functions
CharacterHistoryCtmcPathUpdate* CharacterHistoryCtmcPathUpdate::clone(void) const
{
    return new CharacterHistoryCtmcPathUpdate( *this );
}

void CharacterHistoryCtmcPathUpdate::swapNode(DagNode *oldN, DagNode *newN)
{
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<BranchHistory>* >( newN );
}

const std::string& CharacterHistoryCtmcPathUpdate::getMoveName(void) const
{
    static std::string name = "CharacterHistoryCtmcPathUpdate";
    return name;
}

double CharacterHistoryCtmcPathUpdate::performSimpleMove(void)
{
    // store old history
    storedValue = variable->getValue();
    
    // sample characters to update
    std::set<size_t> updateSet;
    updateSet.insert(GLOBAL_RNG->uniform01() * numCharacters); // at least one is inserted
    for (size_t i = 0; i < numCharacters; i++)
    {
        if (GLOBAL_RNG->uniform01() < lambda)
        {
            updateSet.insert(i);
        }
    }
    
    // propose new value
    AbstractCharacterHistoryCtmc* p = static_cast< AbstractCharacterHistoryCtmc* >( &variable->getDistribution() );
    p->simulatePath(updateSet);
    
    return 0.0;
}

void CharacterHistoryCtmcPathUpdate::printParameterSummary(std::ostream &o) const
{
    o << variable->getValue().getNumEvents();
}

void CharacterHistoryCtmcPathUpdate::rejectSimpleMove(void)
{
    variable->setValue( new BranchHistory(storedValue) );
}

void CharacterHistoryCtmcPathUpdate::tune(void)
{
    /*
    double rate = numAccepted / double(numTried);
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
     */
}
