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
    if (geographicDistances != NULL)
        geographicDistances->updateGeographicDistancePowers(distancePower->getValue());
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
   
    // rate to extinction cfg is 0
    if (numOn(currState) == 1 && nextState->getState() == 0)
        return 0.0;
    
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
    
    if (tree->getValue().getNode(index).isRoot())
        return 0.0;
    
    BranchHistory* bh = value;
    
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned int n = numOn(currState);
    if (n == 0)
        return 0.0;
    //std::vector<CharacterEvent*> endState = bh->getParentCharacters();
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // rate/time scale
    double t = 0.0;
    double dt = 0.0;
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0)
        bt = 10.0;
    double bs = bt*br;
    //std::cout << "** " << bs << " = " << br << " * " << bt << "\n";
    //bh->print();
    //std::cout << "**\n";

    // stepwise events
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        dt *= bs; // rescale time (correct?)
        
        if ((*it_h)->getState() == 0)
            n--;
        else
            n++;
        if (n == 0)
            return RbConstants::Double::neginf;
        
        double tr = transitionRate(currState, *it_h);
        double sr = sumOfRates(currState);
        
        // lnL for stepwise events for p(x->y)
        lnL += log(tr) - sr * dt;
        
        // proposal density for q(x->y)... might cancel, largely??
        //double tr_iid = rates[ (*it_h)->getState() ]->getValue();
        //lnL -= log(tr_iid) - sr * dt;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        //std::cout << t << " " << dt << " " << tr << " " << sr << " " << lnL << "; " << bt << " " << br << " " << dt << "; " << (*it_h)->getState() << " " << numOn(currState) << "\n";
    }
    
    // lnL for final non-event
    double sr = sumOfRates(currState);
    lnL += -sr * (1.0 - t);
    
    // for q(x->y)... cancels?
    //lnL -= -sr * (1.0 - t);

    //std::cout << "----\n";
    return lnL;
}

double DispersalHistoryCtmc::computeLnProposal(void)
{
    double lnL = 0.0;
    
    if (tree->getValue().getNode(index).isRoot())
        return 0.0;
    
    BranchHistory* bh = value;
    
    std::vector<CharacterEvent*> currState = bh->getParentCharacters();
    unsigned int n = numOn(currState);
    if (n == 0)
        return 0.0;
    //std::vector<CharacterEvent*> endState = bh->getParentCharacters();
    
    std::multiset<CharacterEvent*,CharacterEventCompare> history = bh->getHistory();
    std::multiset<CharacterEvent*,CharacterEventCompare>::iterator it_h;
    
    // rate/time scale
    double t = 0.0;
    double dt = 0.0;
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0)
        bt = 10.0;
    double bs = bt*br;
    //std::cout << "** " << bs << " = " << br << " * " << bt << "\n";
    //bh->print();
    //std::cout << "**\n";
    
    // stepwise events
    for (it_h = history.begin(); it_h != history.end(); it_h++)
    {
        double idx = (*it_h)->getIndex();
        dt = (*it_h)->getTime() - t;
        dt *= bs; // rescale time (correct?)
        
        if ((*it_h)->getState() == 0)
            n--;
        else
            n++;
        if (n == 0)
            return RbConstants::Double::neginf;
        
        double tr = rates[ (*it_h)->getState() ]->getValue();
        double sr = sumOfRates(currState);
        
        // lnL for stepwise events for p(x->y)
        lnL += log(tr) - sr * dt;
        
        // update state
        currState[idx] = *it_h;
        t += dt;
        //std::cout << t << " " << dt << " " << tr << " " << sr << " " << lnL << "; " << bt << " " << br << " " << dt << "; " << (*it_h)->getState() << " " << numOn(currState) << "\n";
    }
    
    // lnL for final non-event
    double sr = sumOfRates(currState);
    lnL += -sr * (1.0 - t);
    
    // for q(x->y)... cancels?
    //lnL -= -sr * (1.0 - t);
    
    
    // child and parent character states
    double p1 = rates[1]->getValue() / (rates[0]->getValue() + rates[1]->getValue());
    unsigned int m = numOn(value->getParentCharacters()) + numOn(value->getChildCharacters());
    lnL += m * log(p1) - (numCharacters - m)* log(1.0 - p1);
    
    //std::cout << "----\n";
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



void DispersalHistoryCtmc::samplePath(const std::set<size_t>& indexSet)
{

    value->clearEvents(indexSet);
    
    // get transition rates
    double r[2];
    r[0] = rates[0]->getValue();
    r[1] = rates[1]->getValue();
    
    /*
    std::vector<std::vector<double> > r(numStates);
    for (size_t i = 0; i < numStates; i++)
        r[i].resize(numStates);

    r[0][1] = rates[0]->getValue();
    r[0][0] = -r[0][1];
    r[1][0] = rates[1]->getValue();
    r[1][1] = -r[1][0];
    
    for (size_t i = 0; i < numStates; i++)
    {
        for (size_t j = 0; j < numStates; j++)
        {
            std::cout << r[i][j] << " ";
        }
        std::cout << "\n";
    }
     */
    
    // reject sample path history
    std::vector<CharacterEvent*> parentVector = value->getParentCharacters();
    std::vector<CharacterEvent*> childVector = value->getChildCharacters();
    std::multiset<CharacterEvent*,CharacterEventCompare> history;
    
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 10.0;
    
    //std::cout << br << " " << bt << "\n";
    
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        size_t i = *it;
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
//                double r_sum = r[currState];
                t += RbStatistics::Exponential::rv( r[currState] * bt * br, *GLOBAL_RNG );
                //std::cout << i << " " << t << " " << currState << " " << r[currState]*bt*br << "\n";
                if (t < 1.0)
                {
                    currState = ( currState == 1 ? 0 : 1);
                    CharacterEvent* evt = new CharacterEvent(i,currState,t);
                    tmpHistory.insert(evt);
                }
            }
            while(t < 1.0);
        }
        while (currState != endState);
        
        for (std::set<CharacterEvent*>::iterator it = tmpHistory.begin(); it != tmpHistory.end(); it++)
            history.insert(*it);
    }
    
    /*
    if (historyContainsExtinction(parentVector, history) == true)
    {
        std::cout << "contains extinction\n";
        samplePath(indexSet);
        return;
    }
     */
    
    //else
        value->updateHistory(history,indexSet);
        //value->setHistory(history);
}

bool DispersalHistoryCtmc::historyContainsExtinction(const std::vector<CharacterEvent*>& v, const std::multiset<CharacterEvent*,CharacterEventCompare>& s)
{
    int n = numOn(v);
 
    std::multiset<CharacterEvent*,CharacterEventCompare>::const_iterator it;
    
    //for (size_t i = 0; i < v.size(); i++) std::cout << v[i]->getState(); std::cout << "\n";
    
    for (it = s.begin(); it != s.end(); it++)
    {
        //for (size_t i = 0; i < (*it)->getIndex(); i++) std::cout << " "; std::cout << (*it)->getState() << "\n";
        if ( (*it)->getState() == 0 )
            n--;
        else
            n++;

        if (n == 0)
            return true;
    }
    
    return false;
}

void DispersalHistoryCtmc::sampleChildCharacterState(const std::set<size_t>& indexSet)
{
    std::vector<CharacterEvent*> childStates = value->getChildCharacters();

    sampleCharacterState(indexSet, childStates, 1.0);
    value->setChildCharacters(childStates);
//std::vector<CharacterEvent*> parentStates = value->getParentCharacters();
    //for (size_t i = 0; i < parentStates.size(); i++) std::cout << parentStates[i]->getState() << " "; std::cout << "\n";
    //for (size_t i = 0; i < childStates.size(); i++) std::cout << childStates[i]->getState() << " "; std::cout << "\n";

}

void DispersalHistoryCtmc::sampleParentCharacterState(const std::set<size_t>& indexSet)
{
    std::vector<CharacterEvent*> parentStates = value->getParentCharacters();
    sampleCharacterState(indexSet, parentStates, 0.0);
    value->setParentCharacters(parentStates);
}

void DispersalHistoryCtmc::sampleCharacterState(const std::set<size_t>& indexSet, std::vector<CharacterEvent*>& states, double t)
{
    double p1 = rates[1]->getValue() / (rates[0]->getValue() + rates[1]->getValue());
    for (std::set<size_t>::iterator it = indexSet.begin(); it != indexSet.end(); it++)
    {
        unsigned int s = (unsigned int)(GLOBAL_RNG->uniform01() < p1 ? 1 : 0);
        //states[*it]->setState(s);
        states[*it] = new CharacterEvent(*it,s,t);
    }
    
    // retry if extinct
    if (numOn(states) == 0)
        sampleCharacterState(indexSet, states,t);

    /*
    else
    {
        for (size_t i = 0; i < states.size(); i++)
            std::cout << states[i]->getState() << " ";
        std::cout << "\n";
    }
     */

}


void DispersalHistoryCtmc::redrawValue(void)
{
    
    //std::cout << "ns " << numStates << "   nc " << numCharacters << "\n";
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);
    
    if (value->getRedrawParentCharacters())
    {
        std::cout << index << " redraw parent\n";
        sampleParentCharacterState(indexSet);
        value->setRedrawParentCharacters(false);
    }
    
    
    if (value->getRedrawChildCharacters())
    {
        std::cout << index << " redraw child\n";
        sampleChildCharacterState(indexSet);
        value->setRedrawChildCharacters(false);
    }
    
    
    if (value->getRedrawHistory())
    {
        std::cout << index << " redraw path\n";
        samplePath(indexSet);
        value->setRedrawHistory(false);
    }
    
    std::cout << "redrawValue done\n";
    //value->print();
}


void DispersalHistoryCtmc::simulate(void)
{
    
    //std::cout << "ns " << numStates << "   nc " << numCharacters << "\n";
    
    std::set<size_t> indexSet;
    for (size_t i = 0; i < numCharacters; i++)
        indexSet.insert(i);

    simulatePath(indexSet);

    //value->print();
}

void DispersalHistoryCtmc::simulatePath(const std::set<size_t>& indexSet)
{
    value->clearEvents(indexSet);
    
    // branch length modifiers
    double br = branchRate->getValue();
    double bt = tree->getValue().getBranchLength(index) / tree->getValue().getRoot().getAge();
    if (bt == 0.0) // root bt
        bt = 10.0;
    
    //std::cout << br << " " << bt << "\n";
    
    // start state
    std::vector<CharacterEvent*> currState = value->getParentCharacters();
    std::set<CharacterEvent*,CharacterEventCompare> history;
    
    // simulate to end state
    double t = 0.0;
    unsigned int n = numOn(currState);
    do
    {
        // sample next event time
        double r_sum = sumOfRates(currState);
        t += RbStatistics::Exponential::rv( r_sum * bt * br, *GLOBAL_RNG );
        
        //std::cout << t << " " << r_sum << " " << bt << " " << br << "\n";
        //std::cout << "t " << t << "\t";
        //for (size_t i = 0; i < currState.size(); i++)
        //    std::cout << currState[i]->getState() << " ";
        //std::cout << "\n";
        
        if (t < 1.0)
        {
            // sample next event index
            double u = GLOBAL_RNG->uniform01() * r_sum;
            for (size_t i = 0; i < numCharacters; i++)
            {
                unsigned int s = (currState[i]->getState() == 0 ? 1 : 0);

                // forbid extinction
                if (n == 1 && s == 0)
                    continue;
                
                CharacterEvent* evt = new CharacterEvent(i,s,t);
                double tx_rate = transitionRate(currState, evt);
                u -= tx_rate;
                
                //std::cout << "\tu " << u << ": " << i << " " << s << " " << t << " " << r_sum << " " << n << " " << tx_rate << " " << bt << " " << br << "\n";
                if (u <= 0.0)
                {
                    currState[i] = evt;
                    history.insert(evt);
                    
                    // check how many areas on
                    if (s == 0)
                        n--;
                    else
                        n++;
                    break;
                }
            }
        }
    }
    while (t < 1.0);
    
    value->setHistory(history);
    value->setChildCharacters(currState);

}

unsigned int DispersalHistoryCtmc::numOn(std::vector<CharacterEvent*> v)
{
    unsigned int numOn = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i]->getState() == 1)
            numOn++;
    }

    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*>::iterator it = s.begin(); it != s.end(); it++)
    {
        if ( (*it)->getState() == 1)
            numOn++;
    }
    
    return numOn;
}

unsigned int DispersalHistoryCtmc::numOn(std::set<CharacterEvent*,CharacterEventCompare> s)
{
    unsigned int numOn = 0;
    for (std::set<CharacterEvent*,CharacterEventCompare>::iterator it = s.begin(); it != s.end(); it++)
    {
        if ( (*it)->getState() == 1)
            numOn++;
    }
    
    return numOn;
    
}
