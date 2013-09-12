//
//  AbstractCharacterHistoryCtmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AbstractCharacterHistoryCtmc.h"
#include "TypedDistribution.h"
#include "RateMatrix.h"

using namespace RevBayesCore;

AbstractCharacterHistoryCtmc::AbstractCharacterHistoryCtmc(BranchHistory* bh, TypedDagNode<RateMatrix> *rateMtx, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods) : TypedDistribution<BranchHistory>(bh), rateMatrix(rateMtx), rateModifiers(rateMods), numStates(ns), numChars(nc) {}

AbstractCharacterHistoryCtmc::AbstractCharacterHistoryCtmc(TypedDagNode<RateMatrix> *rateMtx, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods) : TypedDistribution<BranchHistory>(new BranchHistory()), rateMatrix(rateMtx), rateModifiers(rateMods), numStates(ns), numChars(nc) {}

// AbstractCharacterHistoryCtmc::AbstractCharacterHistoryCtmc(BranchHistory* bh) : TypedDistribution<BranchHistory>(bh) {}


double AbstractCharacterHistoryCtmc::computeLnProbability(void)
{
    
}

void AbstractCharacterHistoryCtmc::redrawValue(void)
{
    // ...
}

void AbstractCharacterHistoryCtmc::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
}


void AbstractCharacterHistoryCtmc::setValue(const std::multiset<CharacterEvent*,CharacterEventCompare>& updateSet, const std::set<CharacterEvent*>& parentSet, const std::set<CharacterEvent*>& childSet, const std::set<size_t>& indexSet )
{
    value->updateHistory(updateSet,parentSet,childSet,indexSet);
}

