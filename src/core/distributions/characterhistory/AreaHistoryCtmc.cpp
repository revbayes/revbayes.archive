//
//  AreaHistoryCtmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AreaHistoryCtmc.h"
#include "StandardState.h" // implement PresenceAbsence state later...
#include <string>

using namespace RevBayesCore;


AreaHistoryCtmc::AreaHistoryCtmc(BranchHistory* bh, const TypedDagNode<RateMatrix> *rateMtx, size_t nc, std::string lbls, std::vector<AbstractCharacterHistoryRateModifier*> rateMods) : AbstractCharacterHistoryCtmc(bh,rateMtx,nc,lbls,rateMods) {}


//AreaHistoryCtmc::AreaHistoryCtmc(BranchHistory* bh) : AbstractCharacterHistoryCtmc(bh) {}


double AreaHistoryCtmc::transitionRate(std::vector<CharacterEvent *> s, CharacterEvent *evt)
{
    double rate = 0.0;
    
    // get base rate of Q_i,j
    
    // for each RateModifier*, multiply
    
    return rate;
}

double AreaHistoryCtmc::sumOfRates(std::vector<CharacterEvent *> s)
{
    double sum = 0.0;
    
    size_t numStates = s[0]->getState()->getNumberOfStates();
    std::string labels = "01";
    CharacterEvent eventChange;
    
    for (size_t i = 0; i < s.size(); i++)
    {
    
        for (size_t j = 0; j < numStates; j++)
        {
            // do not gather rate if no state change occurs
            if (j == s[i]->getState()->getState())
                continue;
            
            // update site i to state j
            std::stringstream ss;
            ss << (int)j;
            StandardState stateChange(ss.str(),labels);
            eventChange = CharacterEvent(&stateChange, i, 0.0);
          
            // sum rates for all state changes
            sum += transitionRate(s, &eventChange);
        }
    }
    return sum;
}

void AreaHistoryCtmc::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    
}

double AreaHistoryCtmc::computeLnProbability(void)
{
    return 0.0;
    
}

AreaHistoryCtmc* AreaHistoryCtmc::clone(void) const
{
    ;
}