//
//  CharacterHistoryCtmcNodeUpdate.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/12/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "CharacterHistoryCtmcNodeUpdate.h"

using namespace RevBayesCore;

CharacterHistoryCtmcNodeUpdate::CharacterHistoryCtmcNodeUpdate( StochasticNode<BranchHistory> *n, double l, bool tuning, double w) : SimpleMove(n, w, tuning)
{
    ;
}

// Basic utility functions
CharacterHistoryCtmcNodeUpdate* CharacterHistoryCtmcNodeUpdate::clone(void) const
{
    return new CharacterHistoryCtmcNodeUpdate( *this );
}

void CharacterHistoryCtmcNodeUpdate::swapNode(DagNode *oldN, DagNode *newN)
{
    SimpleMove::swapNode(oldN, newN);
    
    variable = static_cast<StochasticNode<BranchHistory>* >( newN );
}

const std::string& CharacterHistoryCtmcNodeUpdate::getMoveName(void) const
{
    static std::string name = "CharacterHistoryCtmcNodeUpdate";
    return name;
}

double CharacterHistoryCtmcNodeUpdate::performSimpleMove(void)
{

}

void CharacterHistoryCtmcNodeUpdate::printParameterSummary(std::ostream &o) const
{
     o << "somethin' messy";
}

void CharacterHistoryCtmcNodeUpdate::rejectSimpleMove(void)
{
    // restore node value
    // restore path values
    // variable->setValue( new double(storedValue) );
}

void CharacterHistoryCtmcNodeUpdate::tune(void)
{
    
}
