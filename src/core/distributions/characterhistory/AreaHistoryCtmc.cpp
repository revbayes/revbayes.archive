//
//  AreaHistoryCtmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AreaHistoryCtmc.h"
#include "BranchHistory.h"
#include "StandardState.h" // implement PresenceAbsence state later...
#include <string>

using namespace RevBayesCore;


AreaHistoryCtmc::AreaHistoryCtmc(BranchHistory* bh, TypedDagNode<RateMatrix> *rm, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods) : AbstractCharacterHistoryCtmc(bh,rm,ns,nc,rateMods) {}

AreaHistoryCtmc::AreaHistoryCtmc(TypedDagNode<RateMatrix> *rm, size_t ns, size_t nc, TypedDagNode<std::vector<AbstractCharacterHistoryRateModifier*> >* rateMods) : AbstractCharacterHistoryCtmc(rm,ns,nc,rateMods) {}

AreaHistoryCtmc* AreaHistoryCtmc::clone(void) const
{
    return new AreaHistoryCtmc(*this);
}

double AreaHistoryCtmc::transitionRate(std::vector<CharacterEvent *> curState, CharacterEvent *evt)
{
    
    double rate = 0.0;
    std::vector<CharacterEvent*> newState = curState;
    newState[ evt->getIndex() ] = evt;
    
    // rate according to Q_ij
    const size_t srcIdx = curState[evt->getIndex()]->getState();
    const size_t dstIdx = evt->getState();
    rate = rateMatrix->getValue()[ srcIdx ][ dstIdx ];

    // apply all rateModifiers
    std::vector<AbstractCharacterHistoryRateModifier*>::iterator it;
    for ( it = rateModifiers->getValue().begin(); it != rateModifiers->getValue().end(); it++)
        rate *= (*it)->computeRateModifier(curState,newState);
        
    
    return rate;
}

double AreaHistoryCtmc::sumOfRates(std::vector<CharacterEvent *> curState)
{
    double sum = 0.0;
    CharacterEvent eventChange;
    double t = curState[0]->getTime();
    
    for (size_t i = 0; i < curState.size(); i++)
    {
        for (unsigned int j = 0; j < numStates; j++)
        {
            // i->i substitutions are ignored
            if (j == curState[i]->getState())
                continue;
            
            // sum rates for all state changes
            eventChange = CharacterEvent(i, j, t);
            sum += transitionRate(curState, &eventChange);
        }
    }
    
    return sum;
}

void AreaHistoryCtmc::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    // MJL 091213
    // treat the ratemodifier functions as parameters...
    // then ratemodifiers have their own sets of parameters
    // e.g. distance dependence manages its own beta parameter
    // then we can pass AreaHistoryCtmc an arbitrary vector of RateModifier functions
    
    /*
    if (oldP == alpha) {
        alpha = static_cast<const TypedDagNode<double>* >( newP );
    }
    if (oldP == beta) {
        beta = static_cast<const TypedDagNode<double>* >( newP );
    }*/
}

double AreaHistoryCtmc::computeLnProbability(void)
{
    double lnL = 0.0;
    
    BranchHistory* bh = &this->getValue();
    
    std::vector<CharacterEvent*> curState = bh->getParentCharacters();
    std::vector<CharacterEvent*> endState = bh->getParentCharacters();
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // stepwise events
    double t = 0.0;
    double dt = 0.0;
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        
        double tr = transitionRate(curState, *it_h);
        double sr = sumOfRates(curState);
        
        std::cout << "(" << t << "," << t+dt << ") " << tr << " " << sr << " " << lnL << "\n";
        
        lnL += log(tr) - sr * dt;
        
        // update state
        curState[idx] = *it_h;
        t += dt;
    }
    
    // final dt: no event
    double sr = sumOfRates(curState);
    lnL += -sr * (1.0 - t);
    
    std::cout << "(" << t << ",1.0) " << sr << "\n";
    
    return lnL;
}



void AreaHistoryCtmc::redrawValue(void)
{
    ;
}