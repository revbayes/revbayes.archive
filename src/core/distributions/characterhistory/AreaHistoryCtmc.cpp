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


AreaHistoryCtmc::AreaHistoryCtmc(BranchHistory* bh, TypedDagNode<RateMatrix> *rm, size_t nc, std::string lbls, std::vector<AbstractCharacterHistoryRateModifier*> rateMods) : AbstractCharacterHistoryCtmc(bh,rm,nc,lbls,rateMods) {}


//AreaHistoryCtmc::AreaHistoryCtmc(BranchHistory* bh) : AbstractCharacterHistoryCtmc(bh) {}


double AreaHistoryCtmc::transitionRate(std::vector<CharacterEvent *> curState, CharacterEvent *evt)
{
    double rate = 0.0;
    size_t numStates = curState[0]->getState()->getNumberOfStates();
    
    // get base rate of Q_i,j
    std::vector<CharacterEvent*> newState = curState;
    newState[ evt->getIndex() ] = evt;
    
    // rate according to rate matrix
    const size_t srcIdx = curState[evt->getIndex()]->getState()->getState();
    const size_t dstIdx = evt->getState()->getState();
    rate = rateMatrix->getValue()[ srcIdx ][ dstIdx ];

    // apply all rateModifiers
    std::vector<AbstractCharacterHistoryRateModifier*>::iterator it;
    for ( it = rateModifiers.begin(); it != rateModifiers.end(); it++)
        rate *= (*it)->computeRateModifier(curState,newState);
    
    return rate;
}

double AreaHistoryCtmc::sumOfRates(std::vector<CharacterEvent *> curState)
{
    double sum = 0.0;
    
    size_t numStates = curState[0]->getState()->getNumberOfStates();
    std::string labels = "01";
    CharacterEvent eventChange;
    
    for (size_t i = 0; i < curState.size(); i++)
    {
    
        for (size_t j = 0; j < numStates; j++)
        {
            // do not gather rate if no state change occurs
            if (j == curState[i]->getState()->getState())
                continue;
            
            // update site i to state j -- potentially slow, impl alt std_state class
            std::stringstream ss;
            ss << (int)j;
            StandardState stateChange(ss.str(),labels);
            eventChange = CharacterEvent(&stateChange, i, 0.0);
          
            // sum rates for all state changes
            sum += transitionRate(curState, &eventChange);
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