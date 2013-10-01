//
//  DispersalHistoryCtmc.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 9/25/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "DispersalHistoryCtmc.h"
#include "BranchHistory.h"
#include "DistributionExponential.h"
#include "ExponentialDistribution.h"
#include "TypedDistribution.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include <string>

using namespace RevBayesCore;

DispersalHistoryCtmc::DispersalHistoryCtmc(TypedDagNode<RateMatrix> *rm, std::vector<const TypedDagNode<double>* > r, const TypedDagNode<TimeTree>* t, const TypedDagNode<double>* br, const TypedDagNode<double>* dp, size_t nc, size_t ns, size_t idx, GeographicDistanceRateModifier* gd) : AbstractCharacterHistoryCtmc(rm,r,t,br,nc,ns,idx), geographicDistances(gd), distancePower(dp)

{
    addParameter(distancePower);
}

DispersalHistoryCtmc::DispersalHistoryCtmc(const DispersalHistoryCtmc& m) : AbstractCharacterHistoryCtmc(m)
{
    geographicDistances = m.geographicDistances;
    distancePower = m.distancePower;
}

DispersalHistoryCtmc* DispersalHistoryCtmc::clone(void) const
{
    return new DispersalHistoryCtmc(*this);
}

double DispersalHistoryCtmc::transitionRate(std::vector<CharacterEvent *> currState, CharacterEvent *nextState)
{
    
    double rate = 0.0;
    
    // rate according to binary rate matrix Q
    rate = rates[ nextState->getState() ]->getValue();

    // account for distance
    double drm = 1.0;
    if (geographicDistances != NULL && nextState->getState() == 1)
    {
        drm = geographicDistances->computeRateModifier(currState, nextState);
        //std::cout << rate*drm << " = " << rate << " * " << drm << "\n";
        rate *= drm;
    }

    return rate;
}

double DispersalHistoryCtmc::sumOfRates(std::vector<CharacterEvent *> currState)
{
    // get rate away away from currState
    size_t numOn = 0;
    for (size_t i = 0; i < currState.size(); i++)
    {
        if (currState[i]->getState() == 1)
            numOn++;
    }
    size_t numOff = currState.size() - numOn;
    
    // forbid extinction events
    if (numOn == 1)
        numOn = 0;
    
    double sum = numOn * rates[0]->getValue() + numOff * rates[1]->getValue();
    return sum;
}

void DispersalHistoryCtmc::swapParameter(const DagNode *oldP, const DagNode *newP)
{
    AbstractCharacterHistoryCtmc::swapParameter(oldP, newP);
    
    if (oldP == distancePower)
    {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}

double DispersalHistoryCtmc::computeLnProbability(void)
{
    double lnL = 0.0;
    
    BranchHistory* bh = value;
    
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    std::vector<CharacterEvent*> endState = bh->getParentCharacters();
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // rate/time scale
    double t = 0.0;
    double dt = 0.0;
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index);
    double bs = bt/br;

    // stepwise events
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        dt *= bs; // rescale time (correct?)
        
        double tr = transitionRate(currState, *it_h);
        double sr = sumOfRates(currState);
        
        // lnL for stepwise events
        lnL += log(tr) - sr * dt;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        //std::cout << t << " " << dt << " " << tr << " " << sr << " " << lnL << "\n";
    }
    
    // lnL for final non-event
    double sr = sumOfRates(currState);
    lnL += -sr * (1.0 - t);

    return lnL;
}

//virtual void                        keepSpecialization(DagNode* affecter);
//virtual void                        restoreSpecialization(DagNode *restorer);
void DispersalHistoryCtmc::touchSpecialization(DagNode *toucher)
{
    if (toucher == distancePower && geographicDistances != NULL)
    {
        geographicDistances->updateGeographicDistancePowers(distancePower->getValue());
    }
}



void DispersalHistoryCtmc::simulatePath(void)
{
    value->clearEvents();
    
    // get transition rates
    std::vector<std::vector<double> > r(numStates);
    for (size_t i = 0; i < numStates; i++)
    {
        r[i].resize(numStates);
        for (size_t j = 0; j < numStates; j++)
        {
            if (i != j)
            {
                size_t k = i * (numStates-1) + j;
                r[i][i] -= rates[k]->getValue();
                r[i][j] = rates[k]->getValue();
            }
        }
    }
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = value->getParentCharacters();
    std::vector<CharacterEvent*> childVector = value->getChildCharacters();
    std::set<CharacterEvent*,CharacterEventCompare> history;
    
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index);
    if (bt == 0.0)
        bt = 200;
    bt = 10.0;
    
    for (size_t i = 0; i < numCharacters; i++)
    {
        std::set<CharacterEvent*> tmpHistory;
        
        unsigned int currState = parentVector[i]->getState();
        unsigned int endState = childVector[i]->getState();
        do
        {
            tmpHistory.clear();
            currState = parentVector[i]->getState();
            double t = 0.0;
            do
            {
                double r_sum = -r[currState][currState];
                t += RbStatistics::Exponential::rv( r_sum * bt * br, *GLOBAL_RNG );
                
                if (t < 1.0)
                {
                    currState = ( currState == 1 ? 0 : 1);
                    CharacterEvent* evt = new CharacterEvent(i,currState,t);
                    tmpHistory.insert(evt);
                }
                //std::cout << i << " " << t << " " << currState << "\n";
            }
            while(t < 1.0);
            //std::cout << "--------\n";
        }
        while (currState != endState);
        //std::cout << "^ accepted\n--------\n";
        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
            history.insert(*it);
    }
    
    if (historyContainsExtinction(parentVector, history) == true)
    {
        std::cout << "contains extinction\n";
        simulatePath();
    }
    else
        value->setHistory(history);
}

bool DispersalHistoryCtmc::historyContainsExtinction(const std::vector<CharacterEvent*>& v, const std::set<CharacterEvent*,CharacterEventCompare>& s)
{
    int n = numOn(v);
 
    std::set<CharacterEvent*,CharacterEventCompare>::const_iterator it;
    for (it = s.begin(); it != s.end(); it++)
    {
        if ( (*it)->getState() == 0 )
            n--;
        else
            n++;

        if (n == 0)
            return true;
    }
    
    return false;
}

void DispersalHistoryCtmc::simulateChildCharacterState(void)
{
    value->setChildCharacters(simulateCharacterState(1.0));
}

void DispersalHistoryCtmc::simulateParentCharacterState(void)
{
    value->setParentCharacters(simulateCharacterState(0.0));
}

std::set<CharacterEvent*> DispersalHistoryCtmc::simulateCharacterState(double t)
{
    std::set<CharacterEvent*> characterState;
    int numOn = 0;
    
    for (size_t i = 0; i < numCharacters; i++)
    {
        unsigned int s = (unsigned int)(GLOBAL_RNG->uniform01() * numStates);
        numOn += s;
        characterState.insert(new CharacterEvent(i,s,t));
    }
    
    // retry if extinct
    if (numOn == 0)
        characterState = simulateCharacterState(t);
    
    return characterState;
}


void DispersalHistoryCtmc::redrawValue(void)
{
    
    //std::cout << "ns " << numStates << "   nc " << numCharacters << "\n";
    
    if (value->getRedrawParentCharacters())
    {
        std::cout << index << " redraw parent\n";
        simulateParentCharacterState();
        value->setRedrawParentCharacters(false);
    }
    
    
    if (value->getRedrawChildCharacters())
    {
        std::cout << index << " redraw child\n";
        simulateChildCharacterState();
        value->setRedrawChildCharacters(false);
    }
    
    
    if (value->getRedrawHistory())
    {
        std::cout << index << " redraw path\n";
        simulatePath();
        value->setRedrawHistory(false);
    }
    
    //value->print();
}

unsigned int DispersalHistoryCtmc::numOn(std::vector<CharacterEvent*> v)
{
    unsigned int numOn = 0;
    for (size_t i = 0; i < v.size(); i++)
        numOn++;
    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*>::iterator it = s.begin(); it != s.end(); it++)
        numOn++;
    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*,CharacterEventCompare> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = s.begin(); it != s.end(); it++)
        numOn++;
    
    return numOn;
    
}
