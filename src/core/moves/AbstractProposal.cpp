//
//  AbstractProposal.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 10/24/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "AbstractProposal.h"

using namespace RevBayesCore;

const std::set<DagNode*>& AbstractProposal::getNodes(void) const
{
    return nodes;
}

const std::set<DagNode*>& AbstractProposal::getAuxillaryNodes(void) const
{
    return auxillaryNodes;
}